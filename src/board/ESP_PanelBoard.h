/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

// Check if multiple boards are enabled
#if \
    /* Espressif */ \
      defined(BOARD_ESP32_C3_LCDKIT) \
    + defined(BOARD_ESP32_S3_BOX) \
    + defined(BOARD_ESP32_S3_BOX_3) \
    + defined(BOARD_ESP32_S3_BOX_3_BETA) \
    + defined(BOARD_ESP32_S3_BOX_LITE) \
    + defined(BOARD_ESP32_S3_EYE) \
    + defined(BOARD_ESP32_S3_KORVO_2) \
    + defined(BOARD_ESP32_S3_LCD_EV_BOARD) \
    + defined(BOARD_ESP32_S3_LCD_EV_BOARD_V1_5) \
    + defined(BOARD_ESP32_S3_LCD_EV_BOARD_2) \
    + defined(BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5) \
    + defined(BOARD_ESP32_S3_USB_OTG) \
    + defined(BOARD_ESP32_P4_FUNCTION_EV_BOARD) \
    /* Elecrow */ \
    + defined(BOARD_ELECROW_CROWPANEL_7_0) \
    /* M5Stack */ \
    + defined(BOARD_M5STACK_M5CORE2) \
    + defined(BOARD_M5STACK_M5DIAL) \
    + defined(BOARD_M5STACK_M5CORES3) \
    /* JingCai */ \
    + defined(BOARD_ESP32_4848S040C_I_Y_3) \
    /* Waveshare */ \
    + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_1_85) \
    + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_2_1) \
    + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_4_3) \
    + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_4_3_B) \
    + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_5) \
    + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_5_B) \
    + defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_7) \
    + defined(BOARD_WAVESHARE_ESP32_P4_NANO) \
    /* viewe */ \
    + defined(BOARD_UEDX24320028E_WB_A_2_4) \
    + defined(BOARD_UEDX24320028E_WB_A_2_8) \
    + defined(BOARD_UEDX24320028E_WB_A_3_5_240_320) \
    + defined(BOARD_UEDX24320028E_WB_A_3_5_320_480) \
    + defined(BOARD_UEDX48480040E_WB_A_4_0) \
    + defined(BOARD_UEDX80480043E_WB_A_4_3_800_480) \
    + defined(BOARD_UEDX80480043E_WB_A_4_3_480_272) \
    + defined(BOARD_UEDX80480050E_WB_A_5_0) \
    + defined(BOARD_UEDX80480050E_WB_A_5_0_B) \
    + defined(BOARD_UEDX80480070E_WB_A_7_0) \
    > 1
    #error "Multiple boards enabled! Please check file `ESP_Panel_Board_Supported.h` and make sure only one board is enabled."
#endif

// Include board specific header file
/* Espressif */
#if defined(BOARD_ESP32_C3_LCDKIT)
    #include "board/espressif/ESP32_C3_LCDKIT.h"
#elif defined(BOARD_ESP32_S3_BOX)
    #include "board/espressif/ESP32_S3_BOX.h"
#elif defined(BOARD_ESP32_S3_BOX_3)
    #include "board/espressif/ESP32_S3_BOX_3.h"
#elif defined(BOARD_ESP32_S3_BOX_3_BETA)
    #include "board/espressif/ESP32_S3_BOX_3_BETA.h"
#elif defined(BOARD_ESP32_S3_BOX_LITE)
    #include "board/espressif/ESP32_S3_BOX_LITE.h"
#elif defined(BOARD_ESP32_S3_EYE)
    #include "board/espressif/ESP32_S3_EYE.h"
#elif defined(BOARD_ESP32_S3_KORVO_2)
    #include "board/espressif/ESP32_S3_KORVO_2.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD)
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_V1_5)
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD_V1_5.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2)
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD_2.h"
#elif defined(BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5)
    #include "board/espressif/ESP32_S3_LCD_EV_BOARD_2_V1_5.h"
#elif defined(BOARD_ESP32_S3_USB_OTG)
    #include "board/espressif/ESP32_S3_USB_OTG.h"
#elif defined(BOARD_ESP32_P4_FUNCTION_EV_BOARD)
    #include "board/espressif/ESP32_P4_FUNCTION_EV_BOARD.h"
/* Elecrow */
#elif defined(BOARD_ELECROW_CROWPANEL_7_0)
    #include "board/elecrow/CROWPANEL_7_0.h"
/* M5Stack */
#elif defined(BOARD_M5STACK_M5CORE2)
    #include "board/m5stack/M5CORE2.h"
#elif defined(BOARD_M5STACK_M5DIAL)
    #include "board/m5stack/M5DIAL.h"
#elif defined(BOARD_M5STACK_M5CORES3)
    #include "board/m5stack/M5CORES3.h"
/* Jingcai */
#elif defined(BOARD_ESP32_4848S040C_I_Y_3)
    #include "board/jingcai/ESP32_4848S040C_I_Y_3.h"
/* Waveshare */
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_1_85)
    #include "board/waveshare/ESP32_S3_Touch_LCD_1_85.h"
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_2_1)
    #include "board/waveshare/ESP32_S3_Touch_LCD_2_1.h"
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_4_3)
    #include "board/waveshare/ESP32_S3_Touch_LCD_4_3.h"
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_4_3_B)
    #include "board/waveshare/ESP32_S3_Touch_LCD_4_3_B.h"
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_5)
    #include "board/waveshare/ESP32_S3_Touch_LCD_5.h"
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_5_B)
    #include "board/waveshare/ESP32_S3_Touch_LCD_5_B.h"
#elif defined(BOARD_WAVESHARE_ESP32_S3_Touch_LCD_7)
    #include "board/waveshare/ESP32_S3_Touch_LCD_7.h"
#elif defined(BOARD_WAVESHARE_ESP32_P4_NANO)
    #include "board/waveshare/ESP32_P4_NANO.h"
/* viewe */
#elif defined(BOARD_UEDX24320028E_WB_A_2_4)
    #include "board/viewe/viewe_panel_24320024.h"
#elif defined(BOARD_UEDX24320028E_WB_A_2_8)
    #include "board/viewe/viewe_panel_24320028.h"
#elif defined(BOARD_UEDX24320028E_WB_A_3_5_240_320)
    #include "board/viewe/viewe_panel_24320035.h"
#elif defined(BOARD_UEDX24320028E_WB_A_3_5_320_480)
    #include "board/viewe/viewe_panel_32480035.h"
#elif defined(BOARD_UEDX48480040E_WB_A_4_0)
    #include "board/viewe/viewe_panel_4848040.h"
#elif defined(BOARD_UEDX80480043E_WB_A_4_3_800_480)
    #include "board/viewe/viewe_panel_8048043.h"
#elif defined(BOARD_UEDX80480043E_WB_A_4_3_480_272)
    #include "board/viewe/viewe_panel_48272043.h"
#elif defined(BOARD_UEDX80480050E_WB_A_5_0)
    #include "board/viewe/viewe_panel_8048050.h"
#elif defined(BOARD_UEDX80480050E_WB_A_5_0_B)
    #include "board/viewe/viewe_panel2_8048050.h"
#elif defined(BOARD_UEDX80480070E_WB_A_7_0)
    #include "board/viewe/viewe_panel_8048070.h"
#else
    #error "Unknown board selected! Please check file `ESP_Panel_Board_Supported.h` and make sure only one board is enabled."
#endif

// *INDENT-OFF*
