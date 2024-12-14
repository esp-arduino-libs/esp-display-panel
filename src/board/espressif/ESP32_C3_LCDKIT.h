/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure general panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Panel resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH               (240)
#define ESP_PANEL_BOARD_HEIGHT              (240)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using a LCD panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_LCD     (1)     // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_LCD
/**
 * LCD controller name.
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER      GC9A01

/**
 * LCD bus type.
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE        (ESP_PANEL_BUS_TYPE_SPI)

/**
 * LCD bus parameters.
 *
 * There are different parameters for different bus types. Please only configure the parameters for the selected bus type.
 * For the parameters of other bus types, they will be ignored. To understand the parameters, please check
 * https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32s3/api-reference/peripherals/lcd/index.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 *
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

    /**
     * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
     *
     * For drivers which created by this library, even if they use the same host, the host will be initialized only once.
     * So it is not necessary to set the macro to `1`. For other devices, please set the macro to `1` ensure that the
     * host is initialized only once.
     *
     */
    #define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST  (0)     // 0/1. Typically set to 0
    /* For general */
    #define ESP_PANEL_BOARD_LCD_BUS_HOST_ID         (1)     // Typically set to 1
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For host */
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK          (1)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MOSI         (0)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MISO         (-1)    // -1 if not used
#endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    /* For panel */
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS           (7)     // -1 if not used
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DC           (2)
    #define ESP_PANEL_BOARD_LCD_SPI_MODE            (0)     // 0/1/2/3. Typically set to 0
    #define ESP_PANEL_BOARD_LCD_SPI_CLK_HZ          (40 * 1000 * 1000)
                                                            // Should be an integer divisor of 80M, typically set to 40M
    #define ESP_PANEL_BOARD_LCD_SPI_CMD_BITS        (8)     // Typically set to 8
    #define ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS      (8)     // Typically set to 8

#endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */

/**
 * LCD vendor initialization commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                 ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
/*
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD()                       \
    {                                                               \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},   \
        {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},                     \
        {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},                     \
        {0x29, (uint8_t []){0x00}, 0, 120},                         \
        or
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),                   \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),                   \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),                               \
    }
*/

/* LCD device color */
/* Color depth in bits */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS      (ESP_PANEL_LCD_COLOR_BITS_RGB565)
                                                    // ESP_PANEL_LCD_COLOR_BITS_RGB565/RGB666/RGB888
/* Color RGB element order */
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER (1)     // 0: RGB, 1: BGR

/* LCD pre-process flags */
#define ESP_PANEL_BOARD_LCD_INEVRT_COLOR    (1)     // 0/1
#define ESP_PANEL_BOARD_LCD_SWAP_XY         (0)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X        (1)     // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y        (0)     // 0/1

/* LCD other settings */
/* Reset pin */
#define ESP_PANEL_BOARD_LCD_RST_IO          (-1)    // Reset IO pin num. Set to -1 if not use
#define ESP_PANEL_BOARD_LCD_RST_LEVEL       (0)     // Reset active level. 0: low level, 1: high level

#endif // ESP_PANEL_BOARD_DEFAULT_USE_LCD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using a touch panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH   (0)     // 0/1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using the backlight */
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT       (1)     // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
/**
 * Backlight control mode. Choose one of the following:
 *      - ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO: Use GPIO switch to control the backlight, only support on/off
 *      - ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC:    Use LEDC PWM to control the backlight, support brightness adjustment
 *      - ESP_PANEL_BACKLIGHT_TYPE_CUSTOM:      Use custom function to control the backlight
 *
 */
#define ESP_PANEL_BOARD_BACKLIGHT_TYPE          (ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

#if (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO) || \
    (ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

    /* Output pin */
    #define ESP_PANEL_BOARD_BACKLIGHT_IO        (5)     // Output IO pin num
    #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL  (1)     // Light up level. 0: low level, 1: high level

#endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE

/* Set to 1 if want to turn off the backlight after initializing. Otherwise, the backlight will be on */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF      (0)     // 0/1

#endif // ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using an IO Expander */
#define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER    (0)     // 0/1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Please utilize the following macros to execute any additional code if required /////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions, they are used to check if the configurations in this file are compatible with
 * the current version of `esp_panel_board_custom.h` in the library. The detailed rules are as follows:
 *
 *   1. If the major version is not consistent, then the configurations in this file are incompatible with the library
 *      and must be replaced with the file from the library.
 *   2. If the minor version is not consistent, this file might be missing some new configurations, which will be set to
 *      default values. It is recommended to replace it with the file from the library.
 *   3. Even if the patch version is not consistent, it will not affect normal functionality.
 *
 */
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

// *INDENT-OFF*
