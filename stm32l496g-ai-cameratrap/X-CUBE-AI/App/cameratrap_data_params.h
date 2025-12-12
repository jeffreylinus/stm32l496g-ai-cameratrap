/**
  ******************************************************************************
  * @file    cameratrap_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-12-11T20:34:46-0500
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef CAMERATRAP_DATA_PARAMS_H
#define CAMERATRAP_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_CAMERATRAP_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_cameratrap_data_weights_params[1]))
*/

#define AI_CAMERATRAP_DATA_CONFIG               (NULL)


#define AI_CAMERATRAP_DATA_ACTIVATIONS_SIZES \
  { 41176, }
#define AI_CAMERATRAP_DATA_ACTIVATIONS_SIZE     (41176)
#define AI_CAMERATRAP_DATA_ACTIVATIONS_COUNT    (1)
#define AI_CAMERATRAP_DATA_ACTIVATION_1_SIZE    (41176)



#define AI_CAMERATRAP_DATA_WEIGHTS_SIZES \
  { 629496, }
#define AI_CAMERATRAP_DATA_WEIGHTS_SIZE         (629496)
#define AI_CAMERATRAP_DATA_WEIGHTS_COUNT        (1)
#define AI_CAMERATRAP_DATA_WEIGHT_1_SIZE        (629496)



#define AI_CAMERATRAP_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_cameratrap_activations_table[1])

extern ai_handle g_cameratrap_activations_table[1 + 2];



#define AI_CAMERATRAP_DATA_WEIGHTS_TABLE_GET() \
  (&g_cameratrap_weights_table[1])

extern ai_handle g_cameratrap_weights_table[1 + 2];


#endif    /* CAMERATRAP_DATA_PARAMS_H */
