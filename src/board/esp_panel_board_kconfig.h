/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

// *INDENT-OFF*

#ifndef ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
    #ifdef CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
        #define ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
    #else
        #define ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED 0
    #endif
#endif

#ifndef ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    #ifdef CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
        #define ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    #else
        #define ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM 0
    #endif
#endif

/**
 * Supported Board
 *
 */
#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
    // Espressif
    #ifndef BOARD_ESP32_C3_LCDKIT
        #ifdef CONFIG_BOARD_ESP32_C3_LCDKIT
            #define BOARD_ESP32_C3_LCDKIT CONFIG_BOARD_ESP32_C3_LCDKIT
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_BOX
        #ifdef CONFIG_BOARD_ESP32_S3_BOX
            #define BOARD_ESP32_S3_BOX CONFIG_BOARD_ESP32_S3_BOX
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_BOX_3
        #ifdef CONFIG_BOARD_ESP32_S3_BOX_3
            #define BOARD_ESP32_S3_BOX_3 CONFIG_BOARD_ESP32_S3_BOX_3
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_BOX_3_BETA
        #ifdef CONFIG_BOARD_ESP32_S3_BOX_3_BETA
            #define BOARD_ESP32_S3_BOX_3_BETA CONFIG_BOARD_ESP32_S3_BOX_3_BETA
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_BOX_LITE
        #ifdef CONFIG_BOARD_ESP32_S3_BOX_LITE
            #define BOARD_ESP32_S3_BOX_LITE CONFIG_BOARD_ESP32_S3_BOX_LITE
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_EYE
        #ifdef CONFIG_BOARD_ESP32_S3_EYE
            #define BOARD_ESP32_S3_EYE CONFIG_BOARD_ESP32_S3_EYE
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_KORVO_2
        #ifdef CONFIG_BOARD_ESP32_S3_KORVO_2
            #define BOARD_ESP32_S3_KORVO_2 CONFIG_BOARD_ESP32_S3_KORVO_2
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_LCD_EV_BOARD
        #ifdef CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD
            #define BOARD_ESP32_S3_LCD_EV_BOARD CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_LCD_EV_BOARD_V1_5
        #ifdef CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_V1_5
            #define BOARD_ESP32_S3_LCD_EV_BOARD_V1_5 CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_V1_5
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_LCD_EV_BOARD_2
        #ifdef CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_2
            #define BOARD_ESP32_S3_LCD_EV_BOARD_2 CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_2
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5
        #ifdef CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5
            #define BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5 CONFIG_BOARD_ESP32_S3_LCD_EV_BOARD_2_V1_5
        #endif
    #endif
    #ifndef BOARD_ESP32_S3_USB_OTG
        #ifdef CONFIG_BOARD_ESP32_S3_USB_OTG
            #define BOARD_ESP32_S3_USB_OTG CONFIG_BOARD_ESP32_S3_USB_OTG
        #endif
    #endif
    #ifndef BOARD_ESP32_P4_FUNCTION_EV_BOARD
        #ifdef CONFIG_BOARD_ESP32_P4_FUNCTION_EV_BOARD
            #define BOARD_ESP32_P4_FUNCTION_EV_BOARD CONFIG_BOARD_ESP32_P4_FUNCTION_EV_BOARD
        #endif
    #endif
    // Elecrow
    #ifndef BOARD_ELECROW_CROWPANEL_7_0
        #ifdef CONFIG_BOARD_ELECROW_CROWPANEL_7_0
            #define BOARD_ELECROW_CROWPANEL_7_0 CONFIG_BOARD_ELECROW_CROWPANEL_7_0
        #endif
    #endif
    // M5Stack
    #ifndef BOARD_M5STACK_M5CORE2
        #ifdef CONFIG_BOARD_M5STACK_M5CORE2
            #define BOARD_M5STACK_M5CORE2 CONFIG_BOARD_M5STACK_M5CORE2
        #endif
    #endif
    #ifndef BOARD_M5STACK_M5DIAL
        #ifdef CONFIG_BOARD_M5STACK_M5DIAL
            #define BOARD_M5STACK_M5DIAL CONFIG_BOARD_M5STACK_M5DIAL
        #endif
    #endif
    #ifndef BOARD_M5STACK_M5CORES3
        #ifdef CONFIG_BOARD_M5STACK_M5CORES3
            #define BOARD_M5STACK_M5CORES3 CONFIG_BOARD_M5STACK_M5CORES3
        #endif
    #endif
    // Jingcai
    #ifndef BOARD_ESP32_4848S040C_I_Y_3
        #ifdef CONFIG_BOARD_ESP32_4848S040C_I_Y_3
            #define BOARD_ESP32_4848S040C_I_Y_3 CONFIG_BOARD_ESP32_4848S040C_I_Y_3
        #endif
    #endif
    // Waveshare
    #ifndef BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85
            #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85 CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_1_85
        #endif
    #endif
    #ifndef BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1
            #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1 CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_2_1
        #endif
    #endif
    #ifndef BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3
            #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3 CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3
        #endif
    #endif
    #ifndef BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B
            #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3_B
        #endif
    #endif
    #ifndef BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5
            #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5 CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5
        #endif
    #endif
    #ifndef BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B
            #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_5_B
        #endif
    #endif
    #ifndef BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7
            #define BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7 CONFIG_BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_7
        #endif
    #endif
    #ifndef BOARD_WAVESHARE_ESP32_P4_NANO
        #ifdef CONFIG_BOARD_WAVESHARE_ESP32_P4_NANO
            #define BOARD_WAVESHARE_ESP32_P4_NANO CONFIG_BOARD_WAVESHARE_ESP32_P4_NANO
        #endif
    #endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED */

/**
 * Custom Board
 *
 */
#if ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
    // LCD
    #ifndef ESP_PANEL_BOARD_DEFAULT_USE_LCD
        #ifdef CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_LCD
            #define ESP_PANEL_BOARD_DEFAULT_USE_LCD CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_LCD
        #else
            #define ESP_PANEL_BOARD_DEFAULT_USE_LCD 0
        #endif
    #endif
    #if ESP_PANEL_BOARD_DEFAULT_USE_LCD
        // Controller
        #ifndef ESP_PANEL_LCD_CONTROLLER_EK9716B
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_EK9716B
                #define ESP_PANEL_BOARD_LCD_CONTROLLER EK9716B
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_GC9A01
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_GC9A01
                #define ESP_PANEL_BOARD_LCD_CONTROLLER GC9A01
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_GC9B71
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_GC9B71
                #define ESP_PANEL_BOARD_LCD_CONTROLLER GC9B71
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_GC9503
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_GC9503
                #define ESP_PANEL_BOARD_LCD_CONTROLLER GC9503
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_ILI9341
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_ILI9341
                #define ESP_PANEL_BOARD_LCD_CONTROLLER ILI9341
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_NV3022B
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_NV3022B
                #define ESP_PANEL_BOARD_LCD_CONTROLLER NV3022B
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_SH8601
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_SH8601
                #define ESP_PANEL_BOARD_LCD_CONTROLLER SH8601
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_SPD2010
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_SPD2010
                #define ESP_PANEL_BOARD_LCD_CONTROLLER SPD2010
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_ST7262
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_ST7262
                #define ESP_PANEL_BOARD_LCD_CONTROLLER ST7262
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_ST7701
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_ST7701
                #define ESP_PANEL_BOARD_LCD_CONTROLLER ST7701
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_ST7789
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_ST7789
                #define ESP_PANEL_BOARD_LCD_CONTROLLER ST7789
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_ST7796
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_ST7796
                #define ESP_PANEL_BOARD_LCD_CONTROLLER ST7796
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_ST77916
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_ST77916
                #define ESP_PANEL_BOARD_LCD_CONTROLLER ST77916
            #endif
        #endif
        #ifndef ESP_PANEL_LCD_CONTROLLER_ST77922
            #ifdef CONFIG_ESP_PANEL_LCD_CONTROLLER_ST77922
                #define ESP_PANEL_BOARD_LCD_CONTROLLER ST77922
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_CONTROLLER
            #error "Missing configuration: ESP_PANEL_BOARD_LCD_CONTROLLER"
        #endif
        // Resolution
        #ifndef ESP_PANEL_BOARD_WIDTH
            #ifdef CONFIG_ESP_PANEL_BOARD_WIDTH
                #define ESP_PANEL_BOARD_WIDTH CONFIG_ESP_PANEL_BOARD_WIDTH
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_WIDTH"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_HEIGHT
            #ifdef CONFIG_ESP_PANEL_BOARD_HEIGHT
                #define ESP_PANEL_BOARD_HEIGHT CONFIG_ESP_PANEL_BOARD_HEIGHT
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_HEIGHT"
            #endif
        #endif
        // Bus
        #ifndef ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
                #define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST CONFIG_ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            #else
                #define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST 0
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_BUS_TYPE
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_BUS_TYPE
                #define ESP_PANEL_BOARD_LCD_BUS_TYPE CONFIG_ESP_PANEL_BOARD_LCD_BUS_TYPE
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_LCD_BUS_TYPE"
            #endif
        #endif
        // SPI Bus
        #if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
            #ifndef ESP_PANEL_BOARD_LCD_BUS_HOST_ID
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_BUS_HOST_ID
                    #define ESP_PANEL_BOARD_LCD_BUS_HOST_ID CONFIG_ESP_PANEL_BOARD_LCD_BUS_HOST_ID
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_BUS_HOST_ID"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_MODE
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_MODE
                    #define ESP_PANEL_BOARD_LCD_SPI_MODE CONFIG_ESP_PANEL_BOARD_LCD_SPI_MODE
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_MODE"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_CLK_HZ
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_CLK_HZ
                    #define ESP_PANEL_BOARD_LCD_SPI_CLK_HZ CONFIG_ESP_PANEL_BOARD_LCD_SPI_CLK_HZ
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_CLK_HZ"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ
                    #define ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ CONFIG_ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ
                #else
                    #error "Missing configuration: CONFIG_ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_CMD_BITS
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_CMD_BITS
                    #define ESP_PANEL_BOARD_LCD_SPI_CMD_BITS CONFIG_ESP_PANEL_BOARD_LCD_SPI_CMD_BITS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_CMD_BITS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS
                    #define ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS CONFIG_ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_CS
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_CS
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_CS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_CS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_DC
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DC
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_DC CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DC
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_DC"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_SCK"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_MOSI
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_MOSI
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_MOSI CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_MOSI
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_MOSI"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_MISO
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_MISO
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_MISO CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_MISO
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_MISO"
                #endif
            #endif
        // QSPI Bus
        #elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI
            #ifndef ESP_PANEL_BOARD_LCD_BUS_HOST_ID
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_BUS_HOST_ID
                    #define ESP_PANEL_BOARD_LCD_BUS_HOST_ID CONFIG_ESP_PANEL_BOARD_LCD_BUS_HOST_ID
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_BUS_HOST_ID"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_MODE
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_MODE
                    #define ESP_PANEL_BOARD_LCD_SPI_MODE CONFIG_ESP_PANEL_BOARD_LCD_SPI_MODE
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_MODE"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_CLK_HZ
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_CLK_HZ
                    #define ESP_PANEL_BOARD_LCD_SPI_CLK_HZ CONFIG_ESP_PANEL_BOARD_LCD_SPI_CLK_HZ
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_CLK_HZ"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ
                    #define ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ CONFIG_ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ
                #else
                    #error "Missing configuration: CONFIG_ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_CMD_BITS
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_CMD_BITS
                    #define ESP_PANEL_BOARD_LCD_SPI_CMD_BITS CONFIG_ESP_PANEL_BOARD_LCD_SPI_CMD_BITS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_CMD_BITS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS
                    #define ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS CONFIG_ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_CS
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_CS
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_CS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_CS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_SCK"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_DATA0
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA0
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA0 CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA0
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_DATA0"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_DATA1
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA1
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA1 CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA1
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_DATA1"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_DATA2
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA2
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA2 CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA2
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_DATA2"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_DATA3
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA3
                    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA3 CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_DATA3
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_DATA3"
                #endif
            #endif
        // RGB Bus
        #elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
            #if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
                // 3-wire SPI Interface
                #ifndef ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER
                        #define ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER CONFIG_ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER
                    #else
                        #define ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER 0
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER
                        #define ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER CONFIG_ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER
                    #else
                        #define ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER 0
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER
                        #define ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER CONFIG_ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER
                    #else
                        #define ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER 0
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX
                        #define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX CONFIG_ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX
                    #else
                        #define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX 0
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD
                        #define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD CONFIG_ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD
                    #else
                        #define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD (!ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX)
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_CS
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_CS
                        #define ESP_PANEL_BOARD_LCD_SPI_IO_CS CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_CS
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_CS"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                        #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SCK
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_SCK"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_SPI_IO_SDA
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SDA
                        #define ESP_PANEL_BOARD_LCD_SPI_IO_SDA CONFIG_ESP_PANEL_BOARD_LCD_SPI_IO_SDA
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_SPI_IO_SDA"
                    #endif
                #endif
            #endif /* ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST */
            // RGB Interface
            #ifndef ESP_PANEL_BOARD_LCD_RGB_CLK_HZ
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_CLK_HZ
                    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ CONFIG_ESP_PANEL_BOARD_LCD_RGB_CLK_HZ
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_CLK_HZ"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_HPW
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_HPW
                    #define ESP_PANEL_BOARD_LCD_RGB_HPW CONFIG_ESP_PANEL_BOARD_LCD_RGB_HPW
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_HPW"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_HBP
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_HBP
                    #define ESP_PANEL_BOARD_LCD_RGB_HBP CONFIG_ESP_PANEL_BOARD_LCD_RGB_HBP
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_HBP"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_HFP
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_HFP
                    #define ESP_PANEL_BOARD_LCD_RGB_HFP CONFIG_ESP_PANEL_BOARD_LCD_RGB_HFP
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_HFP"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_VPW
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_VPW
                    #define ESP_PANEL_BOARD_LCD_RGB_VPW CONFIG_ESP_PANEL_BOARD_LCD_RGB_VPW
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_VPW"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_VBP
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_VBP
                    #define ESP_PANEL_BOARD_LCD_RGB_VBP CONFIG_ESP_PANEL_BOARD_LCD_RGB_VBP
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_VBP"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_VFP
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_VFP
                    #define ESP_PANEL_BOARD_LCD_RGB_VFP CONFIG_ESP_PANEL_BOARD_LCD_RGB_VFP
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_VFP"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG
                    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG CONFIG_ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG
                #else
                    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG 0
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH
                    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH CONFIG_ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS
                    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS CONFIG_ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DE
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DE
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DE
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DE"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_PCLK
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_PCLK
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_PCLK
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_PCLK"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DISP
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DISP
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DISP
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DISP"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA0
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA0
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA0
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA0"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA1
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA1
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA1
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA1"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA2
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA2
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA2
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA2"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA3
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA3
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA3
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA3"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA4
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA4
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA4
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA4"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA5
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA5
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA5
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA5"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA6
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA6
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA6
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA6"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA7
                #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA7
                    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA7
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA7"
                #endif
            #endif
            #if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA8
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA8
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA8
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA8"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA9
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA9
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA9
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA9"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA10
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA10
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA10
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA10"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA11
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA11
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA11
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA11"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA12
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA12
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA12
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA12"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA13
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA13
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA13
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA13"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA14
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA14
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA14
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA14"
                    #endif
                #endif
                #ifndef ESP_PANEL_BOARD_LCD_RGB_IO_DATA15
                    #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA15
                        #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15 CONFIG_ESP_PANEL_BOARD_LCD_RGB_IO_DATA15
                    #else
                        #error "Missing configuration: ESP_PANEL_BOARD_LCD_RGB_IO_DATA15"
                    #endif
                #endif
            #endif /* ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH */
        #else
            #error "Invalid configuration: ESP_PANEL_BOARD_LCD_BUS_TYPE"
        #endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */
        // Color Settings
        #ifndef ESP_PANEL_BOARD_LCD_COLOR_BITS
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_COLOR_BITS
                #define ESP_PANEL_BOARD_LCD_COLOR_BITS CONFIG_ESP_PANEL_BOARD_LCD_COLOR_BITS
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_LCD_COLOR_BITS"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER
                #define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER CONFIG_ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER
            #else
                #define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER 0
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_INEVRT_COLOR
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_INEVRT_COLOR
                #define ESP_PANEL_BOARD_LCD_INEVRT_COLOR CONFIG_ESP_PANEL_BOARD_LCD_INEVRT_COLOR
            #else
                #define ESP_PANEL_BOARD_LCD_INEVRT_COLOR 0
            #endif
        #endif
        // Transformation settings
        #ifndef ESP_PANEL_BOARD_LCD_SWAP_XY
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_SWAP_XY
                #define ESP_PANEL_BOARD_LCD_SWAP_XY CONFIG_ESP_PANEL_BOARD_LCD_SWAP_XY
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_MIRROR_X
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_MIRROR_X
                #define ESP_PANEL_BOARD_LCD_MIRROR_X CONFIG_ESP_PANEL_BOARD_LCD_MIRROR_X
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_MIRROR_Y
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_MIRROR_Y
                #define ESP_PANEL_BOARD_LCD_MIRROR_Y CONFIG_ESP_PANEL_BOARD_LCD_MIRROR_Y
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_RST_IO
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RST_IO
                #define ESP_PANEL_BOARD_LCD_RST_IO CONFIG_ESP_PANEL_BOARD_LCD_RST_IO
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_LCD_RST_IO"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_LCD_RST_LEVEL
            #ifdef CONFIG_ESP_PANEL_BOARD_LCD_RST_LEVEL
                #define ESP_PANEL_BOARD_LCD_RST_LEVEL CONFIG_ESP_PANEL_BOARD_LCD_RST_LEVEL
            #else
                #define ESP_PANEL_BOARD_LCD_RST_LEVEL 0
            #endif
        #endif
    #endif /* ESP_PANEL_BOARD_DEFAULT_USE_LCD */

    #ifndef ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
        #ifdef CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
            #define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
        #else
            #define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH 0
        #endif
    #endif
    // LCD Touch
    #if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
        // Controller
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_CST816S
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_CST816S
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER CST816S
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_FT5x06
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_FT5x06
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER FT5x06
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_GT911
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_GT911
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER GT911
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_GT1151
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_GT1151
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER GT1151
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_ST1633
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_ST1633
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER ST1633
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_ST7123
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_ST7123
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER ST7123
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_TT21100
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_TT21100
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER TT21100
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_CONTROLLER_STMPE610
            #ifdef CONFIG_ESP_PANEL_TOUCH_CONTROLLER_STMPE610
                #define ESP_PANEL_BOARD_TOUCH_CONTROLLER STMPE610
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_CONTROLLER
            #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_CONTROLLER"
        #endif
        // Resolution
        #ifndef ESP_PANEL_TOUCH_H_RES
            #ifdef CONFIG_ESP_PANEL_TOUCH_H_RES
                #define ESP_PANEL_TOUCH_H_RES CONFIG_ESP_PANEL_TOUCH_H_RES
            #else
                #error "Missing configuration: ESP_PANEL_TOUCH_H_RES"
            #endif
        #endif
        #ifndef ESP_PANEL_TOUCH_V_RES
            #ifdef CONFIG_ESP_PANEL_TOUCH_V_RES
                #define ESP_PANEL_TOUCH_V_RES CONFIG_ESP_PANEL_TOUCH_V_RES
            #else
                #error "Missing configuration: ESP_PANEL_TOUCH_V_RES"
            #endif
        #endif
        // Bus Settings
        #ifndef ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
                #define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
            #else
                #define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST 0
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_BUS_TYPE
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_TYPE
                #define ESP_PANEL_BOARD_TOUCH_BUS_TYPE CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_TYPE
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_BUS_TYPE"
            #endif
        #endif
        #if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
            // I2C Bus
            #ifndef ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID
                    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS
                    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ
                    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP
                    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP
                #else
                    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP 0
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP
                    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP
                #else
                    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP 0
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL
                    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA
                    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA CONFIG_ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA"
                #endif
            #endif
        #elif ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
            #ifndef ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID
                    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID CONFIG_ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_SPI_IO_CS
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_CS
                    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_CS CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_CS
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_SPI_IO_CS"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK
                    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI
                    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI"
                #endif
            #endif
            #ifndef ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO
                #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO
                    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO CONFIG_ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO
                #else
                    #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO"
                #endif
            #endif
        #else
            #error "Invalid configuration: ESP_PANEL_BOARD_TOUCH_BUS_TYPE"
        #endif /* ESP_PANEL_BOARD_TOUCH_BUS_TYPE */
        // Transformation Settings
        #ifndef ESP_PANEL_BOARD_TOUCH_SWAP_XY
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_SWAP_XY
                #define ESP_PANEL_BOARD_TOUCH_SWAP_XY CONFIG_ESP_PANEL_BOARD_TOUCH_SWAP_XY
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_MIRROR_X
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_MIRROR_X
                #define ESP_PANEL_BOARD_TOUCH_MIRROR_X CONFIG_ESP_PANEL_BOARD_TOUCH_MIRROR_X
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_MIRROR_Y
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_MIRROR_Y
                #define ESP_PANEL_BOARD_TOUCH_MIRROR_Y CONFIG_ESP_PANEL_BOARD_TOUCH_MIRROR_Y
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_RST_IO
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_RST_IO
                #define ESP_PANEL_BOARD_TOUCH_RST_IO CONFIG_ESP_PANEL_BOARD_TOUCH_RST_IO
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_RST_IO"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_RST_LEVEL
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_RST_LEVEL
                #define ESP_PANEL_BOARD_TOUCH_RST_LEVEL CONFIG_ESP_PANEL_BOARD_TOUCH_RST_LEVEL
            #else
                #define ESP_PANEL_BOARD_TOUCH_RST_LEVEL 0
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_INT_IO
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_INT_IO
                #define ESP_PANEL_BOARD_TOUCH_INT_IO CONFIG_ESP_PANEL_BOARD_TOUCH_INT_IO
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_TOUCH_INT_IO"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_TOUCH_INT_LEVEL
            #ifdef CONFIG_ESP_PANEL_BOARD_TOUCH_INT_LEVEL
                #define ESP_PANEL_BOARD_TOUCH_INT_LEVEL CONFIG_ESP_PANEL_BOARD_TOUCH_INT_LEVEL
            #else
                #define ESP_PANEL_BOARD_TOUCH_INT_LEVEL 0
            #endif
        #endif
    #endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

    // Backlight
    #ifndef ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
        #ifdef CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
            #define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT CONFIG_ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
        #else
            #define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT 0
        #endif
    #endif
    #if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
        #ifndef ESP_PANEL_BOARD_BACKLIGHT_IO
            #ifdef CONFIG_ESP_PANEL_BACKLIGHT_IO
                #define ESP_PANEL_BOARD_BACKLIGHT_IO CONFIG_ESP_PANEL_BACKLIGHT_IO
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_BACKLIGHT_IO"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL
            #ifdef CONFIG_ESP_PANEL_BACKLIGHT_ON_LEVEL
                #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL CONFIG_ESP_PANEL_BACKLIGHT_ON_LEVEL
            #else
                #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL 1
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF
            #ifdef CONFIG_ESP_PANEL_BACKLIGHT_IDLE_OFF
                #define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF CONFIG_ESP_PANEL_BACKLIGHT_IDLE_OFF
            #else
                #define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF 0
            #endif
        #endif
    #endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */

    // IO Expander
    #ifndef ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
        #ifdef CONFIG_ESP_PANEL_USE_EXPANDER
            #define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER CONFIG_ESP_PANEL_USE_EXPANDER
        #else
            #define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER 0
        #endif
    #endif
    #if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
        // CHIP
        #ifndef ESP_PANEL_EXPANDER_CHIP_CH422G
            #ifdef CONFIG_ESP_PANEL_EXPANDER_CHIP_CH422G
                #define ESP_PANEL_EXPANDER_CHIP_NAME CH422G
            #endif
        #endif
        #ifndef ESP_PANEL_EXPANDER_CHIP_HT8574
            #ifdef CONFIG_ESP_PANEL_EXPANDER_CHIP_HT8574
                #define ESP_PANEL_EXPANDER_CHIP_NAME HT8574
            #endif
        #endif
        #ifndef ESP_PANEL_EXPANDER_CHIP_TCA95xx_8bit
            #ifdef CONFIG_ESP_PANEL_EXPANDER_CHIP_TCA95xx_8bit
                #define ESP_PANEL_EXPANDER_CHIP_NAME TCA95XX_8BIT
            #endif
        #endif
        #ifndef ESP_PANEL_EXPANDER_TYPE_TCA95xx_16bit
            #ifdef CONFIG_ESP_PANEL_EXPANDER_CHIP_TCA95xx_16bit
                #define ESP_PANEL_EXPANDER_CHIP_NAME TCA95XX_16BIT
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
            #ifdef CONFIG_ESP_PANEL_EXPANDER_SKIP_INIT_HOST
                #define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST CONFIG_ESP_PANEL_EXPANDER_SKIP_INIT_HOST
            #else
                #define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST 0
            #endif
        #endif
        // Bus Settings
        #ifndef ESP_PANEL_BOARD_EXPANDER_HOST_ID
            #ifdef CONFIG_ESP_PANEL_BOARD_EXPANDER_HOST_ID
                #define ESP_PANEL_BOARD_EXPANDER_HOST_ID CONFIG_ESP_PANEL_BOARD_EXPANDER_HOST_ID
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_EXPANDER_HOST_ID"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS
            #ifdef CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS
                #define ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ
            #ifdef CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ
                #define ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP
            #ifdef CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP
                #define ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP
            #else
                #define ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP 0
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP
            #ifdef CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP
                #define ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP
            #else
                #define ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP 0
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL
            #ifdef CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL
                #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL"
            #endif
        #endif
        #ifndef ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA
            #ifdef CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA
                #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA CONFIG_ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA
            #else
                #error "Missing configuration: ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA"
            #endif
        #endif
    #endif /* ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER */
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM */

// *INDENT-OFF*
