/**
  ******************************************************************************
  * @file    rain_detector_model_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    Sun Jun 14 01:37:47 2026
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef RAIN_DETECTOR_MODEL_DATA_PARAMS_H
#define RAIN_DETECTOR_MODEL_DATA_PARAMS_H
#pragma once

#include "ai_platform.h"

/*
#define AI_RAIN_DETECTOR_MODEL_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_rain_detector_model_data_weights_params[1]))
*/

#define AI_RAIN_DETECTOR_MODEL_DATA_CONFIG               (NULL)


#define AI_RAIN_DETECTOR_MODEL_DATA_ACTIVATIONS_SIZES \
  { 35308, }
#define AI_RAIN_DETECTOR_MODEL_DATA_ACTIVATIONS_SIZE     (35308)
#define AI_RAIN_DETECTOR_MODEL_DATA_ACTIVATIONS_COUNT    (1)
#define AI_RAIN_DETECTOR_MODEL_DATA_ACTIVATION_1_SIZE    (35308)



#define AI_RAIN_DETECTOR_MODEL_DATA_WEIGHTS_SIZES \
  { 2176, }
#define AI_RAIN_DETECTOR_MODEL_DATA_WEIGHTS_SIZE         (2176)
#define AI_RAIN_DETECTOR_MODEL_DATA_WEIGHTS_COUNT        (1)
#define AI_RAIN_DETECTOR_MODEL_DATA_WEIGHT_1_SIZE        (2176)



#define AI_RAIN_DETECTOR_MODEL_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_rain_detector_model_activations_table[1])

extern ai_handle g_rain_detector_model_activations_table[1 + 2];



#define AI_RAIN_DETECTOR_MODEL_DATA_WEIGHTS_TABLE_GET() \
  (&g_rain_detector_model_weights_table[1])

extern ai_handle g_rain_detector_model_weights_table[1 + 2];


#endif    /* RAIN_DETECTOR_MODEL_DATA_PARAMS_H */
