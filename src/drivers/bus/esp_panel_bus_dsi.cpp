/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"
#if SOC_MIPI_DSI_SUPPORTED
#include <stdlib.h>
#include <string.h>
#include "esp_panel_utils.h"
#include "drivers/host/esp_panel_host_dsi.hpp"
#include "esp_panel_bus_dsi.hpp"

namespace esp_panel::drivers {

void BusDSI::Config::convertPartialToFull()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<HostPartialConfig>(host)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printHostConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<HostPartialConfig>(host);
        host = HostFullConfig{
            .bus_id = HOST_ID_DEFAULT,
            .num_data_lanes = static_cast<uint8_t>(config.num_data_lanes),
            .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,
            .lane_bit_rate_mbps = static_cast<uint32_t>(config.lane_bit_rate_mbps),
        };
    }

    if (std::holds_alternative<RefreshPanelPartialConfig>(refresh_panel)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printRefreshPanelConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<RefreshPanelPartialConfig>(refresh_panel);
        refresh_panel = RefreshPanelFullConfig{
            .virtual_channel = 0,
            .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,
            .dpi_clock_freq_mhz = static_cast<uint32_t>(config.dpi_clock_freq_mhz),
            .pixel_format = (config.bits_per_pixel == 16) ?  LCD_COLOR_PIXEL_FORMAT_RGB565 :
            ((config.bits_per_pixel == 18) ? LCD_COLOR_PIXEL_FORMAT_RGB666 :
             LCD_COLOR_PIXEL_FORMAT_RGB888),
            .num_fbs = 1,
            .video_timing = {
                .h_size = static_cast<uint32_t>(config.h_size),
                .v_size = static_cast<uint32_t>(config.v_size),
                .hsync_pulse_width = static_cast<uint32_t>(config.hsync_pulse_width),
                .hsync_back_porch = static_cast<uint32_t>(config.hsync_back_porch),
                .hsync_front_porch = static_cast<uint32_t>(config.hsync_front_porch),
                .vsync_pulse_width = static_cast<uint32_t>(config.vsync_pulse_width),
                .vsync_back_porch = static_cast<uint32_t>(config.vsync_back_porch),
                .vsync_front_porch = static_cast<uint32_t>(config.vsync_front_porch),
            },
            .flags = {
                .use_dma2d = true,
            },
        };
    }

    if (std::holds_alternative<PHY_LDO_PartialConfig>(phy_ldo)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printPHY_LDO_Config();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<PHY_LDO_PartialConfig>(phy_ldo);
        phy_ldo = PHY_LDO_FullConfig{
            .chan_id = static_cast<int>(config.chan_id),
            .voltage_mv = PHY_LDO_VOLTAGE_MV_DEFAULT,
        };
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusDSI::Config::printHostConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<HostFullConfig>(host)) {
        auto &config = std::get<HostFullConfig>(host);
        ESP_UTILS_LOGI(
            "\n\t{Full host config}:"
            "\n\t\t-> [bus_id]: %d"
            "\n\t\t-> [num_data_lanes]: %d"
            "\n\t\t-> [phy_clk_src]: %d"
            "\n\t\t-> [lane_bit_rate_mbps]: %d"
            , config.bus_id
            , config.num_data_lanes
            , static_cast<int>(config.phy_clk_src)
            , config.lane_bit_rate_mbps
        );
    } else {
        auto &config = std::get<HostPartialConfig>(host);
        ESP_UTILS_LOGI(
            "\n\t{Partial host config}"
            "\n\t\t-> [num_data_lanes]: %d"
            "\n\t\t-> [lane_bit_rate_mbps]: %d"
            , config.num_data_lanes
            , config.lane_bit_rate_mbps
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusDSI::Config::printControlPanelConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_LOGI(
        "\n\t{Full control panel config}"
        "\n\t\t-> [virtual_channel]: %d"
        "\n\t\t-> [lcd_cmd_bits]: %d"
        "\n\t\t-> [lcd_param_bits]: %d"
        , control_panel.virtual_channel
        , control_panel.lcd_cmd_bits
        , control_panel.lcd_param_bits
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusDSI::Config::printRefreshPanelConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<RefreshPanelFullConfig>(refresh_panel)) {
        auto &config = std::get<RefreshPanelFullConfig>(refresh_panel);
        // Here to split the log to avoid the log buffer overflow
        ESP_UTILS_LOGI(
            "\n\t{Full refresh panel config}"
            "\n\t\t-> [virtual_channel]: %d"
            "\n\t\t-> [dpi_clk_src]: %d"
            "\n\t\t-> [dpi_clock_freq_mhz]: %d"
            "\n\t\t-> [pixel_format]: %d"
            "\n\t\t-> [num_fbs]: %d"
            , config.virtual_channel
            , static_cast<int>(config.dpi_clk_src)
            , config.dpi_clock_freq_mhz
            , static_cast<int>(config.pixel_format)
            , config.num_fbs
        );
        ESP_UTILS_LOGI(
            "\n\t\t-> {video_timing}"
            "\n\t\t\t-> [h_size]: %d"
            "\n\t\t\t-> [v_size]: %d"
            "\n\t\t\t-> [hsync_pulse_width]: %d"
            "\n\t\t\t-> [hsync_back_porch]: %d"
            "\n\t\t\t-> [hsync_front_porch]: %d"
            "\n\t\t\t-> [vsync_pulse_width]: %d"
            "\n\t\t\t-> [vsync_back_porch]: %d"
            "\n\t\t\t-> [vsync_front_porch]: %d"
            , config.video_timing.h_size
            , config.video_timing.v_size
            , config.video_timing.hsync_pulse_width
            , config.video_timing.hsync_back_porch
            , config.video_timing.hsync_front_porch
            , config.video_timing.vsync_pulse_width
            , config.video_timing.vsync_back_porch
            , config.video_timing.vsync_front_porch
        );
        ESP_UTILS_LOGI(
            "\n\t\t-> {flags}"
            "\n\t\t\t-> [use_dma2d]: %d"
            "\n\t\t\t-> [disable_lp]: %d"
            , config.flags.use_dma2d
            , config.flags.disable_lp
        );
    } else {
        auto &config = std::get<RefreshPanelPartialConfig>(refresh_panel);
        ESP_UTILS_LOGI(
            "\n\t{Partial refresh panel config}"
            "\n\t\t-> [dpi_clock_freq_mhz]: %d"
            "\n\t\t-> [bits_per_pixel]: %d"
            "\n\t\t-> [h_size]: %d"
            "\n\t\t-> [v_size]: %d"
            , config.dpi_clock_freq_mhz
            , config.bits_per_pixel
            , config.h_size
            , config.v_size
        );
        ESP_UTILS_LOGI(
            "\n\t\t-> [hsync_pulse_width]: %d"
            "\n\t\t-> [hsync_back_porch]: %d"
            "\n\t\t-> [hsync_front_porch]: %d"
            "\n\t\t-> [vsync_pulse_width]: %d"
            "\n\t\t-> [vsync_back_porch]: %d"
            "\n\t\t-> [vsync_front_porch]: %d"
            , config.hsync_pulse_width
            , config.hsync_back_porch
            , config.hsync_front_porch
            , config.vsync_pulse_width
            , config.vsync_back_porch
            , config.vsync_front_porch
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusDSI::Config::printPHY_LDO_Config() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<PHY_LDO_FullConfig>(phy_ldo)) {
        auto &config = std::get<PHY_LDO_FullConfig>(phy_ldo);
        ESP_UTILS_LOGI(
            "\n\t{Full PHY LDO config}"
            "\n\t\t-> [chan_id]: %d"
            "\n\t\t-> [voltage_mv]: %d"
            "\n\t\t-> {flags}"
            "\n\t\t\t-> [adjustable]: %d"
            "\n\t\t\t-> [owned_by_hw]: %d"
            , config.chan_id
            , config.voltage_mv
            , config.flags.adjustable
            , config.flags.owned_by_hw
        );
    } else {
        auto &config = std::get<PHY_LDO_PartialConfig>(phy_ldo);
        ESP_UTILS_LOGI(
            "\n\t{Partial PHY LDO config}"
            "\n\t\t-> [chan_id]: %d"
            , config.chan_id
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

const BusDSI::Config::HostFullConfig *BusDSI::Config::getHostFullConfig() const
{
    if (std::holds_alternative<HostPartialConfig>(host)) {
        return nullptr;
    }

    return &std::get<HostFullConfig>(host);
}

const BusDSI::Config::RefreshPanelFullConfig *BusDSI::Config::getRefreshPanelFullConfig() const
{
    if (std::holds_alternative<RefreshPanelPartialConfig>(refresh_panel)) {
        return nullptr;
    }

    return &std::get<RefreshPanelFullConfig>(refresh_panel);
}

const BusDSI::Config::PHY_LDO_FullConfig *BusDSI::Config::getPHY_LDO_Config() const
{
    if (std::holds_alternative<PHY_LDO_PartialConfig>(phy_ldo)) {
        return nullptr;
    }

    return &std::get<PHY_LDO_FullConfig>(phy_ldo);
}

BusDSI::~BusDSI()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusDSI::configDPI_FrameBufferNumber(uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", (int)num);
    getRefreshPanelFullConfig().num_fbs = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusDSI::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

    // Convert the partial configuration to full configuration
    _config.convertPartialToFull();
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
    _config.printHostConfig();
    _config.printControlPanelConfig();
    _config.printRefreshPanelConfig();
    _config.printPHY_LDO_Config();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG

    // Get the host instance if not skipped
    auto host_config = getConfig().getHostFullConfig();
    _host = HostDSI::getInstance(host_config->bus_id, *host_config);
    ESP_UTILS_CHECK_NULL_RETURN(
        _host, false, "Get DSI host(%d) instance failed", host_config->bus_id
    );
    ESP_UTILS_LOGD("Get DSI host(%d) instance", host_config->bus_id);

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusDSI::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::BEGIN), false, "Already begun");

    // Initialize the bus if not initialized
    if (!isOverState(State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    // Turn on the power for MIPI DSI PHY
    auto phy_ldo_config = getConfig().getPHY_LDO_Config();
    if (phy_ldo_config->chan_id >= 0) {
        // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_ldo_acquire_channel(phy_ldo_config, &_phy_ldo_handle), false, "Acquire LDO channel failed"
        );
        ESP_UTILS_LOGD("MIPI DSI PHY (LDO %d) Powered on", phy_ldo_config->chan_id);
    }

    // Startup the host
    auto host_config = getConfig().getHostFullConfig();
    ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "init host(%d) failed", host_config->bus_id);
    ESP_UTILS_LOGD("Begin DSI host(%d)", host_config->bus_id);

    // Create the control panel
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_dbi(getHostHandle(), &getConfig().control_panel, &control_panel), false,
        "create control panel failed"
    );
    ESP_UTILS_LOGD("Create control panel @%p", control_panel);

    setState(State::BEGIN);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusDSI::del()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    // Delete the control panel if valid
    if (isControlPanelValid()) {
        ESP_UTILS_CHECK_FALSE_RETURN(delControlPanel(), false, "Delete control panel failed");
    }

    // Release the host instance if valid
    if (_host != nullptr) {
        _host = nullptr;
        auto host_config = getConfig().getHostFullConfig();
        ESP_UTILS_CHECK_FALSE_RETURN(
            HostDSI::tryReleaseInstance(host_config->bus_id), false, "Release DSI host(%d) failed", host_config->bus_id
        );
    }

    // Turn off the power for MIPI DSI PHY if valid
    if (_phy_ldo_handle != nullptr) {
        auto phy_ldo_config = getConfig().getPHY_LDO_Config();
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_ldo_release_channel(_phy_ldo_handle), false, "Release LDO channel(%d) failed", phy_ldo_config->chan_id
        );
        _phy_ldo_handle = nullptr;
        ESP_UTILS_LOGD("MIPI DSI PHY (LDO %d) Powered off", phy_ldo_config->chan_id);
    }

    setState(State::DEINIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

/* Implement the function here to avoid error: invalid use of incomplete type `HostDSI` */
esp_lcd_dsi_bus_handle_t BusDSI::getHostHandle()
{
    return (_host == nullptr) ? nullptr : static_cast<esp_lcd_dsi_bus_handle_t>(_host->getHandle());
}

BusDSI::Config::RefreshPanelFullConfig &BusDSI::getRefreshPanelFullConfig()
{
    if (std::holds_alternative<Config::RefreshPanelPartialConfig>(_config.refresh_panel)) {
        _config.convertPartialToFull();
    }

    return std::get<Config::RefreshPanelFullConfig>(_config.refresh_panel);
}

} // namespace esp_panel::drivers

#endif // SOC_MIPI_DSI_SUPPORTED
