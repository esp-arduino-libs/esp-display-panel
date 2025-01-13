/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an LCD panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_LCD           (1)     // 0/1

#if ESP_PANEL_BOARD_DEFAULT_USE_LCD
/**
 * LCD Controller Name.
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER          ILI9341

/* LCD resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH         (320)
#define ESP_PANEL_BOARD_HEIGHT        (240)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST    (0)     // 0/1
/**
 * LCD Bus Type.
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE      (ESP_PANEL_BUS_TYPE_SPI)
/**
 * LCD Bus Parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

    #define ESP_PANEL_BOARD_LCD_BUS_HOST_ID           (1)     // Typically set to 1
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS             (5)
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK            (7)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MOSI           (6)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MISO           (-1)    // -1 if not used
#endif
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DC             (4)
    #define ESP_PANEL_BOARD_LCD_SPI_MODE              (0)     // 0/1/2/3, typically set to 0
    #define ESP_PANEL_BOARD_LCD_SPI_CLK_HZ            (40 * 1000 * 1000)
                                                        // Should be an integer divisor of 80M, typically set to 40M
    #define ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ    (10)    // Typically set to 10
    #define ESP_PANEL_BOARD_LCD_SPI_CMD_BITS          (8)     // Typically set to 8
    #define ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS        (8)     // Typically set to 8

#endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */

/**
 * LCD Vendor Initialization Commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... })
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() \
    { \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC8, {0xFF, 0x93, 0x42}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x0E, 0x0E}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC5, {0xD0}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x02}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB4, {0x02}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE0, {0x00, 0x03, 0x08, 0x06, 0x13, 0x09, 0x39, 0x39, 0x48, 0x02, 0x0a, 0x08, \
                                               0x17, 0x17, 0x0F}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE1, {0x00, 0x28, 0x29, 0x01, 0x0d, 0x03, 0x3f, 0x33, 0x52, 0x04, 0x0f, 0x0e, \
                                               0x37, 0x38, 0x0F}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB1, {00, 0x1B}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB7, {0x06}), \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(100, 0x11), \
    }

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS    (16)        // 8/16/18/24
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (1)         // 0/1
#define ESP_PANEL_BOARD_LCD_INEVRT_COLOR  (0)         // 0/1

/* LCD Transformation Flags */
#define ESP_PANEL_BOARD_LCD_SWAP_XY       (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X      (1)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y      (1)         // 0/1

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_BOARD_LCD_RST_IO        (48)
#define ESP_PANEL_BOARD_LCD_RST_LEVEL     (1)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_LCD */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an touch panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
/**
 * Touch controller name.
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        GT911


/* Touch Panel Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST      (0)     // 0/1
/**
 * Touch panel bus type.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE            (ESP_PANEL_BUS_TYPE_I2C)
/* Touch panel bus parameters */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID     (0)     // Typically set to 0
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS     (0)     // Typically set to 0 to use default address
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ      (400 * 1000)
                                                    // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP  (0)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP  (0)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL      (18)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA      (8)
#endif

#endif

/* Touch Transformation Flags */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY         (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X        (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y        (0)         // 0/1

/* Touch Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_BOARD_TOUCH_RST_IO          (-1)
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL       (0)         // 0: low level, 1: high level
/* IO num of INT pin, set to -1 if not use */
#define ESP_PANEL_BOARD_TOUCH_INT_IO          (3)
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL       (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
/**
 * Backlight control mode. Choose one of the following:
 *  - ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO: Use GPIO switch to control the backlight, only support on/off
 *  - ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC:    Use LEDC PWM to control the backlight, support brightness adjustment
 *  - ESP_PANEL_BACKLIGHT_TYPE_CUSTOM:      Use custom function to control the backlight
 */
#define ESP_PANEL_BOARD_BACKLIGHT_TYPE        (ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

#if ESP_PANEL_BOARD_BACKLIGHT_TYPE != ESP_PANEL_BACKLIGHT_TYPE_CUSTOM
/* IO num of backlight pin */
#define ESP_PANEL_BOARD_BACKLIGHT_IO          (47)
#define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL    (1)         // 0: low level, 1: high level
#else
/**
 * @brief Custom backlight control function
 *
 * @param[in] percent   Brightness percentage, 0-100
 * @param[in] user_data User data, default is a pointer of `Board`
 */
#define ESP_PANEL_BOARD_BACKLIGHT_CUSTOM_FUNCTION( percent, user_data )  \
    {  \
        Board *board = static_cast<Board *>(user_data);  \
    }
#endif /* ESP_PANEL_BOARD_BACKLIGHT_TYPE */

/* Set to 1 if you want to turn off the backlight after initializing the panel; otherwise, set it to turn on */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF    (0)         // 0: on, 1: off

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER          (0)         // 0/1

// #define ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION( p )

#define ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION( p )  \
    {  \
        /* Maintain the touch INT signal in a low state during the reset process to set its I2C address to `0x5D` */ \
        gpio_set_direction((gpio_num_t)ESP_PANEL_BOARD_TOUCH_INT_IO, GPIO_MODE_OUTPUT); \
        gpio_set_level((gpio_num_t)ESP_PANEL_BOARD_TOUCH_INT_IO, 0); \
        vTaskDelay(pdMS_TO_TICKS(10)); \
        return true; \
    }

// #define ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_POST_BEGIN_FUNCTION( p )

#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

// *INDENT-ON*
