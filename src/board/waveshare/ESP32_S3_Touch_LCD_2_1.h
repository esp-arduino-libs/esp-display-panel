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

    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ            (16 * 1000 * 1000)
    #define ESP_PANEL_BOARD_LCD_RGB_HPW               (8)
    #define ESP_PANEL_BOARD_LCD_RGB_HBP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HFP               (50)
    #define ESP_PANEL_BOARD_LCD_RGB_VPW               (3)
    #define ESP_PANEL_BOARD_LCD_RGB_VBP               (8)
    #define ESP_PANEL_BOARD_LCD_RGB_VFP               (8)
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG   (0)     // 0: rising edge, 1: falling edge

                                                        // | 8-bit RGB888 | 16-bit RGB565 |
                                                        // |--------------|---------------|
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH        (16)    // |      8       |      16       |
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS        (16)    // |      24      |      16       |

    #define ESP_PANEL_BOARD_LCD_RGB_FRAME_BUF_NUM     (2)     // 1/2/3
    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_BOARD_WIDTH*10)     // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                        // To enable the bounce buffer, set it to a non-zero value. Typically set to `ESP_PANEL_BOARD_WIDTH * 10`
                                                        // The size of the Bounce Buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`,
                                                        // where N is an even number.
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC          (38)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC          (39)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE             (40)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK           (41)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP           (-1)    // -1 if not used

                                                        // | RGB565 | RGB666 | RGB888 |
                                                        // |--------|--------|--------|
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0          (5)     // |   B0   |  B0-1  |   B0-3 |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1          (45)    // |   B1   |  B2    |   B4   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2          (48)    // |   B2   |  B3    |   B5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3          (47)    // |   B3   |  B4    |   B6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4          (21)    // |   B4   |  B5    |   B7   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5          (14)    // |   G0   |  G0    |   G0-2 |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6          (13)    // |   G1   |  G1    |   G3   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7          (12)    // |   G2   |  G2    |   G4   |
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8          (11)    // |   G3   |  G3    |   G5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9          (10)    // |   G4   |  G4    |   G6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10         (9)     // |   G5   |  G5    |   G7   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11         (46)    // |   R0   |  R0-1  |   R0-3 |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12         (3)     // |   R1   |  R2    |   R4   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13         (8)     // |   R2   |  R3    |   R5   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14         (18)    // |   R3   |  R4    |   R6   |
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15         (17)    // |   R4   |  R5    |   R7   |
#endif

#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS               (2)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK              (2)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SDA              (1)
    #define ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER     (1)     // 0/1
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
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD()                                                                                                 \
    {                                                                                                                                   \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},                                                                       \
        {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},                                                                                         \
        {0xC1, (uint8_t []){0x0B, 0x02}, 2, 0},                                                                                         \
        {0xC2, (uint8_t []){0x07, 0x02}, 2, 0},                                                                                         \
        {0xCC, (uint8_t []){0x10}, 1, 0},                                                                                               \
        {0xCD, (uint8_t []){0x08}, 1, 0},                                                                                               \
        {0xB0, (uint8_t []){0x00, 0x11, 0x16, 0x0E, 0x11, 0x06, 0x05, 0x09, 0x08, 0x21, 0x06, 0x13, 0x10, 0x29, 0x31, 0x18}, 16, 0},    \
        {0xB1, (uint8_t []){0x00, 0x11, 0x16, 0x0E, 0x11, 0x07, 0x05, 0x09, 0x09, 0x21, 0x05, 0x13, 0x11, 0x2A, 0x31, 0x18}, 16, 0},    \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x11}, 5, 0},                                                                       \
        {0xB0, (uint8_t []){0x6D}, 1, 0},                                                                                               \
        {0xB1, (uint8_t []){0x37}, 1, 0},                                                                                               \
        {0xB2, (uint8_t []){0x81}, 1, 0},                                                                                               \
        {0xB3, (uint8_t []){0x80}, 1, 0},                                                                                               \
        {0xB5, (uint8_t []){0x43}, 1, 0},                                                                                               \
        {0xB7, (uint8_t []){0x85}, 1, 0},                                                                                               \
        {0xB8, (uint8_t []){0x20}, 1, 0},                                                                                               \
        {0xC1, (uint8_t []){0x78}, 1, 0},                                                                                               \
        {0xC2, (uint8_t []){0x78}, 1, 0},                                                                                               \
        {0xD0, (uint8_t []){0x88}, 1, 0},                                                                                               \
        {0xE0, (uint8_t []){0x00, 0x00, 0x02}, 3, 0},                                                                                   \
        {0xE1, (uint8_t []){0x03, 0xA0, 0x00, 0x00, 0x04, 0xA0, 0x00, 0x00, 0x00, 0x20, 0x20}, 11, 0},                                  \
        {0xE2, (uint8_t []){0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 13, 0},                      \
        {0xE3, (uint8_t []){0x00, 0x00, 0x11, 0x00}, 4, 0},                                                                             \
        {0xE4, (uint8_t []){0x22, 0x00}, 2, 0},                                                                                         \
        {0xE5, (uint8_t []){0x05, 0xEC, 0xA0, 0xA0, 0x07, 0xEE, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 16, 0},    \
        {0xE6, (uint8_t []){0x00, 0x00, 0x11, 0x00}, 4, 0},                                                                             \
        {0xE7, (uint8_t []){0x22, 0x00}, 2, 0},                                                                                         \
        {0xE8, (uint8_t []){0x06, 0xED, 0xA0, 0xA0, 0x08, 0xEF, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 16, 0},    \
        {0xEB, (uint8_t []){0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00}, 7, 0},                                                           \
        {0xED, (uint8_t []){0xFF, 0xFF, 0xFF, 0xBA, 0x0A, 0xBF, 0x45, 0xFF, 0xFF, 0x54, 0xFB, 0xA0, 0xAB, 0xFF, 0xFF, 0xFF}, 16, 0},    \
        {0xEF, (uint8_t []){0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F}, 6, 0},                                                                 \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x13}, 5, 0},                                                                       \
        {0xEF, (uint8_t []){0x08}, 1, 0},                                                                                               \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x00}, 5, 0},                                                                       \
        {0x36, (uint8_t []){0x00}, 1, 0},                                                                                               \
        {0x3A, (uint8_t []){0x66}, 1, 0},                                                                                               \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(480, 0x11),                                                                                   \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x20),                                                                                   \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(0, 0x29)                                                                                      \
    }

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS    (16)        // 8/16/18/24
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
/* Reset pin */
#define ESP_PANEL_BOARD_LCD_RST_IO          (-1)      // IO num of RESET pin, set to -1 if not use
#define ESP_PANEL_BOARD_LCD_RST_LEVEL       (0)       // Active level. 0: low level, 1: high level

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
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        CST816S     //Actually a CST820 but the data structure is similar


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
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS     (0)     // Typically set to 0 to use the default address.
                                                    // - For touchs with only one address, set to 0
                                                    // - For touchs with multiple addresses, set to 0 or the address
                                                    //   Like GT911, there are two addresses: 0x5D(default) and 0x14
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ      (400 * 1000)
                                                    // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP  (1)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP  (1)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL      (7)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA      (15)
#endif

#endif /* ESP_PANEL_BOARD_TOUCH_BUS_TYPE */

/* Touch Transformation Flags */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY         (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X        (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y        (0)         // 0/1

/* Touch Other Settings */
/* Reset pin */
#define ESP_PANEL_BOARD_TOUCH_RST_IO          (-1)        // IO num of RESET pin, set to -1 if not use
                                                    // For GT911, the RST pin is also used to configure the I2C address
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL       (0)         // Active level. 0: low level, 1: high level
/* Interrupt pin */
#define ESP_PANEL_BOARD_TOUCH_INT_IO          (16)        // IO num of INT pin, set to -1 if not use
                                                    // For GT911, the INT pin is also used to configure the I2C address
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL       (1)         // Active level. 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
/* Backlight pin */
#define ESP_PANEL_BOARD_BACKLIGHT_IO          (6)        // IO num of backlight pin
#define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL    (1)         // 0: low level, 1: high level

/* Set to 1 if you want to turn off the backlight after initializing the panel; otherwise, set it to turn on */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF    (0)         // 0: on, 1: off

/* Set to 1 if use PWM for brightness control */
#define ESP_PANEL_LCD_BL_USE_PWM        (1)         // 0/1
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER          (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
/**
 * IO expander name.
 */
#define ESP_PANEL_BOARD_EXPANDER_CHIP         TCA95XX_8BIT

/* IO expander Settings */
/**
 * If set to 1, the driver will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST       (1)     // 0/1
/* IO expander parameters */
#define ESP_PANEL_BOARD_EXPANDER_HOST_ID              (0)     // Typically set to 0
#define ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS          (0x20)  // The actual I2C address. Even for the same model of IC,
                                                        // the I2C address may be different, and confirmation based on
                                                        // the actual hardware connection is required
#if !ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ       (400 * 1000)
                                                        // Typically set to 400K
    #define ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP   (1)     // 0/1
    #define ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP   (1)     // 0/1
    #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL       (7)
    #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA       (15)
#endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please utilize the following macros to execute any additional code if required. //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION( p )
//  #define ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION( p )
#define ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION( p )   \
{  \
    _expander_ptr->pinMode(1,OUTPUT); \
    _expander_ptr->digitalWrite(1,LOW); \
    vTaskDelay(pdMS_TO_TICKS(30));  \
    _expander_ptr->digitalWrite(1,HIGH); \
    vTaskDelay(pdMS_TO_TICKS(50)); \
}
// #define ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_POST_BEGIN_FUNCTION( p )

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
 */
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 2
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 2


// *INDENT-ON*
