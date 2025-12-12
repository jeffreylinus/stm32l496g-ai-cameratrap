/**
  ******************************************************************************
  * @file    cameratrap.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-12-11T20:34:46-0500
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "cameratrap.h"
#include "cameratrap_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_cameratrap
 
#undef AI_CAMERATRAP_MODEL_SIGNATURE
#define AI_CAMERATRAP_MODEL_SIGNATURE     "0x130bf0f795a7ee68b2dc61dea9610515"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2025-12-11T20:34:46-0500"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_CAMERATRAP_N_BATCHES
#define AI_CAMERATRAP_N_BATCHES         (1)

static ai_ptr g_cameratrap_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_cameratrap_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 19200, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 38400, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 38400, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  pool_2_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  nl_5_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_6_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 38400, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 9600, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 9600, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  pool_9_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  gemm_10_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  nl_10_nl_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  gemm_11_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  nl_12_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_13_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 9600, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4800, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_15_pad_before_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 5984, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_15_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4800, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  pool_16_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  gemm_17_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  gemm_18_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  nl_19_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_20_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4800, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  gemm_22_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  gemm_23_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 64, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  gemm_24_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  nl_25_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 72, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 576, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 2, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 576, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 576, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  gemm_10_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  gemm_10_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 2, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  gemm_11_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  gemm_11_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1152, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_15_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2304, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_15_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  gemm_17_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 64, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  gemm_17_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 4, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  gemm_18_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 64, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  gemm_18_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  gemm_22_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 614400, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  gemm_22_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 128, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  gemm_23_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8192, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  gemm_23_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 64, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  gemm_24_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 256, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  gemm_24_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 4, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 292, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1552, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 8, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 2, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_7_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1552, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1552, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  gemm_10_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 8, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  gemm_11_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 2, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_14_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2816, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_15_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 5408, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  gemm_17_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 16, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  gemm_18_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 4, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  gemm_22_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 4800, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  gemm_23_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 128, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  gemm_24_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 64, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0035416490864008665f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_0_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0023630878422409296f, 0.0022175468038767576f, 0.002066713059321046f, 0.0024430060293525457f, 0.001935602049343288f, 0.0027579113375395536f, 0.0019206716679036617f, 0.002155288588255644f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_14_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00667076138779521f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_14_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0018865034217014909f, 0.0028174996841698885f, 0.0023180574644356966f, 0.0022968328557908535f, 0.0026287136133760214f, 0.0023652531672269106f, 0.0013484646333381534f, 0.002649871865287423f, 0.0020354432053864002f, 0.0022984608076512814f, 0.0027243008371442556f, 0.0019055228913202882f, 0.001464287517592311f, 0.0018789628520607948f, 0.001758747617714107f, 0.0019963339436799288f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_15_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.010716131888329983f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_15_pad_before_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00667076138779521f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_15_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0016955401515588164f, 0.0015117516741156578f, 0.002721887081861496f, 0.0024439822882413864f, 0.0029917401261627674f, 0.0019910719711333513f, 0.0023481608368456364f, 0.0017916616052389145f, 0.0019624491687864065f, 0.0023780460469424725f, 0.0021005342714488506f, 0.0016592213651165366f, 0.002129168715327978f, 0.0026955166831612587f, 0.0019267870811745524f, 0.0033198469318449497f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_1_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.010509620420634747f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_1_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0035834822338074446f, 0.003525417996570468f, 0.004620428662747145f, 0.0022845168132334948f, 0.0033916402608156204f, 0.0019024662906304002f, 0.003313375636935234f, 0.0035243737511336803f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_7_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005218340549618006f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_7_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0030378687661141157f, 0.0023375179152935743f, 0.002939098048955202f, 0.003350168000906706f, 0.0033665583468973637f, 0.0021835975348949432f, 0.002632443094626069f, 0.0025185011327266693f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_8_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005142791662365198f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_8_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0023466807324439287f, 0.0028961142525076866f, 0.003587735816836357f, 0.0032089794985949993f, 0.0024286648258566856f, 0.0031655787024646997f, 0.003628003178164363f, 0.003122280817478895f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #13 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_13_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0028349552303552628f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #14 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_20_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00562735041603446f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #15 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_6_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005404320079833269f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #16 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_10_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(7.84313680668447e-09f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #17 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_10_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005965232849121094f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #18 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_11_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0008067279122769833f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #19 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_11_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.024427419528365135f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #20 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_17_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0061557115986943245f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #21 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_17_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.007461000699549913f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #22 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_18_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.006828174460679293f),
    AI_PACK_INTQ_ZP(10)))

/* Int quant #23 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_18_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005994367878884077f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #24 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_22_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.03246527910232544f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #25 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_22_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003142527537420392f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #26 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_23_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.025058576837182045f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #27 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_23_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0029640041757375f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #28 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_24_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0431898795068264f),
    AI_PACK_INTQ_ZP(-50)))

/* Int quant #29 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_24_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.002950734691694379f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #30 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_3_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005421147681772709f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #31 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_3_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.006071533542126417f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #32 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_4_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008132478222250938f),
    AI_PACK_INTQ_ZP(-43)))

/* Int quant #33 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_4_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.006924987770617008f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #34 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(input_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003921568859368563f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #35 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_10_nl_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(7.84313680668447e-09f),
    AI_PACK_INTQ_ZP(0)))

/* Int quant #36 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_12_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #37 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_19_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #38 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_25_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #39 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_5_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #40 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_16_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0026472597382962704f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #41 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_2_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.008389251306653023f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #42 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_9_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0009329724707640707f),
    AI_PACK_INTQ_ZP(-128)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_bias, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_0_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_output, AI_STATIC,
  1, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 60, 80), AI_STRIDE_INIT(4, 1, 1, 8, 480),
  1, &conv2d_0_output_array, &conv2d_0_output_array_intq)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_scratch0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 292, 1, 1), AI_STRIDE_INIT(4, 1, 1, 292, 292),
  1, &conv2d_0_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_weights, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 1, 3, 3, 8), AI_STRIDE_INIT(4, 1, 1, 8, 24),
  1, &conv2d_0_weights_array, &conv2d_0_weights_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &conv2d_14_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_output, AI_STATIC,
  5, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 15, 20), AI_STRIDE_INIT(4, 1, 1, 16, 240),
  1, &conv2d_14_output_array, &conv2d_14_output_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_scratch0, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 2816, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2816, 2816),
  1, &conv2d_14_scratch0_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_14_weights, AI_STATIC,
  7, 0x1,
  AI_SHAPE_INIT(4, 8, 3, 3, 16), AI_STRIDE_INIT(4, 1, 8, 128, 384),
  1, &conv2d_14_weights_array, &conv2d_14_weights_array_intq)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_15_bias, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &conv2d_15_bias_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_15_output, AI_STATIC,
  9, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 15, 20), AI_STRIDE_INIT(4, 1, 1, 16, 240),
  1, &conv2d_15_output_array, &conv2d_15_output_array_intq)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_15_pad_before_output, AI_STATIC,
  10, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 17, 22), AI_STRIDE_INIT(4, 1, 1, 16, 272),
  1, &conv2d_15_pad_before_output_array, &conv2d_15_pad_before_output_array_intq)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_15_scratch0, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 5408, 1, 1), AI_STRIDE_INIT(4, 1, 1, 5408, 5408),
  1, &conv2d_15_scratch0_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_15_weights, AI_STATIC,
  12, 0x1,
  AI_SHAPE_INIT(4, 16, 3, 3, 16), AI_STRIDE_INIT(4, 1, 16, 256, 768),
  1, &conv2d_15_weights_array, &conv2d_15_weights_array_intq)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_bias, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_1_bias_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_output, AI_STATIC,
  14, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 60, 80), AI_STRIDE_INIT(4, 1, 1, 8, 480),
  1, &conv2d_1_output_array, &conv2d_1_output_array_intq)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_scratch0, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 1552, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1552, 1552),
  1, &conv2d_1_scratch0_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_weights, AI_STATIC,
  16, 0x1,
  AI_SHAPE_INIT(4, 8, 3, 3, 8), AI_STRIDE_INIT(4, 1, 8, 64, 192),
  1, &conv2d_1_weights_array, &conv2d_1_weights_array_intq)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_bias, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_7_bias_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_output, AI_STATIC,
  18, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 30, 40), AI_STRIDE_INIT(4, 1, 1, 8, 240),
  1, &conv2d_7_output_array, &conv2d_7_output_array_intq)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_scratch0, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 1552, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1552, 1552),
  1, &conv2d_7_scratch0_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_7_weights, AI_STATIC,
  20, 0x1,
  AI_SHAPE_INIT(4, 8, 3, 3, 8), AI_STRIDE_INIT(4, 1, 8, 64, 192),
  1, &conv2d_7_weights_array, &conv2d_7_weights_array_intq)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_bias, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_8_bias_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_output, AI_STATIC,
  22, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 30, 40), AI_STRIDE_INIT(4, 1, 1, 8, 240),
  1, &conv2d_8_output_array, &conv2d_8_output_array_intq)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_scratch0, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 1552, 1, 1), AI_STRIDE_INIT(4, 1, 1, 1552, 1552),
  1, &conv2d_8_scratch0_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_weights, AI_STATIC,
  24, 0x1,
  AI_SHAPE_INIT(4, 8, 3, 3, 8), AI_STRIDE_INIT(4, 1, 8, 64, 192),
  1, &conv2d_8_weights_array, &conv2d_8_weights_array_intq)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_13_output, AI_STATIC,
  25, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 30, 40), AI_STRIDE_INIT(4, 1, 1, 8, 240),
  1, &eltwise_13_output_array, &eltwise_13_output_array_intq)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_20_output, AI_STATIC,
  26, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 15, 20), AI_STRIDE_INIT(4, 1, 1, 16, 240),
  1, &eltwise_20_output_array, &eltwise_20_output_array_intq)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_20_output0, AI_STATIC,
  27, 0x1,
  AI_SHAPE_INIT(4, 1, 4800, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4800, 4800),
  1, &eltwise_20_output_array, &eltwise_20_output_array_intq)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_6_output, AI_STATIC,
  28, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 60, 80), AI_STRIDE_INIT(4, 1, 1, 8, 480),
  1, &eltwise_6_output_array, &eltwise_6_output_array_intq)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  gemm_10_bias, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &gemm_10_bias_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  gemm_10_output, AI_STATIC,
  30, 0x1,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &gemm_10_output_array, &gemm_10_output_array_intq)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  gemm_10_scratch0, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 2, 2, 16, 16),
  1, &gemm_10_scratch0_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  gemm_10_weights, AI_STATIC,
  32, 0x1,
  AI_SHAPE_INIT(4, 8, 2, 1, 1), AI_STRIDE_INIT(4, 1, 8, 16, 16),
  1, &gemm_10_weights_array, &gemm_10_weights_array_intq)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  gemm_11_bias, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &gemm_11_bias_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  gemm_11_output, AI_STATIC,
  34, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &gemm_11_output_array, &gemm_11_output_array_intq)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  gemm_11_scratch0, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 2, 2, 4, 4),
  1, &gemm_11_scratch0_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  gemm_11_weights, AI_STATIC,
  36, 0x1,
  AI_SHAPE_INIT(4, 2, 8, 1, 1), AI_STRIDE_INIT(4, 1, 2, 16, 16),
  1, &gemm_11_weights_array, &gemm_11_weights_array_intq)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  gemm_17_bias, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &gemm_17_bias_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  gemm_17_output, AI_STATIC,
  38, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &gemm_17_output_array, &gemm_17_output_array_intq)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  gemm_17_scratch0, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 2, 2, 32, 32),
  1, &gemm_17_scratch0_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  gemm_17_weights, AI_STATIC,
  40, 0x1,
  AI_SHAPE_INIT(4, 16, 4, 1, 1), AI_STRIDE_INIT(4, 1, 16, 64, 64),
  1, &gemm_17_weights_array, &gemm_17_weights_array_intq)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  gemm_18_bias, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &gemm_18_bias_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  gemm_18_output, AI_STATIC,
  42, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &gemm_18_output_array, &gemm_18_output_array_intq)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  gemm_18_scratch0, AI_STATIC,
  43, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 2, 2, 8, 8),
  1, &gemm_18_scratch0_array, NULL)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  gemm_18_weights, AI_STATIC,
  44, 0x1,
  AI_SHAPE_INIT(4, 4, 16, 1, 1), AI_STRIDE_INIT(4, 1, 4, 64, 64),
  1, &gemm_18_weights_array, &gemm_18_weights_array_intq)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  gemm_22_bias, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 4, 4, 512, 512),
  1, &gemm_22_bias_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  gemm_22_output, AI_STATIC,
  46, 0x1,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 1, 1, 128, 128),
  1, &gemm_22_output_array, &gemm_22_output_array_intq)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  gemm_22_scratch0, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 4800, 1, 1), AI_STRIDE_INIT(4, 2, 2, 9600, 9600),
  1, &gemm_22_scratch0_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  gemm_22_weights, AI_STATIC,
  48, 0x1,
  AI_SHAPE_INIT(4, 4800, 128, 1, 1), AI_STRIDE_INIT(4, 1, 4800, 614400, 614400),
  1, &gemm_22_weights_array, &gemm_22_weights_array_intq)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  gemm_23_bias, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 4, 4, 256, 256),
  1, &gemm_23_bias_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  gemm_23_output, AI_STATIC,
  50, 0x1,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 1, 1, 64, 64),
  1, &gemm_23_output_array, &gemm_23_output_array_intq)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  gemm_23_scratch0, AI_STATIC,
  51, 0x0,
  AI_SHAPE_INIT(4, 1, 128, 1, 1), AI_STRIDE_INIT(4, 2, 2, 256, 256),
  1, &gemm_23_scratch0_array, NULL)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  gemm_23_weights, AI_STATIC,
  52, 0x1,
  AI_SHAPE_INIT(4, 128, 64, 1, 1), AI_STRIDE_INIT(4, 1, 128, 8192, 8192),
  1, &gemm_23_weights_array, &gemm_23_weights_array_intq)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  gemm_24_bias, AI_STATIC,
  53, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &gemm_24_bias_array, NULL)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  gemm_24_output, AI_STATIC,
  54, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &gemm_24_output_array, &gemm_24_output_array_intq)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  gemm_24_scratch0, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 64, 1, 1), AI_STRIDE_INIT(4, 2, 2, 128, 128),
  1, &gemm_24_scratch0_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  gemm_24_weights, AI_STATIC,
  56, 0x1,
  AI_SHAPE_INIT(4, 64, 4, 1, 1), AI_STRIDE_INIT(4, 1, 64, 256, 256),
  1, &gemm_24_weights_array, &gemm_24_weights_array_intq)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_bias, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &gemm_3_bias_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_output, AI_STATIC,
  58, 0x1,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &gemm_3_output_array, &gemm_3_output_array_intq)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_scratch0, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 2, 2, 16, 16),
  1, &gemm_3_scratch0_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_weights, AI_STATIC,
  60, 0x1,
  AI_SHAPE_INIT(4, 8, 2, 1, 1), AI_STRIDE_INIT(4, 1, 8, 16, 16),
  1, &gemm_3_weights_array, &gemm_3_weights_array_intq)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_bias, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &gemm_4_bias_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_output, AI_STATIC,
  62, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &gemm_4_output_array, &gemm_4_output_array_intq)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_scratch0, AI_STATIC,
  63, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 2, 2, 4, 4),
  1, &gemm_4_scratch0_array, NULL)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_weights, AI_STATIC,
  64, 0x1,
  AI_SHAPE_INIT(4, 2, 8, 1, 1), AI_STRIDE_INIT(4, 1, 2, 16, 16),
  1, &gemm_4_weights_array, &gemm_4_weights_array_intq)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  input_output, AI_STATIC,
  65, 0x1,
  AI_SHAPE_INIT(4, 1, 1, 120, 160), AI_STRIDE_INIT(4, 1, 1, 1, 120),
  1, &input_output_array, &input_output_array_intq)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  nl_10_nl_output, AI_STATIC,
  66, 0x1,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &nl_10_nl_output_array, &nl_10_nl_output_array_intq)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  nl_12_output, AI_STATIC,
  67, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &nl_12_output_array, &nl_12_output_array_intq)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  nl_19_output, AI_STATIC,
  68, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &nl_19_output_array, &nl_19_output_array_intq)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  nl_25_output, AI_STATIC,
  69, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &nl_25_output_array, &nl_25_output_array_intq)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  nl_5_output, AI_STATIC,
  70, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &nl_5_output_array, &nl_5_output_array_intq)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  pool_16_output, AI_STATIC,
  71, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &pool_16_output_array, &pool_16_output_array_intq)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  pool_2_output, AI_STATIC,
  72, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &pool_2_output_array, &pool_2_output_array_intq)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  pool_9_output, AI_STATIC,
  73, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &pool_9_output_array, &pool_9_output_array_intq)



/**  Layer declarations section  **********************************************/



AI_STATIC_CONST ai_i8 nl_25_nl_params_data[] = { -119, -119, -119, -118, -118, -118, -117, -117, -116, -116, -115, -115, -114, -113, -113, -112, -112, -111, -110, -109, -109, -108, -107, -106, -105, -104, -103, -103, -102, -100, -99, -98, -97, -96, -95, -93, -92, -91, -89, -88, -86, -85, -83, -82, -80, -78, -77, -75, -73, -71, -69, -67, -65, -63, -61, -59, -57, -54, -52, -50, -47, -45, -43, -40, -38, -35, -32, -30, -27, -25, -22, -19, -16, -14, -11, -8, -6, -3, 0, 3, 6, 8, 11, 14, 16, 19, 22, 25, 27, 30, 32, 35, 38, 40, 43, 45, 47, 50, 52, 54, 57, 59, 61, 63, 65, 67, 69, 71, 73, 75, 77, 78, 80, 82, 83, 85, 86, 88, 89, 91, 92, 93, 95, 96, 97, 98, 99, 100, 102, 103, 103, 104, 105, 106, 107, 108, 109, 109, 110, 111, 112, 112, 113, 113, 114, 115, 115, 116, 116, 117, 117, 118, 118, 118, 119, 119, 119, 120, 120, 120, 121, 121, 121, 122, 122, 122, 122, 123, 123, 123, 123, 123, 124, 124, 124, 124, 124, 124, 125, 125, 125, 125, 125, 125, 125, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };
AI_ARRAY_OBJ_DECLARE(
    nl_25_nl_params, AI_ARRAY_FORMAT_S8,
    nl_25_nl_params_data, nl_25_nl_params_data, 256, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_25_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_24_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_25_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_25_layer, 25,
  NL_TYPE, 0x0, NULL,
  nl, forward_nl_integer,
  &nl_25_chain,
  NULL, &nl_25_layer, AI_STATIC, 
  .nl_params = &nl_25_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_24_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_23_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_24_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_24_weights, &gemm_24_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_24_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_24_layer, 24,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_24_chain,
  NULL, &nl_25_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_23_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_22_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_23_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_23_weights, &gemm_23_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_23_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_23_layer, 23,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_23_chain,
  NULL, &gemm_24_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_22_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_20_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_22_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_22_weights, &gemm_22_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_22_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_22_layer, 22,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_22_chain,
  NULL, &gemm_23_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_20_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv2d_15_output, &nl_19_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_20_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  eltwise_20_layer, 20,
  ELTWISE_INTEGER_TYPE, 0x0, NULL,
  eltwise_integer, forward_eltwise_integer_INT8,
  &eltwise_20_chain,
  NULL, &gemm_22_layer, AI_STATIC, 
  .operation = ai_mul_f32, 
  .buffer_operation = ai_mul_buffer_INT8, 
)


AI_STATIC_CONST ai_i8 nl_19_nl_params_data[] = { -56, -56, -56, -55, -55, -54, -54, -54, -53, -53, -53, -52, -52, -52, -51, -51, -50, -50, -50, -49, -49, -49, -48, -48, -47, -47, -47, -46, -46, -46, -45, -45, -44, -44, -44, -43, -43, -42, -42, -42, -41, -41, -41, -40, -40, -39, -39, -39, -38, -38, -37, -37, -37, -36, -36, -35, -35, -35, -34, -34, -33, -33, -32, -32, -32, -31, -31, -30, -30, -30, -29, -29, -28, -28, -28, -27, -27, -26, -26, -25, -25, -25, -24, -24, -23, -23, -22, -22, -22, -21, -21, -20, -20, -20, -19, -19, -18, -18, -17, -17, -17, -16, -16, -15, -15, -14, -14, -13, -13, -13, -12, -12, -11, -11, -10, -10, -10, -9, -9, -8, -8, -7, -7, -7, -6, -6, -5, -5, -4, -4, -3, -3, -3, -2, -2, -1, -1, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 18, 18, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 25, 25, 25, 26, 26, 27, 27, 28, 28, 28, 29, 29, 30, 30, 30, 31, 31, 32, 32, 32, 33, 33, 34, 34, 35, 35, 35, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43, 44, 44, 44, 45, 45, 46, 46, 46, 47, 47, 47, 48, 48, 49 };
AI_ARRAY_OBJ_DECLARE(
    nl_19_nl_params, AI_ARRAY_FORMAT_S8,
    nl_19_nl_params_data, nl_19_nl_params_data, 256, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_19_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_18_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_19_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_19_layer, 19,
  NL_TYPE, 0x0, NULL,
  nl, forward_nl_integer,
  &nl_19_chain,
  NULL, &eltwise_20_layer, AI_STATIC, 
  .nl_params = &nl_19_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_18_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_17_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_18_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_18_weights, &gemm_18_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_18_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_18_layer, 18,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_18_chain,
  NULL, &nl_19_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_17_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_16_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_17_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_17_weights, &gemm_17_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_17_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_17_layer, 17,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_17_chain,
  NULL, &gemm_18_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_16_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_15_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_16_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_16_layer, 16,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap_integer_INT8,
  &pool_16_chain,
  NULL, &gemm_17_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(15, 20), 
  .pool_stride = AI_SHAPE_2D_INIT(15, 20), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_15_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_15_pad_before_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_15_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_15_weights, &conv2d_15_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_15_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_15_layer, 15,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_deep_3x3_sssa8_ch,
  &conv2d_15_chain,
  NULL, &pool_16_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_i8 conv2d_15_pad_before_value_data[] = { -128 };
AI_ARRAY_OBJ_DECLARE(
    conv2d_15_pad_before_value, AI_ARRAY_FORMAT_S8,
    conv2d_15_pad_before_value_data, conv2d_15_pad_before_value_data, 1, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_15_pad_before_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_14_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_15_pad_before_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_15_pad_before_layer, 15,
  PAD_TYPE, 0x0, NULL,
  pad, forward_pad,
  &conv2d_15_pad_before_chain,
  NULL, &conv2d_15_layer, AI_STATIC, 
  .value = &conv2d_15_pad_before_value, 
  .mode = AI_PAD_CONSTANT, 
  .pads = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_14_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_13_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_14_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_14_weights, &conv2d_14_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_14_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_14_layer, 14,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_14_chain,
  NULL, &conv2d_15_pad_before_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_13_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv2d_8_output, &nl_12_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_13_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  eltwise_13_layer, 13,
  ELTWISE_INTEGER_TYPE, 0x0, NULL,
  eltwise_integer, forward_eltwise_integer_INT8,
  &eltwise_13_chain,
  NULL, &conv2d_14_layer, AI_STATIC, 
  .operation = ai_mul_f32, 
  .buffer_operation = ai_mul_buffer_INT8, 
)


AI_STATIC_CONST ai_i8 nl_12_nl_params_data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13 };
AI_ARRAY_OBJ_DECLARE(
    nl_12_nl_params, AI_ARRAY_FORMAT_S8,
    nl_12_nl_params_data, nl_12_nl_params_data, 256, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_12_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_11_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_12_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_12_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_nl_integer,
  &nl_12_chain,
  NULL, &eltwise_13_layer, AI_STATIC, 
  .nl_params = &nl_12_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_11_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_10_nl_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_11_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_11_weights, &gemm_11_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_11_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_11_layer, 11,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_11_chain,
  NULL, &nl_12_layer, AI_STATIC, 
)


AI_STATIC_CONST ai_i8 nl_10_nl_nl_params_data[] = { 0 };
AI_ARRAY_OBJ_DECLARE(
    nl_10_nl_nl_params, AI_ARRAY_FORMAT_S8,
    nl_10_nl_nl_params_data, nl_10_nl_nl_params_data, 1, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_10_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_10_nl_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_10_nl_layer, 10,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu_integer,
  &nl_10_nl_chain,
  NULL, &gemm_11_layer, AI_STATIC, 
  .nl_params = &nl_10_nl_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_9_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_10_weights, &gemm_10_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_10_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_10_layer, 10,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_10_chain,
  NULL, &nl_10_nl_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_9_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_9_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_9_layer, 9,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap_integer_INT8,
  &pool_9_chain,
  NULL, &gemm_10_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(30, 40), 
  .pool_stride = AI_SHAPE_2D_INIT(30, 40), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_8_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_8_weights, &conv2d_8_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_8_layer, 8,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_8_chain,
  NULL, &pool_9_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_7_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_6_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_7_weights, &conv2d_7_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_7_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_7_layer, 7,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_7_chain,
  NULL, &conv2d_8_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_6_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &conv2d_1_output, &nl_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_6_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  eltwise_6_layer, 6,
  ELTWISE_INTEGER_TYPE, 0x0, NULL,
  eltwise_integer, forward_eltwise_integer_INT8,
  &eltwise_6_chain,
  NULL, &conv2d_7_layer, AI_STATIC, 
  .operation = ai_mul_f32, 
  .buffer_operation = ai_mul_buffer_INT8, 
)


AI_STATIC_CONST ai_i8 nl_5_nl_params_data[] = { -43, -42, -42, -41, -41, -40, -40, -39, -39, -38, -38, -37, -37, -36, -36, -35, -35, -35, -34, -34, -33, -33, -32, -32, -31, -31, -30, -30, -29, -29, -28, -28, -27, -27, -26, -26, -25, -25, -24, -24, -23, -23, -22, -22, -21, -21, -20, -20, -19, -19, -18, -18, -17, -17, -16, -16, -15, -15, -14, -13, -13, -12, -12, -11, -11, -10, -10, -9, -9, -8, -8, -7, -7, -6, -6, -5, -5, -4, -4, -3, -3, -2, -2, -1, -1, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 52, 53, 53, 54, 54, 55, 55, 55, 56, 56, 57, 57, 58, 58, 58, 59, 59, 60, 60, 60, 61, 61, 62, 62, 62, 63, 63, 64, 64, 64, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 69, 69, 69, 70, 70, 70, 71, 71, 71, 72, 72, 73, 73, 73, 74, 74, 74, 75, 75, 75, 76, 76, 76, 77 };
AI_ARRAY_OBJ_DECLARE(
    nl_5_nl_params, AI_ARRAY_FORMAT_S8,
    nl_5_nl_params_data, nl_5_nl_params_data, 256, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_5_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_5_layer, 5,
  NL_TYPE, 0x0, NULL,
  nl, forward_nl_integer,
  &nl_5_chain,
  NULL, &eltwise_6_layer, AI_STATIC, 
  .nl_params = &nl_5_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_4_weights, &gemm_4_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_4_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_4_layer, 4,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_4_chain,
  NULL, &nl_5_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_3_weights, &gemm_3_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_3_layer, 3,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA,
  &gemm_3_chain,
  NULL, &gemm_4_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_2_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_2_layer, 2,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap_integer_INT8,
  &pool_2_chain,
  NULL, &gemm_3_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(60, 80), 
  .pool_stride = AI_SHAPE_2D_INIT(60, 80), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_1_weights, &conv2d_1_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_1_layer, 1,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_1_chain,
  NULL, &pool_2_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_0_weights, &conv2d_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_0_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_0_layer, 0,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_0_chain,
  NULL, &conv2d_1_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 2, 2), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 629496, 1, 1),
    629496, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 41176, 1, 1),
    41176, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_CAMERATRAP_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_CAMERATRAP_OUT_NUM, &nl_25_output),
  &conv2d_0_layer, 0x5bee3995, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 629496, 1, 1),
      629496, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 41176, 1, 1),
      41176, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_CAMERATRAP_IN_NUM, &input_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_CAMERATRAP_OUT_NUM, &nl_25_output),
  &conv2d_0_layer, 0x5bee3995, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool cameratrap_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_cameratrap_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 20912);
    input_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 20912);
    conv2d_0_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 40112);
    conv2d_0_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 40112);
    conv2d_0_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 1224);
    conv2d_0_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 1224);
    conv2d_1_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 39624);
    conv2d_1_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 39624);
    conv2d_1_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 248);
    conv2d_1_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 248);
    pool_2_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 38648);
    pool_2_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 38648);
    gemm_3_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 38656);
    gemm_3_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 38656);
    gemm_3_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 38672);
    gemm_3_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 38672);
    gemm_4_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 38648);
    gemm_4_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 38648);
    gemm_4_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 38652);
    gemm_4_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 38652);
    nl_5_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 38660);
    nl_5_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 38660);
    eltwise_6_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 248);
    eltwise_6_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 248);
    conv2d_7_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 38648);
    conv2d_7_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 38648);
    conv2d_7_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    conv2d_7_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    conv2d_8_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 9600);
    conv2d_8_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 9600);
    conv2d_8_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 11152);
    conv2d_8_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 11152);
    pool_9_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    pool_9_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_10_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 8);
    gemm_10_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 8);
    gemm_10_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 24);
    gemm_10_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 24);
    nl_10_nl_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    nl_10_nl_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_11_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 4);
    gemm_11_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 4);
    gemm_11_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 8);
    gemm_11_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 8);
    nl_12_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    nl_12_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    eltwise_13_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 8);
    eltwise_13_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 8);
    conv2d_14_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 9608);
    conv2d_14_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 9608);
    conv2d_14_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 12424);
    conv2d_14_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 12424);
    conv2d_15_pad_before_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    conv2d_15_pad_before_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    conv2d_15_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 5984);
    conv2d_15_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 5984);
    conv2d_15_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 11392);
    conv2d_15_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 11392);
    pool_16_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    pool_16_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_17_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 16);
    gemm_17_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 16);
    gemm_17_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 48);
    gemm_17_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 48);
    gemm_18_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_18_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_18_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 8);
    gemm_18_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 8);
    nl_19_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 24);
    nl_19_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 24);
    eltwise_20_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 40);
    eltwise_20_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 40);
    gemm_22_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 4840);
    gemm_22_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 4840);
    gemm_22_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 14440);
    gemm_22_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 14440);
    gemm_23_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_23_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_23_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 256);
    gemm_23_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 256);
    gemm_24_scratch0_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_24_scratch0_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    gemm_24_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 128);
    gemm_24_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 128);
    nl_25_output_array.data = AI_PTR(g_cameratrap_activations_map[0] + 0);
    nl_25_output_array.data_start = AI_PTR(g_cameratrap_activations_map[0] + 0);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool cameratrap_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_cameratrap_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    conv2d_0_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_0_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 0);
    conv2d_0_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 0);
    conv2d_0_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_0_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 72);
    conv2d_0_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 72);
    conv2d_1_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 104);
    conv2d_1_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 104);
    conv2d_1_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 680);
    conv2d_1_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 680);
    gemm_3_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_3_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 712);
    gemm_3_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 712);
    gemm_3_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_3_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 728);
    gemm_3_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 728);
    gemm_4_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_4_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 736);
    gemm_4_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 736);
    gemm_4_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_4_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 752);
    gemm_4_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 752);
    conv2d_7_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_7_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 784);
    conv2d_7_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 784);
    conv2d_7_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_7_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 1360);
    conv2d_7_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 1360);
    conv2d_8_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_8_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 1392);
    conv2d_8_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 1392);
    conv2d_8_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_8_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 1968);
    conv2d_8_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 1968);
    gemm_10_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_10_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 2000);
    gemm_10_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 2000);
    gemm_10_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_10_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 2016);
    gemm_10_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 2016);
    gemm_11_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_11_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 2024);
    gemm_11_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 2024);
    gemm_11_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_11_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 2040);
    gemm_11_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 2040);
    conv2d_14_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_14_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 2072);
    conv2d_14_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 2072);
    conv2d_14_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_14_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 3224);
    conv2d_14_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 3224);
    conv2d_15_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_15_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 3288);
    conv2d_15_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 3288);
    conv2d_15_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_15_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 5592);
    conv2d_15_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 5592);
    gemm_17_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_17_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 5656);
    gemm_17_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 5656);
    gemm_17_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_17_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 5720);
    gemm_17_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 5720);
    gemm_18_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_18_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 5736);
    gemm_18_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 5736);
    gemm_18_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_18_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 5800);
    gemm_18_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 5800);
    gemm_22_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_22_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 5864);
    gemm_22_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 5864);
    gemm_22_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_22_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 620264);
    gemm_22_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 620264);
    gemm_23_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_23_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 620776);
    gemm_23_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 620776);
    gemm_23_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_23_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 628968);
    gemm_23_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 628968);
    gemm_24_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_24_weights_array.data = AI_PTR(g_cameratrap_weights_map[0] + 629224);
    gemm_24_weights_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 629224);
    gemm_24_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_24_bias_array.data = AI_PTR(g_cameratrap_weights_map[0] + 629480);
    gemm_24_bias_array.data_start = AI_PTR(g_cameratrap_weights_map[0] + 629480);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_cameratrap_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_CAMERATRAP_MODEL_NAME,
      .model_signature   = AI_CAMERATRAP_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 6258578,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x5bee3995,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_cameratrap_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_CAMERATRAP_MODEL_NAME,
      .model_signature   = AI_CAMERATRAP_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 6258578,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x5bee3995,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_cameratrap_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_cameratrap_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_cameratrap_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_cameratrap_create(network, AI_CAMERATRAP_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_cameratrap_data_params_get(&params) != true) {
    err = ai_cameratrap_get_error(*network);
    return err;
  }
#if defined(AI_CAMERATRAP_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_CAMERATRAP_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_cameratrap_init(*network, &params) != true) {
    err = ai_cameratrap_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_cameratrap_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_cameratrap_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_cameratrap_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_cameratrap_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= cameratrap_configure_weights(net_ctx, params);
  ok &= cameratrap_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_cameratrap_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_cameratrap_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_CAMERATRAP_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

