/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"
#if SOC_LCD_RGB_SUPPORTED
#include <memory>
#include <variant>
#include "esp_lcd_panel_rgb.h"
#include "esp_io_expander.hpp"
#include "esp_panel_types.h"
#include "port/esp_lcd_panel_io_additions.h"
#include "esp_panel_bus_base.hpp"

// *INDENT-OFF*
#ifdef SOC_LCDCAM_RGB_DATA_WIDTH
    #define ESP_PANEL_BUS_RGB_DATA_BITS SOC_LCDCAM_RGB_DATA_WIDTH
#elif defined(SOC_LCD_RGB_DATA_WIDTH)
    #define ESP_PANEL_BUS_RGB_DATA_BITS SOC_LCD_RGB_DATA_WIDTH
#else
    #warning "`SOC_LCDCAM_RGB_DATA_WIDTH` or `SOC_LCD_RGB_DATA_WIDTH` not defined, using default value 16"
    #define ESP_PANEL_BUS_RGB_DATA_BITS 16
#endif // SOC_LCDCAM_RGB_DATA_WIDTH
// *INDENT-OFF*

/**
 * For the ESP32-S3, the RGB peripheral only supports 16-bit RGB565 and 8-bit RGB888 color formats. For more details,
 * please refer to the part `Display > LCD Screen > RGB LCD Introduction` in ESP-IoT-Solution Programming Guide
 * (https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/rgb_lcd.html#color-formats).
 */

namespace esp_panel::drivers {

/**
 * @brief The RGB bus class
 *
 * @note  The class is a derived class of `Bus`, users can use it to construct the RGB bus
 */
class BusRGB: public Bus {
public:
    /**
     * Here are some default values for RGB bus
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_RGB,
        .name = "RGB",
    };
    static constexpr int RGB_PCLK_HZ_DEFAULT = 16 * 1000 * 1000;
    static constexpr int RGB_DATA_WIDTH_DEFAULT = 16;

    /**
     * @brief The RGB bus configuration structure
     */
    struct Config {
        using ControlPanelFullConfig = esp_lcd_panel_io_3wire_spi_config_t;
        using RefreshPanelFullConfig = esp_lcd_rgb_panel_config_t;

        struct ControlPanelPartialConfig {
            int cs_io_type = static_cast<int>(IO_TYPE_GPIO);
            int scl_io_type = static_cast<int>(IO_TYPE_GPIO);
            int sda_io_type = static_cast<int>(IO_TYPE_GPIO);
            int cs_gpio_num = -1;
            int scl_gpio_num = -1;
            int sda_gpio_num = -1;
            bool flags_scl_active_falling_edge = false;
        };
        struct RefreshPanelPartialConfig {
            int pclk_hz = RGB_PCLK_HZ_DEFAULT;
            int h_res = 0;
            int v_res = 0;
            int hsync_pulse_width = 10;
            int hsync_back_porch = 10;
            int hsync_front_porch = 20;
            int vsync_pulse_width = 10;
            int vsync_back_porch = 10;
            int vsync_front_porch = 10;
            int data_width = RGB_DATA_WIDTH_DEFAULT;
            int bits_per_pixel = RGB_DATA_WIDTH_DEFAULT;
            int bounce_buffer_size_px = 0;
            int hsync_gpio_num = -1;
            int vsync_gpio_num = -1;
            int de_gpio_num = -1;
            int pclk_gpio_num = -1;
            int disp_gpio_num = -1;
            int data_gpio_nums[ESP_PANEL_BUS_RGB_DATA_BITS] = {
                -1, -1, -1, -1, -1, -1, -1, -1,
    #if ESP_PANEL_BUS_RGB_DATA_BITS >= 16
                -1, -1, -1, -1, -1, -1, -1, -1
    #endif // ESP_PANEL_BUS_RGB_DATA_BITS
                };
            bool flags_pclk_active_neg = false;
        };

        void convertPartialToFull();
        void printControlPanelConfig() const;
        void printRefreshPanelConfig() const;

        const ControlPanelFullConfig *getControlPanelFullConfig() const
        {
            if (std::holds_alternative<ControlPanelPartialConfig>(control_panel)) {
                return nullptr;
            }
            return &std::get<ControlPanelFullConfig>(control_panel);
        }

        const RefreshPanelFullConfig *getRefreshPanelFullConfig() const
        {
            if (std::holds_alternative<RefreshPanelPartialConfig>(refresh_panel)) {
                return nullptr;
            }
            return &std::get<RefreshPanelFullConfig>(refresh_panel);
        }

        // Control Panel
        std::variant<ControlPanelFullConfig, ControlPanelPartialConfig> control_panel = {};
        // Refresh Panel
        std::variant<RefreshPanelFullConfig, RefreshPanelPartialConfig> refresh_panel = {};
        // Extra
        bool use_control_panel = false;
    };

// *INDENT-OFF*
    /**
     * @brief Construct the "3-wire SPI + 16-bit RGB" bus with separate parameters
     *
     * @note  This function uses some default values to config the bus, use `config*()` functions to change them
     *
     * @param[in] cs_io    3-wire SPI CS pin
     * @param[in] sck_io   3-wire SPI SCK pin
     * @param[in] sda_io   3-wire SPI SDA pin
     * @param[in] d[N]_io  RGB data pins, N is [0, 15]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 3-wire SPI IOs */
        int cs_io, int sck_io, int sda_io,
        /* 16-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Control Panel
            .control_panel = Config::ControlPanelPartialConfig{
                .cs_gpio_num = cs_io,
                .scl_gpio_num = sck_io,
                .sda_gpio_num = sda_io,
            },
            // Refresh Panel
            .refresh_panel = Config::RefreshPanelPartialConfig{
                .pclk_hz = clk_mhz * 1000 * 1000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 16,
                .bits_per_pixel = 16,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                    d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
                },
            },
            // Extra
            .use_control_panel = true,
        }
    {
    }

    /**
     * @brief Construct the single "16-bit RGB" bus with separate parameters
     *
     * @note  This function uses some default values to config the bus, use `config*()` functions to change them
     *
     * @param[in] d[N]_io  RGB data pins, N is [0, 15]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 16-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Refresh Panel
            .refresh_panel = Config::RefreshPanelPartialConfig{
                .pclk_hz = clk_mhz * 1000 * 1000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 16,
                .bits_per_pixel = 16,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                    d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
                },
            },
            // Extra
            .use_control_panel = false,
        }
    {
    }

    /**
     * @brief Construct the "3-wire SPI + 8-bit RGB" bus with separate parameters
     *
     * @note  This function uses some default values to config the bus, use `config*()` functions to change them
     *
     * @param[in] cs_io    3-wire SPI CS pin
     * @param[in] sck_io   3-wire SPI SCK pin
     * @param[in] sda_io   3-wire SPI SDA pin
     * @param[in] d[N]_io  RGB data pins, N is [0, 7]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 3-wire SPI IOs */
        int cs_io, int sck_io, int sda_io,
        /* 8-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Control Panel
            .control_panel = Config::ControlPanelPartialConfig{
                .cs_gpio_num = cs_io,
                .scl_gpio_num = sck_io,
                .sda_gpio_num = sda_io,
            },
            // Refresh Panel
            .refresh_panel = Config::RefreshPanelPartialConfig{
                .pclk_hz = clk_mhz * 1000 * 1000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 8,
                .bits_per_pixel = 24,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io
                },
            },
            // Extra
            .use_control_panel = true,
        }
    {
    }

    /**
     * @brief Construct the single "8-bit RGB" bus with separate parameters
     *
     * @note  This function uses some default values to config the bus, use `config*()` functions to change them
     *
     * @param[in] d[N]_io  RGB data pins, N is [0, 7]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 8-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Refresh Panel
            .refresh_panel = Config::RefreshPanelPartialConfig{
                .pclk_hz = clk_mhz * 1000 * 1000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 8,
                .bits_per_pixel = 24,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io
                },
            },
            // Extra
            .use_control_panel = false,
        }
    {
    }

    /**
     * @brief Construct the RGB bus with configuration
     *
     * @param[in] config RGB bus configuration
     */
    BusRGB(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the RGB bus
     */
    ~BusRGB() override;

    /**
     * Here are some functions to configure the bus. These functions should be called before `init()`
     */
    bool configSPI_IO_Type(bool cs_use_expander, bool sck_use_expander, bool sda_use_expander);
    bool configSPI_IO_Expander(esp_io_expander_t *handle);
    bool configSPI_SCL_ActiveFallingEdge(bool enable);
    bool configRGB_FrameBufferNumber(uint8_t num);
    bool configRGB_BounceBufferSize(uint32_t size_in_pixel);
    bool configRGB_TimingFlags(
        bool hsync_idle_low, bool vsync_idle_low, bool de_idle_high, bool pclk_active_neg, bool pclk_idle_high
    );

    /**
     * @brief Initialize the bus
     *
     * @return true if success, otherwise false
     */
    bool init() override;

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     */
    bool begin() override;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     */
    bool del() override;

    /**
     * @brief Get the bus configuration
     *
     * @return Bus configuration
     */
    const Config &getConfig() const
    {
        return _config;
    }

    // TODO: Remove in the next major version
    [[deprecated("Deprecated. Please use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int cs_io, int sck_io, int sda_io,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            cs_io, sck_io, sda_io,
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            16, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }
    [[deprecated("Deprecated. Please use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int cs_io, int sck_io, int sda_io,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            cs_io, sck_io, sda_io,
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            16, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }
    [[deprecated("Deprecated. Please use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            16, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }
    [[deprecated("Deprecated. Please use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            16, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }
    [[deprecated("Deprecated. Please use `configSPI_IO_Expander()` \
    instead.")]]
    bool configSpiLine(bool cs_use_expaneder, bool sck_use_expander, bool sda_use_expander, ESP_IOExpander *io_expander);
    [[deprecated("Deprecated. Please use `configRGB_FrameBufferNumber()` \
    instead.")]]
    bool configRgbFrameBufferNumber(uint8_t num)
    {
        return configRGB_FrameBufferNumber(num);
    }
    [[deprecated("Deprecated. Please use `configRGB_BounceBufferSize()` \
    instead.")]]
    bool configRgbBounceBufferSize(uint32_t size_in_pixel)
    {
        return configRGB_BounceBufferSize(size_in_pixel);
    }
    [[deprecated("Deprecated. Please use `configRGB_TimingFlags()` \
    instead.")]]
    bool configRgbTimingFlags(
        bool hsync_idle_low, bool vsync_idle_low, bool de_idle_high, bool pclk_active_neg, bool pclk_idle_high
    )
    {
        return configRGB_TimingFlags(hsync_idle_low, vsync_idle_low, de_idle_high, pclk_active_neg, pclk_idle_high);
    }
    [[deprecated("Deprecated.")]]
    bool configRgbFlagDispActiveLow();
    [[deprecated("Deprecated.")]]
    bool configRgbTimingFreqHz(uint32_t hz);
    [[deprecated("Deprecated.")]]
    bool configRgbTimingPorch(uint16_t hpw, uint16_t hbp, uint16_t hfp, uint16_t vpw, uint16_t vbp, uint16_t vfp);
    [[deprecated("Deprecated. Please use \
    `getConfig().getRefreshPanelFullConfig()` instead.")]]
    const esp_lcd_rgb_panel_config_t *getRgbConfig()
    {
        return getConfig().getRefreshPanelFullConfig();
    }

private:
    Config::ControlPanelFullConfig &getControlPanelFullConfig()
    {
        if (std::holds_alternative<Config::ControlPanelPartialConfig>(_config.control_panel)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::ControlPanelFullConfig>(_config.control_panel);
    }

    Config::RefreshPanelFullConfig &getRefreshPanelFullConfig()
    {
        if (std::holds_alternative<Config::RefreshPanelPartialConfig>(_config.refresh_panel)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::RefreshPanelFullConfig>(_config.refresh_panel);
    }

    Config _config = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::BusRGB`
 *             instead.
 *
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::BusRGB ESP_PanelBusRGB __attribute__((deprecated("Deprecated and will be removed in the \
next major version. Please use `esp_panel::drivers::BusRGB` instead.")));

#endif // SOC_LCD_RGB_SUPPORTED
