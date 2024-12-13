/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "soc/soc_caps.h"
#if SOC_MIPI_DSI_SUPPORTED
#include <memory>
#include "esp_ldo_regulator.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_panel_types.h"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

// Forward declaration
class BusHost_DSI;

/**
 * @brief MIPI-DSI bus object class
 *
 * @note  This class is a derived class of `Bus`, user can use it directly
 */
class Bus_DSI: public Bus {
public:
    constexpr static int HOST_ID_DEFAULT = 0;
    constexpr static int PHY_LDO_VOLTAGE_MV_DEFAULT = 2500;
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_MIPI_DSI,
        .name = "MIPI-DSI",
    };

// *INDENT-OFF*
    struct Config {
        esp_lcd_dsi_bus_config_t getHostConfig(void) const
        {
            return {
                .bus_id = HOST_ID_DEFAULT,
                .num_data_lanes = static_cast<uint8_t>(host_num_data_lanes),
                .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,
                .lane_bit_rate_mbps = static_cast<uint32_t>(host_lane_bit_rate_mbps),
            };
        }

        esp_lcd_dbi_io_config_t getIO_Config(void) const
        {
            return {
                .virtual_channel = 0,
                .lcd_cmd_bits = 8,
                .lcd_param_bits = 8,
            };
        }

        esp_lcd_dpi_panel_config_t getPanelConfig(void) const
        {
            return {
                .virtual_channel = 0,
                .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,
                .dpi_clock_freq_mhz = static_cast<uint32_t>(panel_dpi_clock_freq_mhz),
                .pixel_format = (panel_bits_per_pixel == 16) ?  LCD_COLOR_PIXEL_FORMAT_RGB565 :
                                ((panel_bits_per_pixel == 18) ? LCD_COLOR_PIXEL_FORMAT_RGB666 :
                                                                LCD_COLOR_PIXEL_FORMAT_RGB888),
                .num_fbs = 1,
                .video_timing = {
                    .h_size = static_cast<uint32_t>(panel_h_size),
                    .v_size = static_cast<uint32_t>(panel_v_size),
                    .hsync_pulse_width = static_cast<uint32_t>(panel_hsync_pulse_width),
                    .hsync_back_porch = static_cast<uint32_t>(panel_hsync_back_porch),
                    .hsync_front_porch = static_cast<uint32_t>(panel_hsync_front_porch),
                    .vsync_pulse_width = static_cast<uint32_t>(panel_vsync_pulse_width),
                    .vsync_back_porch = static_cast<uint32_t>(panel_vsync_back_porch),
                    .vsync_front_porch = static_cast<uint32_t>(panel_vsync_front_porch),
                },
                .flags = {
                    .use_dma2d = true,
                },
            };
        }

        esp_ldo_channel_config_t getPHY_LDO_Config(void) const
        {
            return {
                .chan_id = phy_ldo_chan_id,
                .voltage_mv = PHY_LDO_VOLTAGE_MV_DEFAULT,
            };
        }

        static Config create(
            /* DSI */
            int lane_num, int lane_rate_mbps,
            /* DPI */
            int clk_mhz, int bits_per_pixel, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp,
            /* PHY LDO */
            int phy_ldo_id = -1
        )
        {
            return {
                // Host
                .host_num_data_lanes = lane_num,
                .host_lane_bit_rate_mbps = lane_rate_mbps,
                // Panel
                .panel_dpi_clock_freq_mhz = clk_mhz,
                .panel_bits_per_pixel = bits_per_pixel,
                .panel_h_size = h_res,
                .panel_v_size = v_res,
                .panel_hsync_pulse_width = hpw,
                .panel_hsync_back_porch = hbp,
                .panel_hsync_front_porch = hfp,
                .panel_vsync_pulse_width = vpw,
                .panel_vsync_back_porch = vbp,
                .panel_vsync_front_porch = vfp,
                // PHY LDO
                .phy_ldo_chan_id = phy_ldo_id,
            };
        }

        // Host
        int host_num_data_lanes = 2;
        int host_lane_bit_rate_mbps = 0;
        // Panel
        int panel_dpi_clock_freq_mhz = 0;
        int panel_bits_per_pixel = 16;
        int panel_h_size = 0;
        int panel_v_size = 0;
        int panel_hsync_pulse_width = 0;
        int panel_hsync_back_porch = 0;
        int panel_hsync_front_porch = 0;
        int panel_vsync_pulse_width = 0;
        int panel_vsync_back_porch = 0;
        int panel_vsync_front_porch = 0;
        // PHY LDO
        int phy_ldo_chan_id = -1;
    };

    /**
     * @brief Construct a MIPI-DSI bus object in a common way, the host_handle will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_HOST_DSI_CONFIG_DEFAULT && ESP_PANEL_IO_DBI_CONFIG_DEFAULT)
     *        to config the bus object, use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param ldo_chan_id  MIPI-DSI CS pin
     */
    Bus_DSI(
        /* DSI */
        int lane_num, int lane_rate_mbps,
        /* DPI */
        int clk_mhz, int bits_per_pixel, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp,
        /* PHY LDO */
        int phy_ldo_id = -1
    ):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(
            /* DSI */
            lane_num, lane_rate_mbps,
            /* DPI */
            clk_mhz, bits_per_pixel, h_res, v_res, hpw, hbp, hfp, vpw, vbp, vfp,
            /* PHY LDO */
            phy_ldo_id
        );
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
        _panel_config = config.getPanelConfig();
        _phy_ldo_config = config.getPHY_LDO_Config();
    }

    Bus_DSI(const Config &config):
        Bus(ATTRIBUTES_DEFAULT)
    {
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
        _panel_config = config.getPanelConfig();
        _phy_ldo_config = config.getPHY_LDO_Config();
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the MIPI-DSI bus object
     *
     */
    ~Bus_DSI() override;

    /**
     * @brief Here are some functions to configure the MIPI-DSI bus object. These functions should be called before `begin()`
     *
     */
    void configDpiFrameBufferNumber(uint8_t num);

    bool init(void) override;

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     *
     */
    bool del(void) override;

    bool checkIsInit(void)
    {
        return _flags.is_init;
    }

    /* Not implement the function here to avoid error: invalid use of incomplete type `BusHost_DSI` */
    esp_lcd_dsi_bus_handle_t getDSI_Handle(void);

    const esp_lcd_dsi_bus_config_t *getHostConfig(void)
    {
        return &_host_config;
    }

    const esp_lcd_dpi_panel_config_t *getPanelConfig(void)
    {
        return &_panel_config;
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `getHostConfig()` instead")]]
    const esp_lcd_dsi_bus_config_t *getDsiConfig(void)
    {
        return getHostConfig();
    }

    [[deprecated("Deprecated and will be removed in the next major version. Please use `getPanelConfig()` instead")]]
    const esp_lcd_dpi_panel_config_t *getDpiConfig(void)
    {
        return getPanelConfig();
    }

private:
    struct {
        uint8_t is_init: 1;
    } _flags = {};
    esp_lcd_dsi_bus_config_t _host_config = {};
    esp_lcd_dbi_io_config_t _io_config = {};
    esp_lcd_dpi_panel_config_t _panel_config = {};
    esp_ldo_channel_config_t _phy_ldo_config = {};
    esp_ldo_channel_handle_t _phy_ldo_handle = nullptr;
    std::shared_ptr<BusHost_DSI> _host = nullptr;
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_DSI` instead.
 *
 * Example of migration:
 *
 * Old:
 *  ESP_PanelBus_DSI bus;
 *
 * New:
 *  esp_panel::drivers::Bus_DSI bus;
 */
typedef esp_panel::drivers::Bus_DSI ESP_PanelBus_DSI __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_DSI` instead.")));

#endif /* SOC_MIPI_DSI_SUPPORTED */
