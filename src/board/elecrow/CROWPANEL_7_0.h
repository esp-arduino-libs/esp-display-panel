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
#define ESP_PANEL_BOARD_LCD_CONTROLLER          EK9716B // Fitipower EK9716B

/* LCD resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH         (800)
#define ESP_PANEL_BOARD_HEIGHT        (480)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST    (1)     // 0/1
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

    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ            (14 * 1000 * 1000)
    #define ESP_PANEL_BOARD_LCD_RGB_HPW               (48)    // Hsync pulse width x
    #define ESP_PANEL_BOARD_LCD_RGB_HBP               (40)    // Hsync back porch x
    #define ESP_PANEL_BOARD_LCD_RGB_HFP               (40)    // Hsync front porch x
    #define ESP_PANEL_BOARD_LCD_RGB_VPW               (31)    // Vsync pulse width x
    #define ESP_PANEL_BOARD_LCD_RGB_VBP               (13)    // Vsync back porch x
    #define ESP_PANEL_BOARD_LCD_RGB_VFP               (1)    // Vsync front porch
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG   (1)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH        (16)    //  8 | 16
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS        (16)    // 24 | 16

    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_BOARD_WIDTH * 10)
                                                        // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                        // To enable the bounce buffer, set it to a non-zero value.
                                                        // Typically set to `ESP_PANEL_BOARD_WIDTH * 10`
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC          (39)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC          (40)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE             (41)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK           (0)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP           (-1)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0          (15)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1          (7)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2          (6)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3          (5)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4          (4)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5          (9)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6          (46)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7          (3)
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8          (8)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9          (16)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10         (1)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11         (14)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12         (21)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13         (47)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14         (48)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15         (45)
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
#define ESP_PANEL_BOARD_LCD_COLOR_BITS    (24)        // 8/16/18/24
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

/* Touch resolution in pixels */
#define ESP_PANEL_TOUCH_H_RES       (ESP_PANEL_BOARD_HEIGHT)   // Typically set to the same value as the width of LCD
#define ESP_PANEL_TOUCH_V_RES       (ESP_PANEL_BOARD_WIDTH)    // Typically set to the same value as the height of LCD

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
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL      (20)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA      (19)
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
#define ESP_PANEL_BOARD_TOUCH_INT_IO          (38)
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL       (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
/* IO num of backlight pin */
#define ESP_PANEL_BOARD_BACKLIGHT_IO          (2)
#define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL    (1)         // 0: low level, 1: high level

/* Set to 1 if you want to turn off the backlight after initializing the panel; otherwise, set it to turn on */
#define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF    (0)         // 0: on, 1: off

/* Set to 1 if use PWM for brightness control */
#define ESP_PANEL_LCD_BL_USE_PWM        (0)         // 0/1
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER          (0)         // 0/1

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please utilize the following macros to execute any additional code if required. //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION( p )
#define ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION( p )  \
    {  \
        /* Maintain the touch INT signal in a low state during the reset process to set its I2C address to `0x5D` */ \
        gpio_set_direction((gpio_num_t)ESP_PANEL_BOARD_TOUCH_INT_IO, GPIO_MODE_OUTPUT); \
        gpio_set_level((gpio_num_t)ESP_PANEL_BOARD_TOUCH_INT_IO, 0); \
        vTaskDelay(pdMS_TO_TICKS(10)); \
    }

// #define ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION( p )
// #define ESP_PANEL_BOARD_POST_BEGIN_FUNCTION( p )

// *INDENT-ON*
