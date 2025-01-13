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
#include "esp_panel_bus.hpp"

namespace esp_panel::drivers {

/**
 * @brief The MIPI-DSI bus class for ESP Panel
 *
 * This class is derived from `Bus` class and provides MIPI-DSI bus implementation for ESP Panel
 */
class BusDSI: public Bus {
public:
    /**
     * @brief Default values for MIPI-DSI bus configuration
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
        /**
         * @brief Host configuration types
         */
        using HostFullConfig = esp_lcd_dsi_bus_config_t;

        /**
         * @brief Control panel configuration types
         */
        using ControlPanelFullConfig = esp_lcd_dbi_io_config_t;

        /**
         * @brief Refresh panel configuration types
         */
        using RefreshPanelFullConfig = esp_lcd_dpi_panel_config_t;

        /**
         * @brief PHY LDO configuration types
         */
        using PHY_LDO_FullConfig = esp_ldo_channel_config_t;

        /**
         * @brief Partial host configuration structure
         */
        struct HostPartialConfig {
            int num_data_lanes = 2;        ///< Number of data lanes
            int lane_bit_rate_mbps = 0;    ///< Bit rate of each lane in Mbps
        };

        /**
         * @brief Partial refresh panel configuration structure
         */
        struct RefreshPanelPartialConfig {
            int dpi_clock_freq_mhz = 0;    ///< DPI clock frequency in MHz
            int bits_per_pixel = 16;       ///< Bits per pixel (16/18/24)
            int h_size = 0;                ///< Horizontal resolution
            int v_size = 0;                ///< Vertical resolution
            int hsync_pulse_width = 0;     ///< Horizontal sync pulse width
            int hsync_back_porch = 0;      ///< Horizontal back porch
            int hsync_front_porch = 0;     ///< Horizontal front porch
            int vsync_pulse_width = 0;     ///< Vertical sync pulse width
            int vsync_back_porch = 0;      ///< Vertical back porch
            int vsync_front_porch = 0;     ///< Vertical front porch
        };

        /**
         * @brief Partial PHY LDO configuration structure
         */
        struct PHY_LDO_PartialConfig {
            int chan_id = -1;              ///< Channel ID of the PHY LDO
        };

        /**
         * @brief Convert partial configurations to full configurations
         */
        void convertPartialToFull();

        /**
         * @brief Print host configuration for debugging
         */
        void printHostConfig() const;

        /**
         * @brief Print control panel configuration for debugging
         */
        void printControlPanelConfig() const;

        /**
         * @brief Print refresh panel configuration for debugging
         */
        void printRefreshPanelConfig() const;

        /**
         * @brief Print PHY LDO configuration for debugging
         */
        void printPHY_LDO_Config() const;

        /**
         * @brief Get the full host configuration if available
         *
         * @return Pointer to full host configuration, `nullptr` if using partial configuration
         */
        const HostFullConfig *getHostFullConfig() const;

        /**
         * @brief Get the full refresh panel configuration if available
         *
         * @return Pointer to full refresh panel configuration, `nullptr` if using partial configuration
         */
        const RefreshPanelFullConfig *getRefreshPanelFullConfig() const;

        /**
         * @brief Get the full PHY LDO configuration if available
         *
         * @return Pointer to full PHY LDO configuration, `nullptr` if using partial configuration
         */
        const PHY_LDO_FullConfig *getPHY_LDO_Config() const;

        std::variant<HostFullConfig, HostPartialConfig> host = {};  ///< Host configuration
        ControlPanelFullConfig control_panel = {                    ///< Control panel configuration
            .virtual_channel = 0,
            .lcd_cmd_bits = 8,
            .lcd_param_bits = 8,
        };
        ///< Refresh panel configuration
        std::variant<RefreshPanelFullConfig, RefreshPanelPartialConfig> refresh_panel = {};
        std::variant<PHY_LDO_FullConfig, PHY_LDO_PartialConfig> phy_ldo = {};  ///< PHY LDO configuration
    };

// *INDENT-OFF*
    /**
     * @brief Construct a new MIPI-DSI bus instance with individual parameters
     *
     * Uses default values for most configurations. Call `config*()` functions to modify the default settings
     *
     * @param[in] lane_num        Number of data lanes
     * @param[in] lane_rate_mbps  Bit rate of each lane in Mbps
     * @param[in] clk_mhz         DPI clock frequency in MHz
     * @param[in] bits_per_pixel  Bits per pixel (16/18/24)
     * @param[in] h_res           Horizontal resolution
     * @param[in] v_res           Vertical resolution
     * @param[in] hpw             Horizontal sync pulse width
     * @param[in] hbp             Horizontal back porch
     * @param[in] hfp             Horizontal front porch
     * @param[in] vpw             Vertical sync pulse width
     * @param[in] vbp             Vertical back porch
     * @param[in] vfp             Vertical front porch
     * @param[in] phy_ldo_id      Channel ID of the PHY LDO, set to `-1` if not used
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
     * @brief Construct a new MIPI-DSI bus instance with complete configuration
     *
     * @param[in] config Complete MIPI-DSI bus configuration
     */
    BusDSI(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-ON*

    /**
     * @brief Destroy the MIPI-DSI bus instance
     */
    ~BusDSI() override;

    /**
     * @brief Configure DPI frame buffer number
     *
     * @param[in] num Number of frame buffers
     * @note This function should be called before `init()`
     */
    void configDPI_FrameBufferNumber(uint8_t num);

    /**
     * @brief Initialize the MIPI-DSI bus
     *
     * @return `true` if initialization succeeds, `false` otherwise
     */
    bool init() override;

    /**
     * @brief Start the MIPI-DSI bus operation
     *
     * @return `true` if startup succeeds, `false` otherwise
     */
    bool begin() override;

    /**
     * @brief Delete the MIPI-DSI bus instance and release resources
     *
     * @return `true` if deletion succeeds, `false` otherwise
     */
    bool del() override;

    /**
     * @brief Get the MIPI-DSI bus host handle
     *
     * @return MIPI-DSI bus host handle
     */
    esp_lcd_dsi_bus_handle_t getHostHandle();

    /**
     * @brief Get the current bus configuration
     *
     * @return Reference to the current bus configuration
     */
    const Config &getConfig() const
    {
        return _config;
    }

    /**
     * @brief Configure DPI frame buffer number
     *
     * @param[in] num Number of frame buffers
     * @deprecated Use `configDPI_FrameBufferNumber()` instead
     */
    [[deprecated("Use `configDPI_FrameBufferNumber()` instead")]]
    void configDpiFrameBufferNumber(uint8_t num)
    {
        configDPI_FrameBufferNumber(num);
    }

    /**
     * @brief Get the DSI configuration
     *
     * @return DSI configuration
     * @deprecated Use `getConfig()` instead
     */
    [[deprecated("Use `getConfig()` instead")]]
    const esp_lcd_dsi_bus_config_t *getDsiConfig()
    {
        return getConfig().getHostFullConfig();
    }

    /**
     * @brief Get the DPI configuration
     *
     * @return DPI configuration
     * @deprecated Use `getConfig()` instead
     */
    [[deprecated("Use `getConfig()` instead")]]
    const esp_lcd_dpi_panel_config_t *getDpiConfig()
    {
        return getConfig().getRefreshPanelFullConfig();
    }

private:
    /**
     * @brief Get mutable reference to refresh panel full configuration
     *
     * Converts partial configuration to full configuration if necessary
     *
     * @return Reference to refresh panel full configuration
     */
    Config::RefreshPanelFullConfig &getRefreshPanelFullConfig();

    Config _config = {};                              ///< MIPI-DSI bus configuration
    std::shared_ptr<HostDSI> _host = nullptr;         ///< MIPI-DSI host instance
    esp_ldo_channel_handle_t _phy_ldo_handle = nullptr; ///< PHY LDO handle
};

} // namespace esp_panel::drivers

/**
 * @brief Alias for backward compatibility
 *
 * @deprecated Use `esp_panel::drivers::BusDSI` instead
 */
using ESP_PanelBusDSI [[deprecated("Use `esp_panel::drivers::BusDSI` instead")]] = esp_panel::drivers::BusDSI;

#endif // SOC_MIPI_DSI_SUPPORTED
