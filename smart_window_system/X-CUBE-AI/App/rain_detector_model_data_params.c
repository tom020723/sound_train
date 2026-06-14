/**
  ******************************************************************************
  * @file    rain_detector_model_data_params.c
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

#include "rain_detector_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_rain_detector_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_rain_detector_model_weights_array_u64[272] = {
  0xc33063be02b7f95U, 0x10f13592ee0f815dU, 0x5681531c9f0082e1U, 0x8c85e5a787feb2d8U,
  0xf6129bac818121dfU, 0x247210c2de0e1a81U, 0xcaee4b6d12e57f38U, 0x9dbca4c8e081ecb5U,
  0x68d68c02d0816dfdU, 0xcf7f19eb352e0037U, 0xcff310810adab64cU, 0x9c4cb28128cfe419U,
  0x3b25fb12ffd412e1U, 0x4cee137fcd7f2ecfU, 0x389bde02083f15dfU, 0x7f8a2bd35d658169U,
  0x2507b590e6160aafU, 0x99275e81f357ca2cU, 0x410000005fU, 0xd10000008dU,
  0xfffffe2100000093U, 0xffffffa700000085U, 0xfffffefbffffffa0U, 0xfffffff000000098U,
  0x1800000081U, 0xffffff7200000031U, 0x8640a4a74dd7f50U, 0x6142da83e9dc4f40U,
  0x67567f7fd97f9f4aU, 0x7fac2f0930ac3ce0U, 0x21d774057f9b2b02U, 0xfdc5407f047f9fdaU,
  0x6e4de525e6b9d690U, 0xfc3b2cba1e970d3fU, 0x7e343059a1524276U, 0x624fedf750cbc27fU,
  0xd0f272342d83fde0U, 0x1d0a42fb7f3be4ffU, 0x7f7f10f7c9e882e0U, 0x327f09d4cb0a9260U,
  0x29d56723caa6177fU, 0x6521233a713a7f0fU, 0x9c5e25b366014a7U, 0x2f837ffd690221b8U,
  0x300000004bU, 0x840000024bU, 0xfffffd7e00000075U, 0xffffffd30000004eU,
  0x1320000009aU, 0x12000000efU, 0xfffffffe000000d5U, 0xfffffeeafffffe60U,
  0x55f10e8113c177c5U, 0xce4674e3b6ecebe3U, 0xfc3444b316814e0aU, 0x782c40e8c3c4cca1U,
  0x3b814df74beffab3U, 0x3fd82cb0091ab0a8U, 0x2bd9ac77c39601dU, 0xb64523b570427f78U,
  0xc94d81fd6d342fe0U, 0xf5f020e5113d68b6U, 0xc3a8bf988c1d2091U, 0x3afb3881d3fab641U,
  0x200abef8afcb7f2eU, 0xf8360808d2d8c5fbU, 0x15ca1b0c0cd3be64U, 0x7f2da3dc585b298bU,
  0x4681979db640e9c4U, 0xeb3537f7d31758c1U, 0x49588d81845cf70dU, 0xf49f0c50a56ba72U,
  0x415936f44e6781d8U, 0x13dfca4630deed4eU, 0x320f04b093a57fe0U, 0xfe48e6ceaa6b32dfU,
  0xe0c6fad78d1e9e9aU, 0x934c9b8b19d1a881U, 0x4ae2172fb2b8815aU, 0xee5c4e9e59b7d36U,
  0xecb1f3f529cbe8eaU, 0x7f64390335e43dc7U, 0x1624ae5515b37c9dU, 0x5acc7ff1fe07b404U,
  0xd20dc8a1bdec24b8U, 0x677fbbb91c5ced3fU, 0xacc26dff38106a9U, 0x56594d41a6f104baU,
  0xaaf0933b34447a11U, 0xd38718142377a55U, 0xde81f60e695a7165U, 0xd05826432d6c1010U,
  0xa9d8afdbe23e3d9eU, 0x81c5eb09c34915e9U, 0x2e1a036afc150ae0U, 0xb4cee9b9e97f0241U,
  0xd054f3057f604bc4U, 0x32d5e5f941f72d82U, 0x1b1781efd01d4236U, 0xc8efd6ffdf1a4a48U,
  0xffffff30fffffffcU, 0x2efffffff7U, 0xffffff7000000033U, 0xffffffc6ffffffddU,
  0xffffffc10000004aU, 0xffffffa900000029U, 0x16f00000011U, 0x5d000000d3U,
  0xffffff44ffffff0aU, 0x3800000083U, 0xffffffe000000094U, 0xffffff80ffffffd1U,
  0x1878fb2e6c8d81dfU, 0x337481344cdf9540U, 0x7fd6ab6bf53f110cU, 0xb7297f60f7812c6eU,
  0x39bec1c7ed627f49U, 0xfc81ec7fe72e81a7U, 0x8f73716339a39c7fU, 0x4efd3b49ed7e9757U,
  0xf8132a077f4b8e53U, 0x87b52326ac5c66aU, 0xdf7f8e933c649126U, 0xe51ff826201c0713U,
  0xc17f30377f93ed0cU, 0xf12c7ee13e6ff68U, 0x3897a56f004b9792U, 0x81ec76314fc89943U,
  0x7f1e3cbeef1c8c03U, 0x1c437f405563d07fU, 0x24373ff5489a9426U, 0x926c9281e667487fU,
  0x9b7f6a2459d41222U, 0xa60b7f363988250eU, 0xf726ba931df84ffcU, 0xfcd7a5724457a891U,
  0x8b3a707f41a3c063U, 0x72d41a877f7fed7aU, 0x1a42333f597f9973U, 0x19affffffd5U,
  0x37000001fbU, 0xfffffff800000041U, 0x15dffffffbfU, 0xffffff4e00000009U,
  0xffffff44000000baU, 0xd9ffffffeeU, 0x670000002fU, 0x16affffff29U,
  0x9f00000081U, 0x3200000063U, 0xffffff79ffffffd8U, 0xd334a5033e89ebdeU,
  0x410edae7fb22dc61U, 0x385c71e2502bc581U, 0x4225bb84d4bc9f09U, 0x85d4d0bf308e275fU,
  0xff8121f022a63161U, 0xde2bc8361381ca23U, 0xec0ed1db21f31141U, 0x30167c1b483099cdU,
  0xd2ce0e02cb8ee9ffU, 0x5a20e303dd133a6aU, 0x3c7928c8234bed81U, 0x4a142804fa597f31U,
  0xeccd403ce353e2d1U, 0x1bd7c41fe6060b3dU, 0x9721f2444dd09730U, 0xc0ee9e123dd2e7fU,
  0x33785bea27f209c1U, 0xa61fe8403697d422U, 0x201dd0cdefdce357U, 0x176f7fd7eef700cdU,
  0x6227eefdb87f20e3U, 0xe11173e4e125d0a4U, 0x2ef1b5351b18311aU, 0xbe24ffee3396bf1bU,
  0x3f07f222122bf05eU, 0xf0613ed5fc12e881U, 0x7f1bf230274636e7U, 0xbe321e01cbd6e3faU,
  0xe3c6a53808fd5970U, 0x81f1c4240282badfU, 0x421aaec43adb405eU, 0x587a57103135a59aU,
  0xb9efac1e3f90c204U, 0x320f8212e0cced29U, 0x48257f230d39008aU, 0x55e3ccc8a67f6b9bU,
  0xdec42f9a12bf13c6U, 0xcfa8f9298da36e5U, 0xdb05d344ffda8548U, 0x3827a80e49dc3013U,
  0x564f79fa493b9181U, 0xaef8c728f381effeU, 0xf27c3193bef1665U, 0x385516e9ee3ae9a7U,
  0xd13e060a12a89430U, 0x52fe813b3838f918U, 0x465428c02e20e186U, 0xa63aab4b3a9ad439U,
  0x442681d345cbf444U, 0x3346461040fba1cfU, 0xea37ea4ef1818948U, 0x4b329105f215f811U,
  0x2c3d26d8040eb7b3U, 0x582f0efef77f23fbU, 0xf63dcbe2dd2e18f8U, 0x1309f8f0d7d74af0U,
  0x391335deed457fd7U, 0x8ad60f90c29ffa4U, 0xe4e9c43d06563a5aU, 0x81fee6e0e211c91bU,
  0x6f429432fc491325U, 0xee1821152ff8c192U, 0x8fe35588da1f10acU, 0x89f524002d8133d1U,
  0xe0275f17daf49de0U, 0x547e30c56881cc3cU, 0x3fb2f33a7f9ff779U, 0x7a2c3ed441d4f18aU,
  0xbefbab00ff81f33eU, 0x140fd4df2be9165fU, 0xb7253ef2530d3c8U, 0x52f3230dd8cac439U,
  0x4f3b52d73fc3a02bU, 0x23acb0d5d3d3ea7fU, 0x1502632d294d6cf8U, 0x12a45230fa2301d8U,
  0xecbfabfdedfc4e7fU, 0xb634db020681b12aU, 0x3127b3ddf7142715U, 0x3f274e09320db5d3U,
  0x3ad639efea3f0f1aU, 0x11ff3ae9073dde0bU, 0xebd9e93e2d0f207fU, 0x42996093f90b7e3U,
  0x1af7c02c4712eb21U, 0x53547f280404e2f0U, 0xbb37c31e2eb2c224U, 0x3041c1ca2be21656U,
  0x87f4502f111b2b8U, 0x51f119da0b5d6f13U, 0xfd177fd7fc0bcabfU, 0xeb9bb34b25ff40fcU,
  0x5e043bf2df4d36c2U, 0xc20076dfba28d2f4U, 0x1581b6ed25413153U, 0x4d0000001aU,
  0xffffffcc0000005cU, 0x18500000164U, 0x13500000141U, 0xa700000105U,
  0x82000001a7U, 0x840000008bU, 0x2c0000010aU, 0xb900000086U,
  0x1860000010eU, 0xffffff5f00000207U, 0x165ffffff78U, 0x186fffffe6bU,
  0x910000017dU, 0x1e0000000f9U, 0xf4000000acU, 0x97621e911d3df047U,
  0x4c7059305361ed63U, 0x4d1f0500cda20b6bU, 0xcb977b59b15181b6U, 0x6fa4a177c9d506d0U,
  0xe7bc90adb5af3ccfU, 0xe8efee9a7f1f8aabU, 0x3a6189ba47b06242U, 0x1d0fffffe4eU,
};


ai_handle g_rain_detector_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_rain_detector_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

