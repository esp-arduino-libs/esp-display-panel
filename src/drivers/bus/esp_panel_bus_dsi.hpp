/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "soc/soc_caps.h"
#if SOC_MIPI_DSI_SUPPORTED
#include <memory>
#include <variant>
#include "esp_ldo_regulator.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_panel_types.h"
#include "drivers/host/esp_panel_host_dsi.hpp"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The MIPI-DSI bus class
 *
 * @note  The class is a derived class of `Bus`, users can use it to construct the MIPI-DSI bus
 */
class BusDSI: public Bus {
public:
    /**
     * Here are some default values for MIPI-DSI bus
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_MIPI_DSI,
        .name = "MIPI-DSI",
    };
    static constexpr int HOST_ID_DEFAULT = 0;
    static constexpr int PHY_LDO_VOLTAGE_MV_DEFAULT = 2500;

    /**
     * @brief The MIPI-DSI bus configuration structure
     */
    struct Config {
        using HostFullConfig = esp_lcd_dsi_bus_config_t;
        using ControlPanelFullConfig = esp_lcd_dbi_io_config_t;
        using RefreshPanelFullConfig = esp_lcd_dpi_panel_config_t;
        using PHY_LDO_FullConfig = esp_ldo_channel_config_t;

        struct HostPartialConfig {
            int num_data_lanes = 2;
            int lane_bit_rate_mbps = 0;
        };
        struct RefreshPanelPartialConfig {
            int dpi_clock_freq_mhz = 0;
            int bits_per_pixel = 16;
            int h_size = 0;
            int v_size = 0;
            int hsync_pulse_width = 0;
            int hsync_back_porch = 0;
            int hsync_front_porch = 0;
            int vsync_pulse_width = 0;
            int vsync_back_porch = 0;
            int vsync_front_porch = 0;
        };
        struct PHY_LDO_PartialConfig {
            int chan_id = -1;
        };

        void convertPartialToFull();
        void printHostConfig() const;
        void printControlPanelConfig() const;
        void printRefreshPanelConfig() const;
        void printPHY_LDO_Config() const;

        const HostFullConfig *getHostFullConfig() const
        {
            if (std::holds_alternative<HostPartialConfig>(host)) {
                return nullptr;
            }

            return &std::get<HostFullConfig>(host);
        }

        const RefreshPanelFullConfig *getRefreshPanelFullConfig() const
        {
            if (std::holds_alternative<RefreshPanelPartialConfig>(refresh_panel)) {
                return nullptr;
            }

            return &std::get<RefreshPanelFullConfig>(refresh_panel);
        }

        const PHY_LDO_FullConfig *getPHY_LDO_Config() const
        {
            if (std::holds_alternative<PHY_LDO_PartialConfig>(phy_ldo)) {
                return nullptr;
            }

            return &std::get<PHY_LDO_FullConfig>(phy_ldo);
        }

        // Host
        std::variant<HostFullConfig, HostPartialConfig> host = {};
        // Control Panel
        ControlPanelFullConfig control_panel = {
            .virtual_channel = 0,
            .lcd_cmd_bits = 8,
            .lcd_param_bits = 8,
        };
        // Refresh Panel
        std::variant<RefreshPanelFullConfig, RefreshPanelPartialConfig> refresh_panel = {};
        // PHY LDO
        std::variant<PHY_LDO_FullConfig, PHY_LDO_PartialConfig> phy_ldo = {};
    };

// *INDENT-OFF*
    /**
     * @brief Construct the MIPI-DSI bus with separate parameters, the host will be initialized by the driver
     *
     * @note  This function uses some default values to config the bus, use `config*()` functions to change them
     *
     * @param[in] lane_num        The number of data lanes
     * @param[in] lane_rate_mbps  The bit rate of each lane in Mbps
     * @param[in] clk_mhz         The DPI clock frequency in MHz
     * @param[in] bits_per_pixel  The bits per pixel (16/18/24)
     * @param[in] h_res           The horizontal resolution
     * @param[in] v_res           The vertical resolution
     * @param[in] hpw             The horizontal sync pulse width
     * @param[in] hbp             The horizontal back porch
     * @param[in] hfp             The horizontal front porch
     * @param[in] vpw             The vertical sync pulse width
     * @param[in] vbp             The vertical back porch
     * @param[in] vfp             The vertical front porch
     * @param[in] phy_ldo_id      The channel ID of the PHY LDO, set to `-1` if not used
     */
    BusDSI(
        /* DSI */
        int lane_num, int lane_rate_mbps,
        /* DPI */
        int clk_mhz, int bits_per_pixel, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp,
        /* PHY LDO */
        int phy_ldo_id = -1
    ):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Host
            .host = Config::HostPartialConfig{
                .num_data_lanes = lane_num,
                .lane_bit_rate_mbps = lane_rate_mbps,
            },
            // Refresh Panel
            .refresh_panel = Config::RefreshPanelPartialConfig{
                .dpi_clock_freq_mhz = clk_mhz,
                .bits_per_pixel = bits_per_pixel,
                .h_size = h_res,
                .v_size = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
            },
            // PHY LDO
            .phy_ldo = Config::PHY_LDO_PartialConfig{
                .chan_id = phy_ldo_id,
            },
        }
    {
    }

    /**
     * @brief Construct the MIPI-DSI bus with configuration
     *
     * @param[in] config The MIPI-DSI bus configuration
     */
    BusDSI(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the MIPI-DSI bus
     */
    ~BusDSI() override;

    /**
     * Here are some functions to configure the bus. These functions should be called before `init()`
     */
    void configDPI_FrameBufferNumber(uint8_t num);

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
     * @brief Delete the bus, release the resources
     *
     * @return true if success, otherwise false
     */
    bool del() override;

    /**
     * @brief Get the bus host Handle
     *
     * @note  Not implement the function here to avoid error: invalid use of incomplete type `HostDSI`
     *
     * @return The host handle
     */
    esp_lcd_dsi_bus_handle_t getHostHandle();

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
    [[deprecated("Deprecated. Please use `configDPI_FrameBufferNumber()` \
    instead")]]
    void configDpiFrameBufferNumber(uint8_t num)
    {
        configDPI_FrameBufferNumber(num);
    }
    [[deprecated("Deprecated. Please use `getConfig()` instead")]]
    const esp_lcd_dsi_bus_config_t *getDsiConfig()
    {
        return getConfig().getHostFullConfig();
    }
    [[deprecated("Deprecated. Please use `getConfig()` instead")]]
    const esp_lcd_dpi_panel_config_t *getDpiConfig()
    {
        return getConfig().getRefreshPanelFullConfig();
    }

private:
    Config::RefreshPanelFullConfig &getRefreshPanelFullConfig()
    {
        if (std::holds_alternative<Config::RefreshPanelPartialConfig>(_config.refresh_panel)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::RefreshPanelFullConfig>(_config.refresh_panel);
    }

    Config _config = {};
    std::shared_ptr<HostDSI> _host = nullptr;
    esp_ldo_channel_handle_t _phy_ldo_handle = nullptr;
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::BusDSI`
 *             instead.
 *
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::BusDSI ESP_PanelBusDSI __attribute__((deprecated("Deprecated and will be removed in the \
next major version. Please use `esp_panel::drivers::BusDSI` instead.")));

#endif // SOC_MIPI_DSI_SUPPORTED
