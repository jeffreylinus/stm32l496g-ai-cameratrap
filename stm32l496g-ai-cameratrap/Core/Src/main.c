/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#undef  AI_DEPRECATED
#define AI_DEPRECATED
#include "cameratrap.h"            // AI network
#include "cameratrap_data.h"       // weights + activations
#include "ai_platform.h"
#include <string.h>
#include <stddef.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;

DCMI_HandleTypeDef hdcmi;

DFSDM_Channel_HandleTypeDef hdfsdm1_channel1;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel2;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

QSPI_HandleTypeDef hqspi;

SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

SRAM_HandleTypeDef hsram1;
SRAM_HandleTypeDef hsram2;

/* USER CODE BEGIN PV */

// Image dimensions
// Image dimensions
#define IMG_W      120
#define IMG_H      160
#define IMG_CH     1
#define IMG_PIXELS (IMG_W * IMG_H * IMG_CH)

#define START_BYTE         0xAA
#define INPUT_ZERO_POINT   (-128)
#define OUTPUT_ZERO_POINT  (-128)

// Output quantization (from X-CUBE-AI report: scale=1/256, zp=-128)
#define OUT_SCALE       (0.00390625f)  // 1/256
#define OUT_ZERO_POINT  (-128)

// JPEG settings
#define JPEG_QUALITY    75
#define JPEG_MAX_BYTES  (IMG_PIXELS * 3)   // worst-case upper bound


// AI input/output data buffers (int8)
AI_ALIGNED(4) static ai_i8    g_ai_input[AI_CAMERATRAP_IN_1_SIZE];
AI_ALIGNED(4) static ai_i8    g_ai_output[AI_CAMERATRAP_OUT_1_SIZE];

// Activations buffer
AI_ALIGNED(4) static uint8_t  g_activations[AI_CAMERATRAP_DATA_ACTIVATIONS_SIZE];

// Network handle
static ai_handle g_network = AI_HANDLE_NULL;

/* NEW: pointers to model's IO tensor descriptors */
static ai_buffer *g_ai_input_hdl  = NULL;
static ai_buffer *g_ai_output_hdl = NULL;

// Raw image buffers (0–255 grayscale)
static uint8_t g_img_raw[IMG_PIXELS];
static uint8_t g_img_flipped[IMG_PIXELS];   // for horizontally flipped image
static uint8_t g_crop[IMG_PIXELS];          // buffer for cropped ROI (max full image)
static int capped = 0;
#define I2C_SLAVE_ADDR 0x08


// JPEG output buffer
static uint8_t g_jpeg_buf[JPEG_MAX_BYTES];

// debug output
static void CaptureAndSend_BMP(UART_HandleTypeDef *input_uart);
static void I2C2_SendExact(const uint8_t *buf, uint32_t len);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
static void MX_DCMI_Init(void);
static void MX_DFSDM1_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_SAI1_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
static void AI_Init(void);
static void AI_Run(void);
static void AI_CommunicationLoop(UART_HandleTypeDef *input_uart);

static void UART_RecvExact(uint8_t *buf, uint32_t len, UART_HandleTypeDef *uart);
static void UART_SendExact(const uint8_t *buf, uint32_t len, UART_HandleTypeDef *uart);
static void Preprocess_Image_To_AI(const uint8_t *src, ai_i8 *dst);

// NEW helpers for TTA
static void Flip_Image_Horiz(const uint8_t *src, uint8_t *dst);
static void Dequantize_BBox(const ai_i8 *q, float *bbox4);
static void Quantize_BBox(const float *bbox4, ai_i8 *q);
static void Map_Flipped_BBox(float *bbox4);   // cx -> 1 - cx
static void BBox_To_CropRect(const float *bbox,
                             float scale,
                             int *x0, int *y0,
                             int *w,  int *h);

/* JPEG encoder (implemented in another .c file you add to the project) */
int jpeg_encode_gray(const uint8_t *src,
                     int            width,
                     int            height,
                     int            quality,
                     uint8_t       *dst,
                     uint32_t       dst_max);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Send_Dummy_I2C2(void)
{
    // Header: cw=25, ch=40, crop_len=1000
    uint8_t header[8] = {
        0x19, 0x00,        // cw = 25
        0x28, 0x00,        // ch = 40
        0xE8, 0x03, 0x00, 0x00  // crop_len = 1000
    };

    // Dummy payload: 1000 bytes
    static uint8_t dummy[1000];
    for (uint32_t i = 0; i < 1000; ++i) {
        dummy[i] = (uint8_t)(i & 0xFF);   // 0,1,2,...,255,0,1,2,...
        // or use 0x55 / 0xAA if you want a fixed pattern
        // dummy[i] = 0x55;
    }

    // Send over I2C2 using your existing helper
    I2C2_SendExact(header, sizeof(header));
    I2C2_SendExact(dummy, sizeof(dummy));
}


#include <string.h>

// Non-blocking, :
// - If a byte is present from Arduino (USART1), send it to PC (USART2).
// - If a byte is present from PC (USART2), send it to Arduino (USART1).
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_DCMI_Init();
  MX_DFSDM1_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_QUADSPI_Init();
  MX_SAI1_Init();
  // MX_SDMMC1_SD_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  AI_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	    uint8_t rx = 0xFF;
	    /*
	    Send_Dummy_I2C2();
	    HAL_Delay(5000);
		*/

	    // Blocking receive of 4 bytes on I2C2 (slave)
	    if (HAL_I2C_Slave_Receive(&hi2c2, &rx, 1, 10000) == HAL_OK)
	    {
	        if (true || rx == 1)
	        {
	            const char *dbg = "start\r\n";
	            HAL_UART_Transmit(&huart2, (uint8_t*)dbg, (uint16_t)strlen(dbg), 100);
	            for (int i = 0; i < 11; i++){
	            	HAL_UART_Transmit(&huart2, (uint8_t*)dbg, (uint16_t)strlen(dbg), 100);
	            	AI_CommunicationLoop(&huart1);
	            	if (capped == 1){
	            		capped = 0;
	            		break;
	            	}
	            	HAL_Delay(2000);
	            }
	        }
	        HAL_Delay(2000);
	    }
	  // BridgeStep();
	//HAL_Delay(200);  // Delay between frames to prevent buffer overflow

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 12;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV4;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV4;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV4;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief DCMI Initialization Function
  * @param None
  * @retval None
  */
static void MX_DCMI_Init(void)
{

  /* USER CODE BEGIN DCMI_Init 0 */

  /* USER CODE END DCMI_Init 0 */

  /* USER CODE BEGIN DCMI_Init 1 */

  /* USER CODE END DCMI_Init 1 */
  hdcmi.Instance = DCMI;
  hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
  hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_FALLING;
  hdcmi.Init.VSPolarity = DCMI_VSPOLARITY_LOW;
  hdcmi.Init.HSPolarity = DCMI_HSPOLARITY_LOW;
  hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
  hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;
  hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
  hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
  hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
  hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;
  if (HAL_DCMI_Init(&hdcmi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCMI_Init 2 */

  /* USER CODE END DCMI_Init 2 */

}

/**
  * @brief DFSDM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DFSDM1_Init(void)
{

  /* USER CODE BEGIN DFSDM1_Init 0 */

  /* USER CODE END DFSDM1_Init 0 */

  /* USER CODE BEGIN DFSDM1_Init 1 */

  /* USER CODE END DFSDM1_Init 1 */
  hdfsdm1_channel1.Instance = DFSDM1_Channel1;
  hdfsdm1_channel1.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel1.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel1.Init.OutputClock.Divider = 2;
  hdfsdm1_channel1.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel1.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel1.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel1.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel1.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel1.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel1.Init.Awd.Oversampling = 1;
  hdfsdm1_channel1.Init.Offset = 0;
  hdfsdm1_channel1.Init.RightBitShift = 0x00;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel1) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel2.Instance = DFSDM1_Channel2;
  hdfsdm1_channel2.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel2.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel2.Init.OutputClock.Divider = 2;
  hdfsdm1_channel2.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel2.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel2.Init.Input.Pins = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  hdfsdm1_channel2.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel2.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel2.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel2.Init.Awd.Oversampling = 1;
  hdfsdm1_channel2.Init.Offset = 0;
  hdfsdm1_channel2.Init.RightBitShift = 0x00;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DFSDM1_Init 2 */

  /* USER CODE END DFSDM1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10D19CE4;
  hi2c2.Init.OwnAddress1 = 100;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 209700;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_7B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 1000000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 1000000;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 2;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 23;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief SAI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.DataSize = SAI_DATASIZE_8;
  hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai_BlockA1.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_192K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  hsai_BlockA1.FrameInit.FrameLength = 8;
  hsai_BlockA1.FrameInit.ActiveFrameLength = 1;
  hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_STARTFRAME;
  hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockA1.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
  hsai_BlockA1.SlotInit.FirstBitOffset = 0;
  hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockA1.SlotInit.SlotNumber = 1;
  hsai_BlockA1.SlotInit.SlotActive = 0x00000000;
  if (HAL_SAI_Init(&hsai_BlockA1) != HAL_OK)
  {
    Error_Handler();
  }
  hsai_BlockB1.Instance = SAI1_Block_B;
  hsai_BlockB1.Init.Protocol = SAI_FREE_PROTOCOL;
  hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_RX;
  hsai_BlockB1.Init.DataSize = SAI_DATASIZE_8;
  hsai_BlockB1.Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai_BlockB1.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;
  hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
  hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  hsai_BlockB1.FrameInit.FrameLength = 8;
  hsai_BlockB1.FrameInit.ActiveFrameLength = 1;
  hsai_BlockB1.FrameInit.FSDefinition = SAI_FS_STARTFRAME;
  hsai_BlockB1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockB1.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
  hsai_BlockB1.SlotInit.FirstBitOffset = 0;
  hsai_BlockB1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockB1.SlotInit.SlotNumber = 1;
  hsai_BlockB1.SlotInit.SlotActive = 0x00000000;
  if (HAL_SAI_Init(&hsai_BlockB1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK2;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_DISABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Perform the SRAM2 memory initialization sequence
  */
  hsram2.Instance = FMC_NORSRAM_DEVICE;
  hsram2.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram2.Init */
  hsram2.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram2.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram2.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram2.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram2.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram2.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram2.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram2.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram2.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram2.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram2.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram2.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram2.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram2.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram2.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram2, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, MFX_WAKEUP_Pin|LCD_PWR_ON_Pin|MIC_VDD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Audio_RST_GPIO_Port, Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : JOY_DOWN_Pin JOY_LEFT_Pin JOY_UP_Pin */
  GPIO_InitStruct.Pin = JOY_DOWN_Pin|JOY_LEFT_Pin|JOY_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : STMOD_INT_Pin */
  GPIO_InitStruct.Pin = STMOD_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STMOD_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_BL_Pin */
  GPIO_InitStruct.Pin = LCD_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D6_Pin */
  GPIO_InitStruct.Pin = ARD_D6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
  HAL_GPIO_Init(ARD_D6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ARD_D3_Pin ARD_D9_Pin */
  GPIO_InitStruct.Pin = ARD_D3_Pin|ARD_D9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : ARD_D5_Pin */
  GPIO_InitStruct.Pin = ARD_D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(ARD_D5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_TE_Pin */
  GPIO_InitStruct.Pin = LCD_TE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LCD_TE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : MFX_WAKEUP_Pin LCD_PWR_ON_Pin MIC_VDD_Pin */
  GPIO_InitStruct.Pin = MFX_WAKEUP_Pin|LCD_PWR_ON_Pin|MIC_VDD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : Audio_RST_Pin */
  GPIO_InitStruct.Pin = Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Audio_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CTP_INT_Pin */
  GPIO_InitStruct.Pin = CTP_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CTP_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DCMI_CLK_Pin */
  GPIO_InitStruct.Pin = DCMI_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF14_LPTIM2;
  HAL_GPIO_Init(DCMI_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MFX_IRQ_OUT_Pin */
  GPIO_InitStruct.Pin = MFX_IRQ_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MFX_IRQ_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : JOY_RIGHT_Pin */
  GPIO_InitStruct.Pin = JOY_RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(JOY_RIGHT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STMOD_RESET_Pin */
  GPIO_InitStruct.Pin = STMOD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STMOD_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
static void UART_RecvExact(uint8_t *buf, uint32_t len, UART_HandleTypeDef *uart)
{
    if (HAL_UART_Receive(uart, buf, len, HAL_MAX_DELAY) != HAL_OK)
        Error_Handler();
}

static HAL_StatusTypeDef UART_RecvIMG(uint8_t *dst,
                                      uint32_t dst_width,     // e.g. 160
                                      uint32_t dst_height,    // e.g. 120
                                      UART_HandleTypeDef *uart,
                                      uint32_t src_width,     // e.g. 320
                                      uint32_t src_height)    // e.g. 240
{
    uint32_t src_len = src_width * src_height;
    uint32_t dst_len = dst_width * dst_height;

    uint32_t dst_index = 0;
    uint32_t row = 0;
    uint32_t col = 0;

    for (uint32_t i = 0; i < src_len; i++) {

        uint8_t pixel;

        // Read ONE pixel from UART
        if (HAL_UART_Receive(uart, &pixel, 1, 1000) != HAL_OK) {
            return HAL_ERROR;
        }

        // Only store if we still have space
        if (dst_index < dst_len) {

            // Hard-coded: keep only even row and even column (2× decimation)
            if ((row % 2) == 0 && (col % 2) == 0) {
                dst[dst_index++] = pixel;
            }
        }

        // Advance position in the source frame
        col++;
        if (col == src_width) {
            col = 0;
            row++;
        }
    }

    return HAL_OK;
}

static void Transpose_160x120_to_120x160(
    const uint8_t *src,   // size 160*120
    uint8_t       *dst)   // size 120*160
{
    const uint32_t SRC_W = 160;
    const uint32_t SRC_H = 120;
    const uint32_t DST_W = 120;
    const uint32_t DST_H = 160;

    // Sanity: SRC_W * SRC_H == DST_W * DST_H == 19200

    for (uint32_t y = 0; y < SRC_H; ++y) {       // y in [0,119]
        for (uint32_t x = 0; x < SRC_W; ++x) {   // x in [0,159]
            uint32_t src_idx = y * SRC_W + x;

            // Transpose: dst(x',y') = src(x,y) with x' = y, y' = x
            uint32_t dst_x   = y;                // in [0,119]
            uint32_t dst_y   = x;                // in [0,159]
            uint32_t dst_idx = dst_y * DST_W + dst_x;

            dst[dst_idx] = src[src_idx];
        }
    }
}



static void UART_SendExact(const uint8_t *buf, uint32_t len, UART_HandleTypeDef *uart)
{
    if (HAL_UART_Transmit(uart, (uint8_t*)buf, len, HAL_MAX_DELAY) != HAL_OK)
        Error_Handler();
}

// Convert grayscale [0–255] → int8 input using q = x + (-128)
static void Preprocess_Image_To_AI(const uint8_t *src, ai_i8 *dst)
{
    for (uint32_t i = 0; i < IMG_PIXELS; ++i)
    {
        int16_t v = (int16_t)src[i] + INPUT_ZERO_POINT; // INPUT_ZERO_POINT = -128
        if (v < -128) v = -128;
        if (v > 127)  v = 127;
        dst[i] = (ai_i8)v;
    }
}

// Flip image horizontally: dst(x,y) = src(W-1-x, y)
static void Flip_Image_Horiz(const uint8_t *src, uint8_t *dst)
{
    for (uint32_t y = 0; y < IMG_H; ++y)
    {
        const uint8_t *row_src = &src[y * IMG_W];
        uint8_t *row_dst       = &dst[y * IMG_W];
        for (uint32_t x = 0; x < IMG_W; ++x)
        {
            row_dst[x] = row_src[IMG_W - 1 - x];
        }
    }
}

// Dequantize 4-element bbox int8 -> float [cx, cy, w, h]
static void Dequantize_BBox(const ai_i8 *q, float *bbox4)
{
    for (int i = 0; i < 4; ++i)
    {
        bbox4[i] = ((float)q[i] - OUT_ZERO_POINT) * OUT_SCALE;
    }
}

// Quantize float bbox [cx,cy,w,h] back to int8
static void Quantize_BBox(const float *bbox4, ai_i8 *q)
{
    for (int i = 0; i < 4; ++i)
    {
        float v = bbox4[i];
        if (v < 0.0f) v = 0.0f;
        if (v > 1.0f) v = 1.0f;   // boxes are normalized

        float qf = v / OUT_SCALE + OUT_ZERO_POINT;
        int32_t qi = (int32_t)((qf >= 0.0f) ? (qf + 0.5f) : (qf - 0.5f));

        if (qi < -128) qi = -128;
        if (qi > 127)  qi = 127;
        q[i] = (ai_i8)qi;
    }
}

// Map bbox predicted on horizontally flipped image back to original:
// [cx, cy, w, h] -> [1-cx, cy, w, h]
static void Map_Flipped_BBox(float *bbox4)
{
    bbox4[0] = 1.0f - bbox4[0];
}

static void AI_Init(void)
{
    ai_error err;

    const ai_handle act[] = { g_activations };

    /* Create and initialize the network instance */
    err = ai_cameratrap_create_and_init(&g_network, act, NULL);
    if (err.type != AI_ERROR_NONE)
    {
        Error_Handler();
    }

    /* Get pointers to the network's IO tensor descriptors */
    g_ai_input_hdl  = ai_cameratrap_inputs_get(g_network, NULL);
    g_ai_output_hdl = ai_cameratrap_outputs_get(g_network, NULL);

    if ((!g_ai_input_hdl) || (!g_ai_output_hdl))
    {
        Error_Handler();
    }
}

// Convert [cx, cy, w, h] in [0,1] to a pixel crop rectangle
// scaled by 'scale' around center, clamped to image bounds.
static void BBox_To_CropRect(const float *bbox,
                             float scale,
                             int *x0, int *y0,
                             int *w,  int *h)
{
    float cx = bbox[0];
    float cy = bbox[1];
    float bw = bbox[2] * scale;
    float bh = bbox[3] * scale;

    // Clamp scaled size back to [0,1]
    if (bw > 1.0f) bw = 1.0f;
    if (bh > 1.0f) bh = 1.0f;
    if (bw < 0.0f) bw = 0.0f;
    if (bh < 0.0f) bh = 0.0f;

    float fw = bw * (float)IMG_W;
    float fh = bh * (float)IMG_H;

    float fx0 = cx * (float)IMG_W - fw * 0.5f;
    float fy0 = cy * (float)IMG_H - fh * 0.5f;
    float fx1 = fx0 + fw;
    float fy1 = fy0 + fh;

    // Clamp to image bounds [0, W] x [0, H]
    if (fx0 < 0.0f)      fx0 = 0.0f;
    if (fy0 < 0.0f)      fy0 = 0.0f;
    if (fx1 > IMG_W)     fx1 = (float)IMG_W;
    if (fy1 > IMG_H)     fy1 = (float)IMG_H;

    // Recompute width/height from clamped corners
    int ix0 = (int)(fx0 + 0.5f);
    int iy0 = (int)(fy0 + 0.5f);
    int ix1 = (int)(fx1 + 0.5f);
    int iy1 = (int)(fy1 + 0.5f);

    // Ensure at least 1 pixel
    if (ix1 <= ix0) ix1 = ix0 + 1;
    if (iy1 <= iy0) iy1 = iy0 + 1;

    *x0 = ix0;
    *y0 = iy0;
    *w  = ix1 - ix0;
    *h  = iy1 - iy0;
}


static void AI_Run(void)
{
    ai_i32 nbatch;

    /* Attach our data buffers to the IO handlers */
    g_ai_input_hdl[0].data  = (void*)g_ai_input;
    g_ai_output_hdl[0].data = (void*)g_ai_output;

    /* Perform inference */
    nbatch = ai_cameratrap_run(g_network,
                               (const ai_buffer*)g_ai_input_hdl,
                               g_ai_output_hdl);

    if (nbatch != 1)
    {
        Error_Handler();
    }
}

// Encode a grayscale crop (w x h) into a 24-bit BMP in dst.
// Returns total file size (bytes) on success, or -1 on error.
static int Encode_Crop_BMP_ToBuf(const uint8_t *src,
                                 int            w,
                                 int            h,
                                 uint8_t       *dst,
                                 uint32_t       dst_max)
{
    if (!src || !dst || w <= 0 || h <= 0) {
        return -1;
    }

    uint32_t row_bytes   = (uint32_t)w * 3U;                 // 3 bytes/pixel
    uint32_t row_padded  = (row_bytes + 3U) & ~3U;           // 4-byte aligned
    uint32_t img_size    = row_padded * (uint32_t)h;
    uint32_t file_size   = 54U + img_size;                   // 54-byte header

    if (file_size > dst_max) {
        return -1;
    }

    uint8_t *p = dst;

    // ----- BITMAPFILEHEADER (14 bytes) -----
    p[0] = 'B';
    p[1] = 'M';

    p[2] = (uint8_t)(file_size      );
    p[3] = (uint8_t)(file_size >>  8);
    p[4] = (uint8_t)(file_size >> 16);
    p[5] = (uint8_t)(file_size >> 24);

    p[6] = p[7] = p[8] = p[9] = 0;   // reserved

    p[10] = 54;  // pixel data offset
    p[11] = 0;
    p[12] = 0;
    p[13] = 0;

    // ----- BITMAPINFOHEADER (40 bytes) -----
    p[14] = 40;  // biSize
    p[15] = p[16] = p[17] = 0;

    // biWidth = w
    p[18] = (uint8_t)(w      );
    p[19] = (uint8_t)(w >>  8);
    p[20] = (uint8_t)(w >> 16);
    p[21] = (uint8_t)(w >> 24);

    // biHeight = h (positive → bottom-up)
    p[22] = (uint8_t)(h      );
    p[23] = (uint8_t)(h >>  8);
    p[24] = (uint8_t)(h >> 16);
    p[25] = (uint8_t)(h >> 24);

    // biPlanes = 1
    p[26] = 1;
    p[27] = 0;

    // biBitCount = 24
    p[28] = 24;
    p[29] = 0;

    // biCompression = BI_RGB = 0
    p[30] = p[31] = p[32] = p[33] = 0;

    // biSizeImage = img_size
    p[34] = (uint8_t)(img_size      );
    p[35] = (uint8_t)(img_size >>  8);
    p[36] = (uint8_t)(img_size >> 16);
    p[37] = (uint8_t)(img_size >> 24);

    // biXPelsPerMeter, biYPelsPerMeter
    p[38] = p[39] = p[40] = p[41] = 0;
    p[42] = p[43] = p[44] = p[45] = 0;

    // biClrUsed, biClrImportant
    p[46] = p[47] = p[48] = p[49] = 0;
    p[50] = p[51] = p[52] = p[53] = 0;

    // ----- Pixel data (bottom-up) -----
    uint8_t *pixel = p + 54;

    for (int y = h - 1; y >= 0; --y) {
        const uint8_t *row_src = &src[(uint32_t)y * (uint32_t)w];

        // Write pixels as B,G,R = gray,gray,gray
        for (int x = 0; x < w; ++x) {
            uint8_t gray = row_src[x];
            *pixel++ = gray;  // B
            *pixel++ = gray;  // G
            *pixel++ = gray;  // R
        }

        // Row padding to 4-byte boundary
        uint32_t pad = row_padded - row_bytes;
        for (uint32_t i = 0; i < pad; ++i) {
            *pixel++ = 0;
        }
    }

    return (int)file_size;
}


static void Send_120x160_BMP_Grayscale(UART_HandleTypeDef *out_uart)
{
    // Hard-coded image dimensions
    const uint32_t width  = 120;
    const uint32_t height = 160;

    // 24-bit BMP → 3 bytes per pixel
    const uint32_t row_bytes = 360;         // 120 * 3 = 360 (already divisible by 4 → no padding)
    const uint32_t img_size  = 57600;       // 360 * 160 = 57,600 bytes
    const uint32_t file_size = 54 + 57600;  // BMP header (54) + data

    uint8_t header[54];

    // ---------------- BITMAPFILEHEADER ----------------
    header[0] = 'B';
    header[1] = 'M';

    header[2] = (uint8_t)(file_size & 0xFF);
    header[3] = (uint8_t)((file_size >> 8) & 0xFF);
    header[4] = (uint8_t)((file_size >> 16) & 0xFF);
    header[5] = (uint8_t)((file_size >> 24) & 0xFF);

    header[6] = 0;
    header[7] = 0;
    header[8] = 0;
    header[9] = 0;

    header[10] = 54;   // Pixel data offset
    header[11] = 0;
    header[12] = 0;
    header[13] = 0;

    // ---------------- BITMAPINFOHEADER ----------------
    header[14] = 40;  // biSize
    header[15] = 0;
    header[16] = 0;
    header[17] = 0;

    // Width = 120
    header[18] = 120;
    header[19] = 0;
    header[20] = 0;
    header[21] = 0;

    // Height = 160 (positive → bottom-up)
    header[22] = 160;
    header[23] = 0;
    header[24] = 0;
    header[25] = 0;

    // Planes = 1
    header[26] = 1;
    header[27] = 0;

    // BitCount = 24
    header[28] = 24;
    header[29] = 0;

    // Compression = BI_RGB = 0
    header[30] = 0;
    header[31] = 0;
    header[32] = 0;
    header[33] = 0;

    // Image size = 57,600
    header[34] = (uint8_t)(img_size & 0xFF);
    header[35] = (uint8_t)((img_size >> 8) & 0xFF);
    header[36] = (uint8_t)((img_size >> 16) & 0xFF);
    header[37] = (uint8_t)((img_size >> 24) & 0xFF);

    // XPelsPerMeter, YPelsPerMeter (unused)
    header[38] = header[39] = header[40] = header[41] = 0;
    header[42] = header[43] = header[44] = header[45] = 0;

    // ClrUsed, ClrImportant = 0
    header[46] = header[47] = header[48] = header[49] = 0;
    header[50] = header[51] = header[52] = header[53] = 0;

    // ---- Send BMP header ----
    UART_SendExact(header, 54, out_uart);

    // ---- Send pixel data (bottom-up) ----
    uint8_t line[360];  // 120 pixels * 3 bytes

    for (int y = 159; y >= 0; --y)     // bottom row first
    {
        for (int x = 0; x < 120; ++x)
        {
            uint8_t gray = g_img_raw[y * 120 + x];

            line[3*x + 0] = gray;  // B
            line[3*x + 1] = gray;  // G
            line[3*x + 2] = gray;  // R
        }

        UART_SendExact(line, 360, out_uart);
    }
}
// Flip image vertically in-place: img(x, y) -> img(x, H-1-y)
static void Flip_Image_Vert_InPlace(uint8_t *img)
{
    for (uint32_t y = 0; y < IMG_H / 2; ++y)
    {
        uint32_t opp_y = IMG_H - 1U - y;  // opposite row
        for (uint32_t x = 0; x < IMG_W; ++x)
        {
            uint32_t idx1 = y      * IMG_W + x;
            uint32_t idx2 = opp_y  * IMG_W + x;
            uint8_t tmp   = img[idx1];
            img[idx1]     = img[idx2];
            img[idx2]     = tmp;
        }
    }
}


static void I2C2_SendExact(const uint8_t *buf, uint32_t len)
{
    const uint8_t dev_addr = (uint8_t)(I2C_SLAVE_ADDR << 1);

    uint32_t total = len;

    while (len > 0U) {

        uint16_t chunk = (len > 64) ? 64U : (uint16_t)len;

        HAL_StatusTypeDef st =
            HAL_I2C_Master_Transmit(&hi2c2,
                                    dev_addr,
                                    (uint8_t*)buf,
                                    chunk,
                                    2000);

        if (st != HAL_OK) {
            char msg[80];
            int n = snprintf(msg, sizeof(msg),
                             "I2C2 TX FAIL: sent=%lu/%lu err=0x%lX\r\n",
                             (unsigned long)(total - len),
                             (unsigned long)total,
                             (unsigned long)hi2c2.ErrorCode);
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, n, 200);
            return;
        }

        // Success for this chunk
        {
            char msg[80];
            int n = snprintf(msg, sizeof(msg),
                             "I2C2 TX OK chunk=%u rem=%lu\r\n",
                             (unsigned int)chunk,
                             (unsigned long)(len - chunk));
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, n, 200);
        }

        buf += chunk;
        len -= chunk;
    }

    // Finished all data
    {
        const char *done = "I2C2 TX COMPLETE\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t*)done, strlen(done), 200);
    }
}


static void AI_CommunicationLoop(UART_HandleTypeDef *input_uart)
{
	const char *dbg = "here\r\n";
    uint8_t start = 'S';
    const char *dbg1 = "at\r\n";

    // 1) Tell Arduino we're ready for a frame
    UART_SendExact(&start, 1, input_uart);
    HAL_UART_Transmit(&huart2, (uint8_t*)dbg, (uint16_t)strlen(dbg), 100);
    // 2) Receive full grayscale image (source 320x240 → decimated into 160x120 buffer)
    if (UART_RecvIMG(g_img_flipped, IMG_H, IMG_W, input_uart, 320, 240) != HAL_OK)
        return;
    HAL_UART_Transmit(&huart2, (uint8_t*)dbg, (uint16_t)strlen(dbg), 100);
    HAL_UART_Transmit(&huart2, (uint8_t*)dbg1, (uint16_t)strlen(dbg1), 100);

    // 3) Rotate / flip into AI input orientation
    Transpose_160x120_to_120x160(g_img_flipped, g_img_raw);
    Flip_Image_Vert_InPlace(g_img_raw);

    float bbox1[4];
    float bbox2[4];
    float bbox_avg[4];



    // ---- First inference: original image ----
    Preprocess_Image_To_AI(g_img_raw, g_ai_input);
    AI_Run();
    Dequantize_BBox(g_ai_output, bbox1);

    // ---- Second inference: horizontally flipped image ----
    Flip_Image_Horiz(g_img_raw, g_img_flipped);
    Preprocess_Image_To_AI(g_img_flipped, g_ai_input);
    AI_Run();
    Dequantize_BBox(g_ai_output, bbox2);

    // Map flipped bbox back to original coordinates (cx -> 1 - cx)
    Map_Flipped_BBox(bbox2);

    // ---- Average the two bboxes ----
    for (int i = 0; i < 4; ++i)
        bbox_avg[i] = 0.5f * (bbox1[i] + bbox2[i]);

    // ---- Quantize averaged bbox (still 4 int8) ----
    Quantize_BBox(bbox_avg, g_ai_output);

    // ---- Compute expanded crop rectangle in pixels ----
    int x0, y0, cw, ch;
    BBox_To_CropRect(bbox_avg, 1.1f, &x0, &y0, &cw, &ch);

    // Sanity check vs g_crop capacity
    if (cw <= 0 || ch <= 0)
        return;

    uint32_t crop_len = (uint32_t)cw * (uint32_t)ch;
    if (crop_len > IMG_PIXELS)
        return;

    // ---- Extract crop from original image into g_crop ----
    for (int yy = 0; yy < ch; ++yy) {
        const uint8_t *src_row = &g_img_raw[(y0 + yy) * IMG_W + x0];
        uint8_t       *dst_row = &g_crop[yy * cw];
        memcpy(dst_row, src_row, (size_t)cw);
    }

    // ================== TRANSMIT PROTOCOL ==================
    // Layout (over huart2 -> PC):
    // [4 bytes bbox int8] +
    // [2 bytes cw][2 bytes ch][4 bytes crop_len] +
    // [crop_len bytes raw grayscale g_crop]
    // =======================================================

    // 1) Send quantized bbox (4 int8)
    /*
    UART_SendExact((uint8_t*)g_ai_output,
                   AI_CAMERATRAP_OUT_1_SIZE,   // should be 4
                   &huart2);
	*/
    // 2) Send 8-byte header: cw, ch, crop_len (all little-endian)
    uint8_t header[8];
    header[0] = (uint8_t)(cw & 0xFF);
    header[1] = (uint8_t)((cw >> 8) & 0xFF);
    header[2] = (uint8_t)(ch & 0xFF);
    header[3] = (uint8_t)((ch >> 8) & 0xFF);

    header[4] = (uint8_t)(crop_len & 0xFF);
    header[5] = (uint8_t)((crop_len >> 8) & 0xFF);
    header[6] = (uint8_t)((crop_len >> 16) & 0xFF);
    header[7] = (uint8_t)((crop_len >> 24) & 0xFF);


    // 3) Send raw grayscale crop
    I2C2_SendExact(header, sizeof(header));
    I2C2_SendExact(g_crop, crop_len);
    capped = 1;
    HAL_UART_Transmit(&huart2, (uint8_t*)dbg, (uint16_t)strlen(dbg), 100);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  NVIC_SystemReset();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
