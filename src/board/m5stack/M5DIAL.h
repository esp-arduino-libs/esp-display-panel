/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
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
 * LCD Controller Name. Choose one of the following:
 *      - GC9A01, GC9B71, GC9503
 *      - ILI9341
 *      - NV3022B
 *      - SH8601
 *      - SPD2010
 *      - ST7262, ST7701, ST7789, ST7796, ST77916, ST77922
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER          GC9A01

/* LCD resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH         (240)
#define ESP_PANEL_BOARD_HEIGHT        (240)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 *
 * Set to 1 if only the RGB interface is used without the 3-wire SPI interface,
 */
#define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST    (0)     // 0/1
/**
 * LCD Bus Type. Choose one of the following:
 *      - ESP_PANEL_BUS_TYPE_I2C (not ready)
 *      - ESP_PANEL_BUS_TYPE_SPI
 *      - ESP_PANEL_BUS_TYPE_QSPI
 *      - ESP_PANEL_BUS_TYPE_I80 (not ready)
 *      - ESP_PANEL_BUS_TYPE_RGB (only supported for ESP32-S3)
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
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS             (7)
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK            (6)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MOSI           (5)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_MISO           (-1)    // -1 if not used
#endif
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DC             (4)
    #define ESP_PANEL_BOARD_LCD_SPI_MODE              (0)     // 0/1/2/3, typically set to 0
    #define ESP_PANEL_BOARD_LCD_SPI_CLK_HZ            (40 * 1000 * 1000)
                                                        // Should be an integer divisor of 80M, typically set to 40M
    #define ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ    (10)    // Typically set to 10
    #define ESP_PANEL_BOARD_LCD_SPI_CMD_BITS          (8)     // Typically set to 8
    #define ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS        (8)     // Typically set to 8

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI

    #define ESP_PANEL_BOARD_LCD_BUS_HOST_ID           (1)     // Typically set to 1
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS             (5)
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK            (9)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA0          (10)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA1          (11)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA2          (12)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_DATA3          (13)
#endif
    #define ESP_PANEL_BOARD_LCD_SPI_MODE              (0)     // 0/1/2/3, typically set to 0
    #define ESP_PANEL_BOARD_LCD_SPI_CLK_HZ            (40 * 1000 * 1000)
                                                        // Should be an integer divisor of 80M, typically set to 40M
    #define ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ    (10)    // Typically set to 10
    #define ESP_PANEL_BOARD_LCD_SPI_CMD_BITS          (32)    // Typically set to 32
    #define ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS        (8)     // Typically set to 8

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ            (16 * 1000 * 1000)
    #define ESP_PANEL_BOARD_LCD_RGB_HPW               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HBP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HFP               (20)
    #define ESP_PANEL_BOARD_LCD_RGB_VPW               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VBP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VFP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG   (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH        (16)    //  8 | 16
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS        (16)    // 24 | 16

    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE   (0)     // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                        // To enable the bounce buffer, set it to a non-zero value. Typically set to `ESP_PANEL_BOARD_WIDTH * 10`
                                                        // The size of the Bounce Buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`,
                                                        // where N is an even number.
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC          (46)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC          (3)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE             (17)   // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK           (9)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP           (-1)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0          (10)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1          (11)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2          (12)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3          (13)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4          (14)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5          (21)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6          (47)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7          (48)
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8          (45)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9          (38)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10         (39)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11         (40)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12         (41)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13         (42)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14         (2)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15         (1)
#endif
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS               (0)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK              (1)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SDA              (2)
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

#else

#error "The function is not ready and will be implemented in the future."

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
/*
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD()                                        \
    {                                                                          \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},              \
        {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},                                \
        {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},                                \
        {0x29, (uint8_t []){0x00}, 0, 120},                                    \
        or                                                                     \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),                   \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),                   \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),                               \
    }
*/

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS    (16)        // 8/16/18/24
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (1)         // 0/1
#define ESP_PANEL_BOARD_LCD_INEVRT_COLOR  (1)         // 0/1

/* LCD Transformation Flags */
#define ESP_PANEL_BOARD_LCD_SWAP_XY       (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X      (1)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y      (0)         // 0/1

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_BOARD_LCD_RST_IO        (8)
#define ESP_PANEL_BOARD_LCD_RST_LEVEL     (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_LCD */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an touch panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
/**
 * Touch controller name. Choose one of the following:
 *      - CST816S
 *      - FT5x06
 *      - GT911, GT1151
 *      - ST1633, ST7123
 *      - TT21100
 *      - XPT2046
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        FT5x06


/* Touch Panel Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST      (0)     // 0/1
/**
 * Touch panel bus type. Choose one of the following:
 *      - ESP_PANEL_BUS_TYPE_I2C
 *      - ESP_PANEL_BUS_TYPE_SPI
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE            (ESP_PANEL_BUS_TYPE_I2C)
/* Touch panel bus parameters */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID     (0)     // Typically set to 0
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS     (0)     // Typically set to 0 to use default address
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ      (400 * 1000)
                                                    // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP  (1)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP  (1)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL      (12)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA      (11)
#endif

#elif ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID         (1)     // Typically set to 1
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_CS           (5)
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK          (7)
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI         (6)
    #define ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO         (9)
#endif
    #define ESP_PANEL_BOARD_TOUCH_SPI_CLK_HZ          (1 * 1000 * 1000)
                                                        // Should be an integer divisor of 80M, typically set to 1M

#else

#error "The function is not ready and will be implemented in the future."

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
#define ESP_PANEL_BOARD_TOUCH_INT_IO          (14)
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL       (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
/* IO num of backlight pin */
#define ESP_PANEL_BOARD_BACKLIGHT_IO          (9)
#define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL    (1)         // 0: low level, 1: high level

/* Set to 1 if turn off the backlight after initializing the panel; otherwise, set it to turn on */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF    (0)         // 0: on, 1: off

/* Set to 1 if use PWM for brightness control */
#define ESP_PANEL_LCD_BL_USE_PWM        (1)         // 0/1
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER          (0)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
/**
 * IO expander name. Choose one of the following:
 *      - CH422G
 *      - HT8574
 *      - TCA95XX_8BIT
 *      - TCA95XX_16BIT
 */
#define ESP_PANEL_BOARD_EXPANDER_CHIP         TCA95XX_8BIT

/* IO expander Settings */
/**
 * If set to 1, the driver will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST       (0)     // 0/1
/* IO expander parameters */
#define ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS          (0x20)  // The actual I2C address. Even for the same model of IC,
                                                        // the I2C address may be different, and confirmation based on
                                                        // the actual hardware connection is required
#if !ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_EXPANDER_HOST_ID          (0)     // Typically set to 0
    #define ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ       (400 * 1000)
                                                        // Typically set to 400K
    #define ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP   (1)     // 0/1
    #define ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP   (1)     // 0/1
    #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL       (18)
    #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA       (8)
#endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER */

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

/**
 * Do not change the following versions, they are used to check if the configurations in this file are compatible with
 * the current version of `esp_panel_board_custom.h` in the library. The detailed rules are as follows:
 *
 *   1. If the major version is not consistent, then the configurations in this file are incompatible with the library
 *      and must be replaced with the file from the library.
 *   2. If the minor version is not consistent, this file might be missing some new configurations, which will be set to
 *      default values. It is recommended to replace it with the file from the library.
 *   3. Even if the patch version is not consistent, it will not affect normal functionality.
 */
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 1

// *INDENT-ON*
