/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ESP_PanelLog.h"
#include "ESP_Panel.h"
#include "esp_panel_library.hpp"

#ifdef ESP_PANEL_BOARD_USE_DEFAULT
#include <memory>
#include "driver/gpio.h"

#undef ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT 0

using namespace std;

/**
 * Macros for adding host of bus
 *
 */
#define _ADD_HOST(name, host, config, id) host->addHost##name(config, id)
#define ADD_HOST(name, host, config, id)  _ADD_HOST(name, host, config, id)
/**
 * Macros for creating panel bus
 *
 */
#define _CREATE_BUS_INIT_HOST(name, host_config, io_config, host_id) \
                                                    make_shared<ESP_PanelBus##name>(host_config, io_config, host_id)
#define CREATE_BUS_INIT_HOST(name, host_config, io_config, host_id) \
                                                        _CREATE_BUS_INIT_HOST(name, host_config, io_config, host_id)
#define _CREATE_BUS_SKIP_HOST(name, io_config, host_id) make_shared<ESP_PanelBus##name>(io_config, host_id)
#define CREATE_BUS_SKIP_HOST(name, io_config, host_id)  _CREATE_BUS_SKIP_HOST(name, io_config, host_id)
/**
 * Macros for configuration of panel IO
 *
 */
#define _LCD_PANEL_IO_SPI_CONFIG(name, line_config, scl_active_edge) \
                                                        name##_PANEL_IO_SPI_CONFIG(line_config, scl_active_edge)
#define LCD_PANEL_IO_SPI_CONFIG(name, line_config, scl_active_edge)  \
                                                    _LCD_PANEL_IO_SPI_CONFIG(name, line_config, scl_active_edge)
/**
 * Macros for creating device
 *
 */
#define _CREATE_LCD(name, bus, cfg) make_shared<ESP_PanelLcd_##name>(bus, cfg)
#define CREATE_LCD(name, bus, cfg)  _CREATE_LCD(name, bus, cfg)
#define _CREATE_TOUCH(name, bus, cfg) make_shared<ESP_PanelTouch_##name>(bus, cfg)
#define CREATE_TOUCH(name, bus, cfg)  _CREATE_TOUCH(name, bus, cfg)
#define _CREATE_EXPANDER(name, host_id, address) make_shared<ESP_IOExpander_##name>(host_id, address)
#define CREATE_EXPANDER(name, host_id, address)  _CREATE_EXPANDER(name, host_id, address)

static const char *TAG = "ESP_Panel";

#if ESP_PANEL_BOARD_DEFAULT_USE_LCD && defined(ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD)
static const esp_panel_lcd_vendor_init_cmd_t lcd_init_cmds[] = ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD();
#endif

ESP_Panel::ESP_Panel():
    _is_initialed(false),
    _use_external_expander(false),
    _lcd_bus_ptr(nullptr),
    _touch_bus_ptr(nullptr),
    _lcd_ptr(nullptr),
    _touch_ptr(nullptr),
    _backlight_ptr(nullptr),
    _host_ptr(nullptr),
    _expander_ptr(nullptr)
{
}

ESP_Panel::~ESP_Panel()
{
    if (!_is_initialed) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete panel failed");
    }

end:
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();
    ESP_LOGD(TAG, "Destroy");
}

void ESP_Panel::configExpander(ESP_IOExpander *expander)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (expander == nullptr) {
        ESP_LOGD(TAG, "Config NULL IO expander pointer");
    } else {
        _use_external_expander = true;
    }

    _expander_ptr.reset(expander);
}

bool ESP_Panel::init(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    shared_ptr<ESP_PanelHost> host_ptr = make_shared<ESP_PanelHost>();
    shared_ptr<ESP_PanelBus> lcd_bus_ptr = nullptr;
    shared_ptr<ESP_PanelLcd> lcd_ptr = nullptr;
    shared_ptr<ESP_PanelBus> touch_bus_ptr = nullptr;
    shared_ptr<ESP_PanelTouch> touch_ptr = nullptr;
    shared_ptr<ESP_PanelBacklight> backlight_ptr = nullptr;
    shared_ptr<ESP_IOExpander> expander_ptr = _expander_ptr;

    ESP_LOGD(TAG, "Panel init start");

    /* LCD related */
#if ESP_PANEL_BOARD_DEFAULT_USE_LCD
    ESP_LOGD(TAG, "Use LCD");

#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
    ESP_LOGD(TAG, "Use SPI bus");
    // SPI bus
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    spi_bus_config_t lcd_bus_host_config = {
        .mosi_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_MOSI,
        .miso_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_MISO,
        .sclk_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .data4_io_num = GPIO_NUM_NC,
        .data5_io_num = GPIO_NUM_NC,
        .data6_io_num = GPIO_NUM_NC,
        .data7_io_num = GPIO_NUM_NC,
        .max_transfer_sz = ESP_PANEL_HOST_SPI_MAX_TRANSFER_SIZE,
        .flags = SPICOMMON_BUSFLAG_MASTER,
        .intr_flags = 0,
    };
#endif
    // SPI panel IO
    esp_lcd_panel_io_spi_config_t lcd_panel_io_config = {
        .cs_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_CS,
        .dc_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_DC,
        .spi_mode = ESP_PANEL_BOARD_LCD_SPI_MODE,
        .pclk_hz = ESP_PANEL_BOARD_LCD_SPI_CLK_HZ,
        .trans_queue_depth = ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
        .lcd_cmd_bits = ESP_PANEL_BOARD_LCD_SPI_CMD_BITS,
        .lcd_param_bits = ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS,
        .flags = {
            .dc_low_on_data = 0,
            .octal_mode = 0,
            .quad_mode = 0,
            .sio_mode = 0,
            .lsb_first = 0,
            .cs_high_active = 0,
        },
    };

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI
    ESP_LOGD(TAG, "Use QSPI bus");
    // QSPI bus
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    spi_bus_config_t lcd_bus_host_config = {
        .data0_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_DATA0,
        .data1_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_DATA1,
        .sclk_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_SCK,
        .data2_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_DATA2,
        .data3_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_DATA3,
        .data4_io_num = -1,
        .data5_io_num = -1,
        .data6_io_num = -1,
        .data7_io_num = -1,
        .max_transfer_sz = ESP_PANEL_HOST_SPI_MAX_TRANSFER_SIZE,
        .flags = SPICOMMON_BUSFLAG_MASTER,
        .intr_flags = 0,
    };
#endif
    // QSPI panel IO
    esp_lcd_panel_io_spi_config_t lcd_panel_io_config = {
        .cs_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_CS,
        .dc_gpio_num = -1,
        .spi_mode = ESP_PANEL_BOARD_LCD_SPI_MODE,
        .pclk_hz = ESP_PANEL_BOARD_LCD_SPI_CLK_HZ,
        .trans_queue_depth = ESP_PANEL_BOARD_LCD_SPI_TRANS_QUEUE_SZ,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
        .lcd_cmd_bits = ESP_PANEL_BOARD_LCD_SPI_CMD_BITS,
        .lcd_param_bits = ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS,
        .flags = {
            .dc_low_on_data = 0,
            .octal_mode = 0,
            .quad_mode = 1,
            .sio_mode = 0,
            .lsb_first = 0,
            .cs_high_active = 0,
        },
    };

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB

    ESP_LOGD(TAG, "Use RGB bus");
    // 3-wire SPI panel IO
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    spi_line_config_t line_config = {
        .cs_io_type = (panel_io_type_t)ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER,
        .cs_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_CS,
        .scl_io_type = (panel_io_type_t)ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER,
        .scl_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_SCK,
        .sda_io_type = (panel_io_type_t)ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER,
        .sda_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_SDA,
        .io_expander = NULL,
    };
    esp_lcd_panel_io_3wire_spi_config_t lcd_panel_io_config =
        LCD_PANEL_IO_SPI_CONFIG(ESP_PANEL_BOARD_LCD_CONTROLLER, line_config, ESP_PANEL_BOARD_LCD_SPI_SCL_ACTIVE_EDGE);
#endif
    // RGB panel
    esp_lcd_rgb_panel_config_t rgb_panel_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .timings =  {
            .pclk_hz = ESP_PANEL_BOARD_LCD_RGB_CLK_HZ,
            .h_res = ESP_PANEL_BOARD_WIDTH,
            .v_res = ESP_PANEL_BOARD_HEIGHT,
            .hsync_pulse_width = ESP_PANEL_BOARD_LCD_RGB_HPW,
            .hsync_back_porch = ESP_PANEL_BOARD_LCD_RGB_HBP,
            .hsync_front_porch = ESP_PANEL_BOARD_LCD_RGB_HFP,
            .vsync_pulse_width = ESP_PANEL_BOARD_LCD_RGB_VPW,
            .vsync_back_porch = ESP_PANEL_BOARD_LCD_RGB_VBP,
            .vsync_front_porch = ESP_PANEL_BOARD_LCD_RGB_VFP,
            .flags = {
                .pclk_active_neg = ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG,
            },
        },
        .data_width = ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH,
        .bits_per_pixel = ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS,
        .num_fbs = 1,
        .bounce_buffer_size_px = ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE,
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0)
        .dma_burst_size = 64,
#else
        .sram_trans_align = 4,
        .psram_trans_align = 64,
#endif /* ESP_IDF_VERSION */
        .hsync_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC,
        .vsync_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC,
        .de_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_DE,
        .pclk_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_PCLK,
        .disp_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_DISP,
        .data_gpio_nums = {
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA0,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA1,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA2,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA3,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA4,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA5,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA6,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA7,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA8,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA9,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA10,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA11,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA12,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA13,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA14,
            ESP_PANEL_BOARD_LCD_RGB_IO_DATA15,
        },
        .flags = {
            .fb_in_psram = 1,
        },
    };

#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI

    ESP_LOGD(TAG, "Use MIPI-DSI bus");
    // MIPI-DSI bus
    esp_lcd_dsi_bus_config_t dsi_bus_config = ESP_PANEL_HOST_DSI_CONFIG_DEFAULT(
                ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_NUM, ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_RATE_MBPS
            );
    // MIPI-DPI panel
    esp_lcd_dpi_panel_config_t dpi_panel_config = ESP_PANEL_DPI_CONFIG_DEFAULT(
                ESP_PANEL_BOARD_LCD_MIPI_DPI_CLK_MHZ, ESP_PANEL_BOARD_LCD_MIPI_DPI_PIXEL_BITS,
                ESP_PANEL_BOARD_WIDTH, ESP_PANEL_BOARD_HEIGHT,
                ESP_PANEL_BOARD_LCD_MIPI_DPI_HPW, ESP_PANEL_BOARD_LCD_MIPI_DPI_HBP, ESP_PANEL_BOARD_LCD_MIPI_DPI_HFP,
                ESP_PANEL_BOARD_LCD_MIPI_DPI_VPW, ESP_PANEL_BOARD_LCD_MIPI_DPI_VBP, ESP_PANEL_BOARD_LCD_MIPI_DPI_VFP
            );

#else

#error "This function is not ready and will be implemented in the future."

#endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */

    // LCD vendor configuration
    esp_panel_lcd_vendor_config_t lcd_vendor_config = {
#ifdef ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD
        .init_cmds = lcd_init_cmds,
        .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
#endif
        .flags = {
#if (ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB) && !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            .mirror_by_cmd = ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD,
            .auto_del_panel_io = ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX,
#endif
        },
    };

    // LCD device configuration
    esp_lcd_panel_dev_config_t lcd_config = {
        .reset_gpio_num = ESP_PANEL_BOARD_LCD_RST_IO,
        .rgb_ele_order = (lcd_rgb_element_order_t)ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER,
        .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
        .bits_per_pixel = ESP_PANEL_BOARD_LCD_COLOR_BITS,
        .flags = {
            .reset_active_high = ESP_PANEL_BOARD_LCD_RST_LEVEL,
        },
        .vendor_config = &lcd_vendor_config,
    };

    ESP_LOGD(TAG, "Create LCD bus");
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
    /* For non-RGB LCD, should use `CREATE_BUS_INIT_HOST()` to init host instead of `ADD_HOST()` */
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    lcd_bus_ptr = CREATE_BUS_INIT_HOST(ESP_PANEL_LCD_BUS_NAME, lcd_panel_io_config, rgb_panel_config, ESP_PANEL_LCD_BUS_HOST);
#else
    lcd_bus_ptr = CREATE_BUS_SKIP_HOST(ESP_PANEL_LCD_BUS_NAME, rgb_panel_config, ESP_PANEL_LCD_BUS_HOST);
#endif
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI
    lcd_bus_ptr = CREATE_BUS_INIT_HOST(
                      ESP_PANEL_LCD_BUS_NAME, dsi_bus_config, dpi_panel_config, ESP_PANEL_BOARD_LCD_MIPI_DSI_PHY_LDO_ID
                  );
#else
    /* For other LCDs, should use `ADD_HOST()` to init host when `ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST` enabled */
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    ESP_UTILS_CHECK_FALSE_RET(ADD_HOST(ESP_PANEL_LCD_BUS_NAME, host_ptr, lcd_bus_host_config, ESP_PANEL_LCD_BUS_HOST),
                              false, "Add host failed");
#endif
    lcd_bus_ptr = CREATE_BUS_SKIP_HOST(ESP_PANEL_LCD_BUS_NAME, lcd_panel_io_config, ESP_PANEL_LCD_BUS_HOST);
#endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */

    ESP_UTILS_CHECK_NULL_RET(lcd_bus_ptr, false, "Create LCD bus failed");

    ESP_LOGD(TAG, "Create LCD device");
    lcd_ptr = CREATE_LCD(ESP_PANEL_BOARD_LCD_CONTROLLER, lcd_bus_ptr.get(), lcd_config);
    ESP_UTILS_CHECK_NULL_RET(lcd_ptr, false, "Create LCD device failed");
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_LCD */

    /* Touch related configuration */
#if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
    ESP_LOGD(TAG, "Use touch");
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    ESP_LOGD(TAG, "Use I2C bus");
    // I2C bus
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    i2c_config_t touch_host_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA,
        .scl_io_num = ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL,
        .sda_pullup_en = ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP,
        .scl_pullup_en = ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP,
        .master = {
            .clk_speed = ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ,
        },
        .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
    };
#endif
    // I2C touch panel IO
    esp_lcd_panel_io_i2c_config_t touch_panel_io_config =
#if ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS == 0
        ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(ESP_PANEL_BOARD_TOUCH_CONTROLLER);
#else
        ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(ESP_PANEL_BOARD_TOUCH_CONTROLLER, ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS);
#endif

#elif ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI

    ESP_LOGD(TAG, "Use SPI bus");
    // SPI bus
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    spi_bus_config_t touch_host_config = {
        .mosi_io_num = ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI,
        .miso_io_num = ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO,
        .sclk_io_num = ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .data4_io_num = GPIO_NUM_NC,
        .data5_io_num = GPIO_NUM_NC,
        .data6_io_num = GPIO_NUM_NC,
        .data7_io_num = GPIO_NUM_NC,
        .max_transfer_sz = ESP_PANEL_HOST_SPI_MAX_TRANSFER_SIZE,
        .flags = SPICOMMON_BUSFLAG_MASTER,
        .intr_flags = 0,
    };
#endif
    // SPI panel IO
    esp_lcd_panel_io_spi_config_t touch_panel_io_config = ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(ESP_PANEL_BOARD_TOUCH_CONTROLLER,
            ESP_PANEL_BOARD_TOUCH_SPI_IO_CS);

#else

#error "This function is not ready and will be implemented in the future."

#endif /* ESP_PANEL_BOARD_TOUCH_BUS_TYPE */

    // Touch device configuration
    esp_lcd_touch_config_t lcd_touch_config = {
        .x_max = ESP_PANEL_TOUCH_H_RES,
        .y_max = ESP_PANEL_TOUCH_V_RES,
        .rst_gpio_num = (gpio_num_t)ESP_PANEL_BOARD_TOUCH_RST_IO,
        .int_gpio_num = (gpio_num_t)ESP_PANEL_BOARD_TOUCH_INT_IO,
        .levels = {
            .reset = ESP_PANEL_BOARD_TOUCH_RST_LEVEL,
            .interrupt = ESP_PANEL_BOARD_TOUCH_INT_LEVEL,
        },
        .flags = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        },
        .process_coordinates = NULL,
        .interrupt_callback = NULL,
        .user_data = NULL,
        .driver_data = NULL,
    };

#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    ESP_UTILS_CHECK_FALSE_RET(ADD_HOST(ESP_PANEL_TOUCH_BUS_NAME, host_ptr, touch_host_config, ESP_PANEL_TOUCH_BUS_HOST),
                              false, "Add host failed");
#endif

    ESP_LOGD(TAG, "Create touch bus");
    touch_bus_ptr = CREATE_BUS_SKIP_HOST(ESP_PANEL_TOUCH_BUS_NAME, touch_panel_io_config, ESP_PANEL_TOUCH_BUS_HOST);
    ESP_UTILS_CHECK_NULL_RET(touch_bus_ptr, false, "Create touch bus failed");

    ESP_LOGD(TAG, "Create touch device");
    touch_ptr = CREATE_TOUCH(ESP_PANEL_BOARD_TOUCH_CONTROLLER, touch_bus_ptr.get(), lcd_touch_config);
    ESP_UTILS_CHECK_NULL_RET(touch_ptr, false, "Create touch device failed");
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

    // Create backlight device
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
    ESP_LOGD(TAG, "Use backlight");
    ESP_LOGD(TAG, "Create backlight device");
    backlight_ptr = make_shared<ESP_PanelBacklight>(ESP_PANEL_BOARD_BACKLIGHT_IO, ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL, true);
    ESP_UTILS_CHECK_NULL_RET(backlight_ptr, false, "Create backlight failed");
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */

    // Create IO expander device
#if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
    ESP_LOGD(TAG, "Use IO expander");
    if (expander_ptr == nullptr) {
#if !ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
        i2c_config_t expander_host_config = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA,
            .scl_io_num = ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL,
            .sda_pullup_en = ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP,
            .scl_pullup_en = ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP,
            .master = {
                .clk_speed = ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ,
            },
            .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
        };
        ESP_UTILS_CHECK_FALSE_RET(ADD_HOST(I2C, host_ptr, expander_host_config, ESP_PANEL_EXPANDER_HOST), false,
                                  "Add host failed");
#endif
        expander_ptr = CREATE_EXPANDER(ESP_PANEL_BOARD_EXPANDER_CHIP, ESP_PANEL_EXPANDER_HOST, ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS);
    }
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER */

    ESP_LOGD(TAG, "Initialize host");
    ESP_UTILS_CHECK_FALSE_RET(host_ptr->begin(), false, "Initialize host failed");

    // Save the created devices
    _lcd_bus_ptr = lcd_bus_ptr;
    _lcd_ptr = lcd_ptr;
    _touch_bus_ptr = touch_bus_ptr;
    _touch_ptr = touch_ptr;
    _backlight_ptr = backlight_ptr;
    _host_ptr = host_ptr;
    _expander_ptr = expander_ptr;
    _is_initialed = true;

    ESP_LOGD(TAG, "Panel init end");

    return true;
}

bool ESP_Panel::begin(void)
{
    ESP_UTILS_CHECK_FALSE_RET(_is_initialed, false, "Panel is not initialized");

    ESP_LOGD(TAG, "Panel begin start");
    // Run additional code before starting the panel if needed
#ifdef ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION
    ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION(this);
#endif

#if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
    // Run additional code before starting the IO expander if needed
#ifdef ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION
    ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION(this);
#endif
    ESP_LOGD(TAG, "Begin IO expander");
    _expander_ptr->init();
    _expander_ptr->begin();
    // Run additional code after the IO expander is started if needed
#ifdef ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION
    ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION(this);
#endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER */

#if ESP_PANEL_BOARD_DEFAULT_USE_LCD
    // Run additional code before starting the bus and LCD if needed
#ifdef ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION
    ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION(this);
#endif
    ESP_LOGD(TAG, "Begin LCD");
#if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER && ((ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER) || (ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER) || \
                               (ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER))
    if (_lcd_bus_ptr->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        shared_ptr<ESP_PanelBusRGB> lcd_bus_ptr = std::static_pointer_cast<ESP_PanelBusRGB>(_lcd_bus_ptr);
        lcd_bus_ptr->configSpiLine(
            ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER, ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER,
            ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER, _expander_ptr.get()
        );
    }
#endif
    ESP_UTILS_CHECK_FALSE_RET(_lcd_bus_ptr->begin(), false, "Begin LCD bus failed");
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->init(), false, "Initialize LCD failed");
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->reset(), false, "Reset LCD failed");
    // Operate LCD device according to the optional configurations
#ifdef ESP_PANEL_BOARD_LCD_SWAP_XY
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->swapXY(ESP_PANEL_BOARD_LCD_SWAP_XY), false, "Swap XY failed");
#endif
#ifdef ESP_PANEL_BOARD_LCD_MIRROR_X
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->mirrorX(ESP_PANEL_BOARD_LCD_MIRROR_X), false, "Mirror X failed");
#endif
#ifdef ESP_PANEL_BOARD_LCD_MIRROR_Y
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->mirrorY(ESP_PANEL_BOARD_LCD_MIRROR_Y), false, "Mirror Y failed");
#endif
#ifdef ESP_PANEL_BOARD_LCD_INEVRT_COLOR
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->invertColor(ESP_PANEL_BOARD_LCD_INEVRT_COLOR), false, "Invert color failed");
#endif
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->begin(), false, "Begin LCD failed");
    ESP_UTILS_CHECK_FALSE_RET(_lcd_ptr->displayOn(), false, "Display on failed");
    // Run additional code after the LCD is started if needed
#ifdef ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION
    ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION(this);
#endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_LCD */

#if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
    // Run additional code before starting the bus and touch if needed
#ifdef ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION
    ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION(this);
#endif
    ESP_LOGD(TAG, "Begin touch");
    ESP_UTILS_CHECK_FALSE_RET(_touch_bus_ptr->begin(), false, "Begin touch bus failed");
    ESP_UTILS_CHECK_FALSE_RET(_touch_ptr->init(), false, "Initialize touch failed");
    ESP_UTILS_CHECK_FALSE_RET(_touch_ptr->begin(), false, "Begin touch failed");
#ifdef ESP_PANEL_BOARD_TOUCH_SWAP_XY
    ESP_UTILS_CHECK_FALSE_RET(_touch_ptr->swapXY(ESP_PANEL_BOARD_TOUCH_SWAP_XY), false, "Swap XY failed");
#endif
#ifdef ESP_PANEL_BOARD_TOUCH_MIRROR_X
    ESP_UTILS_CHECK_FALSE_RET(_touch_ptr->mirrorX(ESP_PANEL_BOARD_TOUCH_MIRROR_X), false, "Mirror X failed");
#endif
#ifdef ESP_PANEL_BOARD_TOUCH_MIRROR_Y
    ESP_UTILS_CHECK_FALSE_RET(_touch_ptr->mirrorY(ESP_PANEL_BOARD_TOUCH_MIRROR_Y), false, "Mirror Y failed");
#endif
    // Run additional code after the touch is started if needed
#ifdef ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION
    ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION(this);
#endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
    // Run additional code before starting the backlight if needed
#ifdef ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION
    ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION(this);
#endif
    ESP_LOGD(TAG, "Begin backlight");
    ESP_UTILS_CHECK_FALSE_RET(_backlight_ptr->begin(), false, "Initialize backlight failed");
#if ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF
    ESP_UTILS_CHECK_FALSE_RET(_backlight_ptr->off(), false, "Turn off backlight failed");
#else
    ESP_UTILS_CHECK_FALSE_RET(_backlight_ptr->on(), false, "Turn on backlight failed");
#endif
    // Run additional code after the backlight is started if needed
#ifdef ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION
    ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION(this);
#endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */

    ESP_LOGD(TAG, "Panel begin end");
    // Run additional code after the panel is started if needed
#ifdef ESP_PANEL_BOARD_POST_BEGIN_FUNCTION
    ESP_PANEL_BOARD_POST_BEGIN_FUNCTION(this);
#endif

    return true;
}

bool ESP_Panel::del(void)
{
    ESP_UTILS_CHECK_FALSE_RET(_is_initialed, false, "Panel is not initialized");

    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (_lcd_bus_ptr != nullptr) {
        ESP_LOGD(TAG, "Delete LCD bus");
        _lcd_bus_ptr = nullptr;
    }

    if (_lcd_ptr != nullptr) {
        ESP_LOGD(TAG, "Delete LCD device");
        _lcd_ptr = nullptr;
    }

    if (_touch_bus_ptr != nullptr) {
        ESP_LOGD(TAG, "Delete touch bus");
        _touch_bus_ptr = nullptr;
    }

    if (_touch_ptr != nullptr) {
        ESP_LOGD(TAG, "Delete touch device");
        _touch_ptr = nullptr;
    }

    if (_backlight_ptr != nullptr) {
        ESP_LOGD(TAG, "Delete backlight bus");
        _backlight_ptr = nullptr;
    }

    if (!_use_external_expander && (_expander_ptr != nullptr)) {
        ESP_LOGD(TAG, "Delete IO expander");
        _expander_ptr = nullptr;
    }

    if (_host_ptr != nullptr) {
        ESP_LOGD(TAG, "Delete host");
        _host_ptr = nullptr;
    }

    _is_initialed = false;
    ESP_LOGD(TAG, "Delete panel");

    return true;
}

ESP_PanelLcd *ESP_Panel::getLcd(void)
{
    return _lcd_ptr.get();
}

ESP_PanelTouch *ESP_Panel::getTouch(void)
{
    return _touch_ptr.get();
}

ESP_PanelBacklight *ESP_Panel::getBacklight(void)
{
    return _backlight_ptr.get();
}

ESP_IOExpander *ESP_Panel::getExpander(void)
{
    return _expander_ptr.get();
}

#endif /* ESP_PANEL_BOARD_USE_DEFAULT */
