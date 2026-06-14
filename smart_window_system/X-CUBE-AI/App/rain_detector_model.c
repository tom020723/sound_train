/**
  ******************************************************************************
  * @file    rain_detector_model.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Sun Jun  7 22:16:02 2026
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "rain_detector_model.h"
#include "rain_detector_model_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_rain_detector_model
 
#undef AI_RAIN_DETECTOR_MODEL_MODEL_SIGNATURE
#define AI_RAIN_DETECTOR_MODEL_MODEL_SIGNATURE     "b7f4f33ea301afd25e2c0cefc54fc491"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Sun Jun  7 22:16:02 2026"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_RAIN_DETECTOR_MODEL_N_BATCHES
#define AI_RAIN_DETECTOR_MODEL_N_BATCHES         (1)

static ai_ptr g_rain_detector_model_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_rain_detector_model_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_2_scratch0_array, AI_ARRAY_FORMAT_U8,
  NULL, NULL, 160, AI_STATIC)
/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 593, AI_STATIC)
/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 304, AI_STATIC)
/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_input_layer0_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1313, AI_STATIC)
/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_6_scratch0_array, AI_ARRAY_FORMAT_U8,
  NULL, NULL, 240, AI_STATIC)
/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 21008, AI_STATIC)
/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_2_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 21008, AI_STATIC)
/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 889, AI_STATIC)
/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_pad_before_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 24720, AI_STATIC)
/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 21008, AI_STATIC)
/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 31512, AI_STATIC)
/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_9_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 416, AI_STATIC)
/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_6_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 31512, AI_STATIC)
/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  pool_7_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 7200, AI_STATIC)
/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  dense_11_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 42, AI_STATIC)
/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_pad_before_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 9984, AI_STATIC)
/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 7200, AI_STATIC)
/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_9_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 9600, AI_STATIC)
/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  pool_10_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 32, AI_STATIC)
/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  dense_11_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 2, AI_STATIC)
/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  dense_11_0_conversion_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)
/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  nl_12_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)
/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  nl_12_0_conversion_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 2, AI_STATIC)
/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 144, AI_STATIC)
/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)
/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_2_scale_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)
/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_2_bias_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)
/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 144, AI_STATIC)
/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_3_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)
/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 384, AI_STATIC)
/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_4_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 24, AI_STATIC)
/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_6_scale_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 24, AI_STATIC)
/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  eltwise_6_bias_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 24, AI_STATIC)
/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 216, AI_STATIC)
/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_8_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 24, AI_STATIC)
/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_9_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 768, AI_STATIC)
/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_9_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)
/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  dense_11_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 64, AI_STATIC)
/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  dense_11_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 2, AI_STATIC)
/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_0_scratch0_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 484, AI_STATIC)
/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(serving_default_input_layer0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.058926623314619064f),
    AI_PACK_INTQ_ZP(-2)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0743909627199173f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_2_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07635604590177536f),
    AI_PACK_INTQ_ZP(-114)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_3_pad_before_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07635604590177536f),
    AI_PACK_INTQ_ZP(-114)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_3_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.044745348393917084f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_4_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.15009227395057678f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_6_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.15546976029872894f),
    AI_PACK_INTQ_ZP(-118)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_7_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.15546976029872894f),
    AI_PACK_INTQ_ZP(-118)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_8_pad_before_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.15546976029872894f),
    AI_PACK_INTQ_ZP(-118)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_8_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.10024482756853104f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_9_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07959216088056564f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(pool_10_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.019511645659804344f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(dense_11_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.10732726752758026f),
    AI_PACK_INTQ_ZP(-29)))

/* Int quant #13 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_12_0_conversion_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #14 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_0_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0017078311648219824f, 0.0018223993247374892f, 0.0013044989900663495f, 0.0014397669583559036f, 0.0018017699476331472f, 0.0011608923086896539f, 0.0025799735449254513f, 0.0013632430927827954f, 0.0015837948303669691f, 0.001348023652099073f, 0.0015067957574501634f, 0.0016069660196080804f, 0.0010448724497109652f, 0.001836992334574461f, 0.0016782728489488363f, 0.0013992617605254054f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #15 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_2_scale_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.029617859050631523f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #16 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_2_bias_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004129476845264435f),
    AI_PACK_INTQ_ZP(127)))

/* Int quant #17 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_3_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0020279292948544025f, 0.0011869387235492468f, 0.0012178755132481456f, 0.0018823521677404642f, 0.001172746648080647f, 0.0014656877610832453f, 0.0012331069447100163f, 0.0017127608880400658f, 0.0018449954222887754f, 0.0018003745935857296f, 0.0022991017904132605f, 0.002062786603346467f, 0.0023798514157533646f, 0.0019013966666534543f, 0.0019166197162121534f, 0.0018746565328910947f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #18 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_4_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 24,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.004052401520311832f, 0.005226731766015291f, 0.002616356359794736f, 0.003971723839640617f, 0.0033667488023638725f, 0.002745497738942504f, 0.004674883559346199f, 0.0029682000167667866f, 0.004301649052649736f, 0.004003270994871855f, 0.003944577183574438f, 0.0037338605616241693f, 0.004128835164010525f, 0.0038944657426327467f, 0.004615834914147854f, 0.003404550487175584f, 0.003361005801707506f, 0.004767418373376131f, 0.0025054002180695534f, 0.0031577162444591522f, 0.00675285654142499f, 0.0035848268307745457f, 0.0032273747492581606f, 0.005516106262803078f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #19 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_6_scale_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.09452394396066666f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #20 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(eltwise_6_bias_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.006141413934528828f),
    AI_PACK_INTQ_ZP(127)))

/* Int quant #21 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_8_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 24,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.002138435607776046f, 0.003409505123272538f, 0.002501640934497118f, 0.001782515668310225f, 0.003409534227102995f, 0.0016108298441395164f, 0.0038455044850707054f, 0.002760828472673893f, 0.0021199595648795366f, 0.0020862934179604053f, 0.0023127743043005466f, 0.002104327315464616f, 0.00227977498434484f, 0.0026847459375858307f, 0.0029715937562286854f, 0.001234796829521656f, 0.0020537066739052534f, 0.0021302029490470886f, 0.0018773527117446065f, 0.0028940222691744566f, 0.002240972826257348f, 0.0025568821001797915f, 0.0018867499893531203f, 0.0026817405596375465f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #22 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conv2d_9_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 32,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0037234295159578323f, 0.0038293455727398396f, 0.004654835909605026f, 0.0039016304071992636f, 0.004985806532204151f, 0.00394065398722887f, 0.0041078184731304646f, 0.003550174878910184f, 0.0030962456949055195f, 0.0042914459481835365f, 0.004118559416383505f, 0.0033385353162884712f, 0.0037527994718402624f, 0.003953899256885052f, 0.0036652139388024807f, 0.0037982771173119545f, 0.0035719017032533884f, 0.003921350464224815f, 0.0034866826608777046f, 0.003294240217655897f, 0.0051003433763980865f, 0.004383427556604147f, 0.0029793393332511187f, 0.004485363140702248f, 0.0035135881043970585f, 0.004980847239494324f, 0.0036727231927216053f, 0.0030125726480036974f, 0.003459576051682234f, 0.0029246844351291656f, 0.003375416388735175f, 0.003704052185639739f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #23 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(dense_11_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 2,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005654680542647839f, 0.005084347445517778f),
    AI_PACK_INTQ_ZP(0, 0)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_2_scratch0, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 160, 1, 1), AI_STRIDE_INIT(4, 1, 1, 160, 160),
  1, &eltwise_2_scratch0_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_scratch0, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 593, 1, 1), AI_STRIDE_INIT(4, 1, 1, 593, 593),
  1, &conv2d_3_scratch0_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_scratch0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 304, 1, 1), AI_STRIDE_INIT(4, 1, 1, 304, 304),
  1, &conv2d_4_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_input_layer0_output, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 1, 1, 13, 101), AI_STRIDE_INIT(4, 1, 1, 1, 13),
  1, &serving_default_input_layer0_output_array, &serving_default_input_layer0_output_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_6_scratch0, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 240, 1, 1), AI_STRIDE_INIT(4, 1, 1, 240, 240),
  1, &eltwise_6_scratch0_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_output, AI_STATIC,
  5, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 13, 101), AI_STRIDE_INIT(4, 1, 1, 16, 208),
  1, &conv2d_0_output_array, &conv2d_0_output_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_2_output, AI_STATIC,
  6, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 13, 101), AI_STRIDE_INIT(4, 1, 1, 16, 208),
  1, &eltwise_2_output_array, &eltwise_2_output_array_intq)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_scratch0, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 889, 1, 1), AI_STRIDE_INIT(4, 1, 1, 889, 889),
  1, &conv2d_8_scratch0_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_pad_before_output, AI_STATIC,
  8, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 15, 103), AI_STRIDE_INIT(4, 1, 1, 16, 240),
  1, &conv2d_3_pad_before_output_array, &conv2d_3_pad_before_output_array_intq)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_output, AI_STATIC,
  9, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 13, 101), AI_STRIDE_INIT(4, 1, 1, 16, 208),
  1, &conv2d_3_output_array, &conv2d_3_output_array_intq)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_output, AI_STATIC,
  10, 0x1,
  AI_SHAPE_INIT(4, 1, 24, 13, 101), AI_STRIDE_INIT(4, 1, 1, 24, 312),
  1, &conv2d_4_output_array, &conv2d_4_output_array_intq)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_9_scratch0, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 416, 1, 1), AI_STRIDE_INIT(4, 1, 1, 416, 416),
  1, &conv2d_9_scratch0_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_6_output, AI_STATIC,
  12, 0x1,
  AI_SHAPE_INIT(4, 1, 24, 13, 101), AI_STRIDE_INIT(4, 1, 1, 24, 312),
  1, &eltwise_6_output_array, &eltwise_6_output_array_intq)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  pool_7_output, AI_STATIC,
  13, 0x1,
  AI_SHAPE_INIT(4, 1, 24, 6, 50), AI_STRIDE_INIT(4, 1, 1, 24, 144),
  1, &pool_7_output_array, &pool_7_output_array_intq)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  dense_11_scratch0, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 42, 1, 1), AI_STRIDE_INIT(4, 2, 2, 84, 84),
  1, &dense_11_scratch0_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_pad_before_output, AI_STATIC,
  15, 0x1,
  AI_SHAPE_INIT(4, 1, 24, 8, 52), AI_STRIDE_INIT(4, 1, 1, 24, 192),
  1, &conv2d_8_pad_before_output_array, &conv2d_8_pad_before_output_array_intq)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_output, AI_STATIC,
  16, 0x1,
  AI_SHAPE_INIT(4, 1, 24, 6, 50), AI_STRIDE_INIT(4, 1, 1, 24, 144),
  1, &conv2d_8_output_array, &conv2d_8_output_array_intq)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_9_output, AI_STATIC,
  17, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 6, 50), AI_STRIDE_INIT(4, 1, 1, 32, 192),
  1, &conv2d_9_output_array, &conv2d_9_output_array_intq)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  pool_10_output, AI_STATIC,
  18, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 1, 1, 32, 32),
  1, &pool_10_output_array, &pool_10_output_array_intq)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  dense_11_output, AI_STATIC,
  19, 0x1,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &dense_11_output_array, &dense_11_output_array_intq)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  dense_11_0_conversion_output, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &dense_11_0_conversion_output_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  nl_12_output, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &nl_12_output_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  nl_12_0_conversion_output, AI_STATIC,
  22, 0x1,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 1, 1, 2, 2),
  1, &nl_12_0_conversion_output_array, &nl_12_0_conversion_output_array_intq)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_weights, AI_STATIC,
  23, 0x1,
  AI_SHAPE_INIT(4, 1, 3, 3, 16), AI_STRIDE_INIT(4, 1, 1, 3, 9),
  1, &conv2d_0_weights_array, &conv2d_0_weights_array_intq)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_bias, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &conv2d_0_bias_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_2_scale, AI_STATIC,
  25, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &eltwise_2_scale_array, &eltwise_2_scale_array_intq)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_2_bias, AI_STATIC,
  26, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &eltwise_2_bias_array, &eltwise_2_bias_array_intq)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_weights, AI_STATIC,
  27, 0x1,
  AI_SHAPE_INIT(4, 16, 3, 3, 1), AI_STRIDE_INIT(4, 1, 1, 16, 48),
  1, &conv2d_3_weights_array, &conv2d_3_weights_array_intq)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_3_bias, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &conv2d_3_bias_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_weights, AI_STATIC,
  29, 0x1,
  AI_SHAPE_INIT(4, 16, 1, 1, 24), AI_STRIDE_INIT(4, 1, 16, 16, 16),
  1, &conv2d_4_weights_array, &conv2d_4_weights_array_intq)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_4_bias, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &conv2d_4_bias_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_6_scale, AI_STATIC,
  31, 0x1,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 1, 1, 24, 24),
  1, &eltwise_6_scale_array, &eltwise_6_scale_array_intq)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  eltwise_6_bias, AI_STATIC,
  32, 0x1,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 1, 1, 24, 24),
  1, &eltwise_6_bias_array, &eltwise_6_bias_array_intq)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_weights, AI_STATIC,
  33, 0x1,
  AI_SHAPE_INIT(4, 24, 3, 3, 1), AI_STRIDE_INIT(4, 1, 1, 24, 72),
  1, &conv2d_8_weights_array, &conv2d_8_weights_array_intq)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_8_bias, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 1, 24, 1, 1), AI_STRIDE_INIT(4, 4, 4, 96, 96),
  1, &conv2d_8_bias_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_9_weights, AI_STATIC,
  35, 0x1,
  AI_SHAPE_INIT(4, 24, 1, 1, 32), AI_STRIDE_INIT(4, 1, 24, 24, 24),
  1, &conv2d_9_weights_array, &conv2d_9_weights_array_intq)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_9_bias, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &conv2d_9_bias_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  dense_11_weights, AI_STATIC,
  37, 0x1,
  AI_SHAPE_INIT(4, 32, 2, 1, 1), AI_STRIDE_INIT(4, 1, 32, 64, 64),
  1, &dense_11_weights_array, &dense_11_weights_array_intq)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  dense_11_bias, AI_STATIC,
  38, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &dense_11_bias_array, NULL)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_0_scratch0, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 484, 1, 1), AI_STRIDE_INIT(4, 1, 1, 484, 484),
  1, &conv2d_0_scratch0_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_12_0_conversion_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_12_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_12_0_conversion_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_12_0_conversion_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &nl_12_0_conversion_chain,
  NULL, &nl_12_0_conversion_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_12_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_11_0_conversion_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_12_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  nl_12_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_sm,
  &nl_12_chain,
  NULL, &nl_12_0_conversion_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_11_0_conversion_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_11_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_11_0_conversion_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_11_0_conversion_layer, 11,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &dense_11_0_conversion_chain,
  NULL, &nl_12_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_11_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_10_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_11_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_11_weights, &dense_11_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_11_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  dense_11_layer, 11,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &dense_11_chain,
  NULL, &dense_11_0_conversion_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_10_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_9_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_10_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_10_layer, 10,
  POOL_TYPE, 0x0, NULL,
  pool, forward_ap_integer_INT8,
  &pool_10_chain,
  NULL, &dense_11_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(6, 50), 
  .pool_stride = AI_SHAPE_2D_INIT(6, 50), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_9_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_9_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_9_weights, &conv2d_9_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_9_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_9_layer, 9,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_pw_sssa8_ch,
  &conv2d_9_chain,
  NULL, &pool_10_layer, AI_STATIC, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_8_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_pad_before_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_8_weights, &conv2d_8_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_8_layer, 8,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_3x3_sssa8_ch,
  &conv2d_8_chain,
  NULL, &conv2d_9_layer, AI_STATIC, 
  .groups = 24, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)


AI_STATIC_CONST ai_i8 conv2d_8_pad_before_value_data[] = { -118 };
AI_ARRAY_OBJ_DECLARE(
    conv2d_8_pad_before_value, AI_ARRAY_FORMAT_S8,
    conv2d_8_pad_before_value_data, conv2d_8_pad_before_value_data, 1, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_8_pad_before_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_7_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_8_pad_before_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_8_pad_before_layer, 8,
  PAD_TYPE, 0x0, NULL,
  pad, forward_pad,
  &conv2d_8_pad_before_chain,
  NULL, &conv2d_8_layer, AI_STATIC, 
  .value = &conv2d_8_pad_before_value, 
  .mode = AI_PAD_CONSTANT, 
  .pads = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  pool_7_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_6_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &pool_7_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  pool_7_layer, 7,
  POOL_TYPE, 0x0, NULL,
  pool, forward_mp_integer_INT8,
  &pool_7_chain,
  NULL, &conv2d_8_pad_before_layer, AI_STATIC, 
  .pool_size = AI_SHAPE_2D_INIT(2, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(2, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_6_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_6_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &eltwise_6_scale, &eltwise_6_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_6_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  eltwise_6_layer, 6,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn_integer,
  &eltwise_6_chain,
  NULL, &pool_7_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_4_weights, &conv2d_4_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_4_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_4_layer, 4,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_pw_sssa8_ch,
  &conv2d_4_chain,
  NULL, &eltwise_6_layer, AI_STATIC, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_3_pad_before_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_3_weights, &conv2d_3_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_3_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_3_layer, 3,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_3x3_sssa8_ch,
  &conv2d_3_chain,
  NULL, &conv2d_4_layer, AI_STATIC, 
  .groups = 16, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
)


AI_STATIC_CONST ai_i8 conv2d_3_pad_before_value_data[] = { -114 };
AI_ARRAY_OBJ_DECLARE(
    conv2d_3_pad_before_value, AI_ARRAY_FORMAT_S8,
    conv2d_3_pad_before_value_data, conv2d_3_pad_before_value_data, 1, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_3_pad_before_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_3_pad_before_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conv2d_3_pad_before_layer, 3,
  PAD_TYPE, 0x0, NULL,
  pad, forward_pad,
  &conv2d_3_pad_before_chain,
  NULL, &conv2d_3_layer, AI_STATIC, 
  .value = &conv2d_3_pad_before_value, 
  .mode = AI_PAD_CONSTANT, 
  .pads = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  eltwise_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &eltwise_2_scale, &eltwise_2_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &eltwise_2_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  eltwise_2_layer, 2,
  BN_TYPE, 0x0, NULL,
  bn, forward_bn_integer,
  &eltwise_2_chain,
  NULL, &conv2d_3_pad_before_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_input_layer0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_0_weights, &conv2d_0_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_0_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_0_layer, 0,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_sssa8_ch,
  &conv2d_0_chain,
  NULL, &eltwise_2_layer, AI_STATIC, 
  .groups = 1, 
  .nl_func = NULL, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2256, 1, 1),
    2256, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 36340, 1, 1),
    36340, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_RAIN_DETECTOR_MODEL_IN_NUM, &serving_default_input_layer0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_RAIN_DETECTOR_MODEL_OUT_NUM, &nl_12_0_conversion_output),
  &conv2d_0_layer, 0, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2256, 1, 1),
      2256, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 36340, 1, 1),
      36340, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_RAIN_DETECTOR_MODEL_IN_NUM, &serving_default_input_layer0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_RAIN_DETECTOR_MODEL_OUT_NUM, &nl_12_0_conversion_output),
  &conv2d_0_layer, 0, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool rain_detector_model_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_rain_detector_model_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    serving_default_input_layer0_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 13420);
    serving_default_input_layer0_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 13420);
    
    conv2d_0_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 12936);
    conv2d_0_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 12936);
    
    conv2d_0_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 14736);
    conv2d_0_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 14736);
    
    eltwise_2_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 35744);
    eltwise_2_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 35744);
    
    eltwise_2_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 14736);
    eltwise_2_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 14736);
    
    conv2d_3_pad_before_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 11024);
    conv2d_3_pad_before_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 11024);
    
    conv2d_3_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 35744);
    conv2d_3_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 35744);
    
    conv2d_3_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 10816);
    conv2d_3_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 10816);
    
    conv2d_4_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 36032);
    conv2d_4_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 36032);
    
    conv2d_4_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    conv2d_4_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    eltwise_6_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 31512);
    eltwise_6_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 31512);
    
    eltwise_6_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    eltwise_6_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    pool_7_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    pool_7_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    conv2d_8_pad_before_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 7200);
    conv2d_8_pad_before_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 7200);
    
    conv2d_8_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    conv2d_8_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    conv2d_8_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 17184);
    conv2d_8_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 17184);
    
    conv2d_9_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    conv2d_9_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    conv2d_9_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 416);
    conv2d_9_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 416);
    
    pool_10_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    pool_10_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    dense_11_scratch0_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 32);
    dense_11_scratch0_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 32);
    
    dense_11_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 116);
    dense_11_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 116);
    
    dense_11_0_conversion_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    dense_11_0_conversion_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    nl_12_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 8);
    nl_12_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 8);
    
    nl_12_0_conversion_output_array.data = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    nl_12_0_conversion_output_array.data_start = AI_PTR(g_rain_detector_model_activations_map[0] + 0);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool rain_detector_model_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_rain_detector_model_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    conv2d_0_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_0_weights_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 0);
    conv2d_0_weights_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 0);
    
    conv2d_0_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_0_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 144);
    conv2d_0_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 144);
    
    eltwise_2_scale_array.format |= AI_FMT_FLAG_CONST;
    eltwise_2_scale_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 208);
    eltwise_2_scale_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 208);
    
    eltwise_2_bias_array.format |= AI_FMT_FLAG_CONST;
    eltwise_2_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 224);
    eltwise_2_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 224);
    
    conv2d_3_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_3_weights_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 240);
    conv2d_3_weights_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 240);
    
    conv2d_3_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_3_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 384);
    conv2d_3_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 384);
    
    conv2d_4_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_4_weights_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 448);
    conv2d_4_weights_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 448);
    
    conv2d_4_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_4_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 832);
    conv2d_4_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 832);
    
    eltwise_6_scale_array.format |= AI_FMT_FLAG_CONST;
    eltwise_6_scale_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 928);
    eltwise_6_scale_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 928);
    
    eltwise_6_bias_array.format |= AI_FMT_FLAG_CONST;
    eltwise_6_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 952);
    eltwise_6_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 952);
    
    conv2d_8_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_8_weights_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 976);
    conv2d_8_weights_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 976);
    
    conv2d_8_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_8_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 1192);
    conv2d_8_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 1192);
    
    conv2d_9_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_9_weights_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 1288);
    conv2d_9_weights_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 1288);
    
    conv2d_9_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_9_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 2056);
    conv2d_9_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 2056);
    
    dense_11_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_11_weights_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 2184);
    dense_11_weights_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 2184);
    
    dense_11_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_11_bias_array.data = AI_PTR(g_rain_detector_model_weights_map[0] + 2248);
    dense_11_bias_array.data_start = AI_PTR(g_rain_detector_model_weights_map[0] + 2248);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/


AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_rain_detector_model_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_RAIN_DETECTOR_MODEL_MODEL_NAME,
      .model_signature   = AI_RAIN_DETECTOR_MODEL_MODEL_SIGNATURE,
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
      
      .n_macc            = 1321192,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_bool ai_rain_detector_model_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_RAIN_DETECTOR_MODEL_MODEL_NAME,
      .model_signature   = AI_RAIN_DETECTOR_MODEL_MODEL_SIGNATURE,
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
      
      .n_macc            = 1321192,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}

AI_API_ENTRY
ai_error ai_rain_detector_model_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_rain_detector_model_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_error ai_rain_detector_model_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
    ai_error err;
    ai_network_params params;

    err = ai_rain_detector_model_create(network, AI_RAIN_DETECTOR_MODEL_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE)
        return err;
    if (ai_rain_detector_model_data_params_get(&params) != true) {
        err = ai_rain_detector_model_get_error(*network);
        return err;
    }
#if defined(AI_RAIN_DETECTOR_MODEL_DATA_ACTIVATIONS_COUNT)
    if (activations) {
        /* set the addresses of the activations buffers */
        for (int idx=0;idx<params.map_activations.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
    }
#endif
#if defined(AI_RAIN_DETECTOR_MODEL_DATA_WEIGHTS_COUNT)
    if (weights) {
        /* set the addresses of the weight buffers */
        for (int idx=0;idx<params.map_weights.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
    }
#endif
    if (ai_rain_detector_model_init(*network, &params) != true) {
        err = ai_rain_detector_model_get_error(*network);
    }
    return err;
}

AI_API_ENTRY
ai_buffer* ai_rain_detector_model_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_buffer* ai_rain_detector_model_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_handle ai_rain_detector_model_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_rain_detector_model_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if (!net_ctx) return false;

  ai_bool ok = true;
  ok &= rain_detector_model_configure_weights(net_ctx, params);
  ok &= rain_detector_model_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_rain_detector_model_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_rain_detector_model_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_RAIN_DETECTOR_MODEL_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

