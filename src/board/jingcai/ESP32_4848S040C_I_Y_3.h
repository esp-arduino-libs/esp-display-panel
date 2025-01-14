/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an LCD panel */
#define ESP_PANEL_BOARD_USE_LCD           (1)     // 0/1

#if ESP_PANEL_BOARD_USE_LCD
/**
 * LCD Controller Name
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER          ST7701

/* LCD resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH         (480)
#define ESP_PANEL_BOARD_HEIGHT        (480)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST    (0)     // 0/1
/**
 * LCD Bus Type.
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE      (ESP_PANEL_BUS_TYPE_RGB)
/**
 * LCD Bus Parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ            (26 * 1000 * 1000)
    #define ESP_PANEL_BOARD_LCD_RGB_HPW               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HBP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HFP               (20)
    #define ESP_PANEL_BOARD_LCD_RGB_VPW               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VBP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VFP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG   (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH        (16)    //  8 | 16
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS        (16)    // 24 | 16

    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_BOARD_WIDTH * 10)
                                                        // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                        // To enable the bounce buffer, set it to a non-zero value.
                                                        // Typically set to `ESP_PANEL_BOARD_WIDTH * 10`
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC          (16)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC          (17)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE             (18)   // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK           (21)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP           (-1)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0          (4)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1          (5)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2          (6)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3          (7)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4          (15)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5          (8)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6          (20)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7          (3)
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8          (46)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9          (9)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10         (10)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11         (11)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12         (12)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13         (13)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14         (14)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15         (0)
#endif
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS               (39)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK              (48)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SDA              (47)
    #define ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER     (0)     // 0/1
    #define ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER    (0)     // 0/1
    #define ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER    (0)     // 0/1
    #define ESP_PANEL_BOARD_LCD_SPI_SCL_ACTIVE_EDGE     (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX       (0)     // Delete the control panel instance automatically if set to 1.
                                                                // If the 3-wire SPI pins are sharing other pins of the RGB interface to save GPIOs,
                                                                // Please set it to 1 to release the control panel and its pins (except CS signal).
    #define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD           (!ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX)
                                                                // The `mirror()` function will be implemented by LCD command if set to 1.
#endif

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
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() \
    { \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC2, {0x31, 0x05}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xCD, {0x00}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB0, {0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0xAA, 0x31, 0x18}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB1, {0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22, 0x04, 0x11, 0x11, 0xA9, 0x32, 0x18}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x11}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB0, {0x60}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB1, {0x32}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB2, {0x07}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB3, {0x80}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB5, {0x49}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB7, {0x85}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB8, {0x21}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x78}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC2, {0x78}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE0, {0x00, 0x1B, 0x02}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE1, {0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE2, {0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0, 0x00, 0x00}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE3, {0x00, 0x00, 0x11, 0x11}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE4, {0x44, 0x44}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE5, {0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED, 0xD8, 0xA0, 0x10, 0xEF, 0xD8, 0xA0}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE6, {0x00, 0x00, 0x11, 0x11}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE7, {0x44, 0x44}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE8, {0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC, 0xD8, 0xA0, 0x0F, 0xEE, 0xD8, 0xA0}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xEB, {0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xEC, {0x3C, 0x00}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xED, {0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x45, 0x67, 0x98, 0xBA}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x13}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE5, {0xE4}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x00}), \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x11), \
    }

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS    (18)        // 8/16/18/24
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_INEVRT_COLOR  (0)         // 0/1

/* LCD Transformation Flags */
#define ESP_PANEL_BOARD_LCD_SWAP_XY       (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X      (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y      (0)         // 0/1

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_BOARD_LCD_RST_IO        (-1)
#define ESP_PANEL_BOARD_LCD_RST_LEVEL     (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_USE_LCD */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an touch panel */
#define ESP_PANEL_BOARD_USE_TOUCH         (1)         // 0/1
#if ESP_PANEL_BOARD_USE_TOUCH
/**
 * Touch controller name
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        GT911


/* Touch Panel Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST      (0)     // 0/1
/**
 * Touch panel bus type
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE            (ESP_PANEL_BUS_TYPE_I2C)
/* Touch panel bus parameters */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    #define ESP_PANEL_BOARD_TOUCH_I2C_HOST_ID     (0)     // Typically set to 0
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS     (0)     // Typically set to 0 to use default address
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ      (400 * 1000)
                                                    // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP  (1)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP  (1)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL      (45)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA      (19)
#endif

#endif /* ESP_PANEL_BOARD_TOUCH_BUS_TYPE */

/* Touch Transformation Flags */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY         (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X        (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y        (0)         // 0/1

/* Touch Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_BOARD_TOUCH_RST_IO          (-1)
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL       (0)         // 0: low level, 1: high level
/* IO num of INT pin, set to -1 if not use */
#define ESP_PANEL_BOARD_TOUCH_INT_IO          (-1)
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL       (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_USE_TOUCH */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ESP_PANEL_BOARD_USE_BACKLIGHT         (1)         // 0/1
#if ESP_PANEL_BOARD_USE_BACKLIGHT
    /**
     * Backlight control mode. Choose one of the following:
     *  - ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO: Use GPIO switch to control the backlight, only support on/off
     *  - ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC:    Use LEDC PWM to control the backlight, support brightness adjustment
     *  - ESP_PANEL_BACKLIGHT_TYPE_CUSTOM:      Use custom function to control the backlight
     */
    #define ESP_PANEL_BOARD_BACKLIGHT_TYPE        (ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC)

    #if ESP_PANEL_BOARD_BACKLIGHT_TYPE != ESP_PANEL_BACKLIGHT_TYPE_CUSTOM
        /* IO num of backlight pin */
        #define ESP_PANEL_BOARD_BACKLIGHT_IO          (38)
        #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL    (1)         // 0: low level, 1: high level
    #else
        /**
         * @brief Custom backlight control function
         *
         * @param[in] percent   Brightness percentage, 0-100
         * @param[in] user_data User data, default is a pointer of `Board`
         *
         */
        #define ESP_PANEL_BOARD_BACKLIGHT_CUSTOM_FUNCTION( percent, user_data )  \
            {  \
                Board *board = static_cast<Board *>(user_data);  \
            }
    #endif /* ESP_PANEL_BOARD_BACKLIGHT_TYPE */

    /* Set to 1 if you want to turn off the backlight after initializing the panel; otherwise, set it to turn on */
    #define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF    (0)         // 0: on, 1: off
#endif /* ESP_PANEL_BOARD_USE_BACKLIGHT */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_BOARD_USE_EXPANDER          (0)         // 0/1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please utilize the following macros to execute any additional code if required. //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_POST_BEGIN_FUNCTION( p )

// *INDENT-ON*
