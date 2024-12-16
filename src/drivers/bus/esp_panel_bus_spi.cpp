/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "hal/spi_ll.h"
#include "esp_panel_utils.h"
#include "drivers/host/esp_panel_host_spi.hpp"
#include "esp_panel_bus_spi.hpp"

namespace esp_panel::drivers {

void BusSPI::Config::convertPartialToFull()
{
    if (!skip_init_host && std::holds_alternative<HostPartialConfig>(host)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printHostConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<HostPartialConfig>(host);
        host = HostFullConfig{
            .mosi_io_num = config.mosi_io_num,
            .miso_io_num = config.miso_io_num,
            .sclk_io_num = config.sclk_io_num,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .data4_io_num = -1,
            .data5_io_num = -1,
            .data6_io_num = -1,
            .data7_io_num = -1,
            .max_transfer_sz = (SPI_LL_DMA_MAX_BIT_LEN >> 3),
            .flags = SPICOMMON_BUSFLAG_MASTER,
            .intr_flags = 0,
        };
    }

    if (std::holds_alternative<ControlPanelPartialConfig>(control_panel)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printControlPanelConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<ControlPanelPartialConfig>(control_panel);
        control_panel = ControlPanelFullConfig{
            .cs_gpio_num = config.cs_gpio_num,
            .dc_gpio_num = config.dc_gpio_num,
            .spi_mode = config.spi_mode,
            .pclk_hz = static_cast<unsigned int>(config.pclk_hz),
            .trans_queue_depth = 10,
            .on_color_trans_done = nullptr,
            .user_ctx = nullptr,
            .lcd_cmd_bits = config.lcd_cmd_bits,
            .lcd_param_bits = config.lcd_param_bits,
            .flags = {
                .dc_low_on_data = 0,
                .octal_mode = 0,
                .quad_mode = 0,
                .sio_mode = 0,
                .lsb_first = 0,
                .cs_high_active = 0,
            },
        };
    }
}

void BusSPI::Config::printHostConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<HostFullConfig>(host)) {
        auto &config = std::get<HostFullConfig>(host);
        ESP_UTILS_LOGI(
            "\n\t{Full host config}:"
            "\n\t\t-> [mosi_io_num]: %d"
            "\n\t\t-> [miso_io_num]: %d"
            "\n\t\t-> [sclk_io_num]: %d"
            "\n\t\t-> [max_transfer_sz]: %d"
            "\n\t\t-> [flags]: %d"
            "\n\t\t-> [intr_flags]: %d"
            , config.mosi_io_num
            , config.miso_io_num
            , config.sclk_io_num
            , config.max_transfer_sz
            , config.flags
            , config.intr_flags
        );
    } else {
        auto &config = std::get<HostPartialConfig>(host);
        ESP_UTILS_LOGI(
            "\n\t{Partial host config}"
            "\n\t\t-> [mosi_io_num]: %d"
            "\n\t\t-> [miso_io_num]: %d"
            "\n\t\t-> [sclk_io_num]: %d"
            , config.mosi_io_num
            , config.miso_io_num
            , config.sclk_io_num
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusSPI::Config::printControlPanelConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<ControlPanelFullConfig>(control_panel)) {
        auto &config = std::get<ControlPanelFullConfig>(control_panel);
        // Here to split the log to avoid the log buffer overflow
        ESP_UTILS_LOGI(
            "\n\t{Full control panel config}"
            "\n\t\t-> [cs_gpio_num]: %d"
            "\n\t\t-> [dc_gpio_num]: %d"
            "\n\t\t-> [spi_mode]: %d"
            "\n\t\t-> [pclk_hz]: %d"
            "\n\t\t-> [trans_queue_depth]: %d"
            "\n\t\t-> [lcd_cmd_bits]: %d"
            "\n\t\t-> [lcd_param_bits]: %d"
            , config.cs_gpio_num
            , config.dc_gpio_num
            , config.spi_mode
            , config.pclk_hz
            , config.trans_queue_depth
            , config.lcd_cmd_bits
            , config.lcd_param_bits
        );
        ESP_UTILS_LOGI(
            "\n\t\t-> {flags}"
            "\n\t\t\t-> [dc_high_on_cmd]: %d"
            "\n\t\t\t-> [dc_low_on_data]: %d"
            "\n\t\t\t-> [dc_low_on_param]: %d"
            "\n\t\t\t-> [octal_mode]: %d"
            "\n\t\t\t-> [quad_mode]: %d"
            "\n\t\t\t-> [sio_mode]: %d"
            "\n\t\t\t-> [lsb_first]: %d"
            "\n\t\t\t-> [cs_high_active]: %d"
            , config.flags.dc_high_on_cmd
            , config.flags.dc_low_on_data
            , config.flags.dc_low_on_param
            , config.flags.octal_mode
            , config.flags.quad_mode
            , config.flags.sio_mode
            , config.flags.lsb_first
            , config.flags.cs_high_active
        );
    } else {
        auto &config = std::get<ControlPanelPartialConfig>(control_panel);
        ESP_UTILS_LOGI(
            "\n\t{Partial control panel config}"
            "\n\t\t-> [cs_gpio_num]: %d"
            "\n\t\t-> [dc_gpio_num]: %d"
            "\n\t\t-> [spi_mode]: %d"
            "\n\t\t-> [pclk_hz]: %d"
            "\n\t\t-> [lcd_cmd_bits]: %d"
            "\n\t\t-> [lcd_param_bits]: %d"
            , config.cs_gpio_num
            , config.dc_gpio_num
            , config.spi_mode
            , config.pclk_hz
            , config.lcd_cmd_bits
            , config.lcd_param_bits
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

BusSPI::~BusSPI()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusSPI::configSPI_Mode(uint8_t mode)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: mode(%d)", (int)mode);
    getControlPanelFullConfig().spi_mode = mode;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusSPI::configSPI_FreqHz(uint32_t hz)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: hz(%d)", (int)hz);
    getControlPanelFullConfig().pclk_hz = hz;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusSPI::configSPI_CommandBits(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", (int)num);
    getControlPanelFullConfig().lcd_cmd_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusSPI::configSPI_ParamBits(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", (int)num);
    getControlPanelFullConfig().lcd_param_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusSPI::configSPI_TransQueueDepth(uint8_t depth)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: depth(%d)", (int)depth);
    getControlPanelFullConfig().trans_queue_depth = depth;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusSPI::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

    // Convert the partial configuration to full configuration
    _config.convertPartialToFull();
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
    _config.printHostConfig();
    _config.printControlPanelConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG

    // Get the host instance if not skipped
    auto &config = getConfig();
    if (!config.skip_init_host) {
        _host = HostSPI::getInstance(config.host_id, *config.getHostFullConfig());
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get SPI host(%d) instance failed", config.host_id);
        ESP_UTILS_LOGD("Get SPI host(%d) instance", config.host_id);
    }

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusSPI::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::BEGIN), false, "Already begun");

    // Initialize the bus if not initialized
    if (!isOverState(State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    // Startup the host if not skipped
    auto &config = getConfig();
    if (_host != nullptr) {
        ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "init host(%d) failed", config.host_id);
        ESP_UTILS_LOGD("Begin SPI host(%d)", config.host_id);
    }

    // Create the control panel
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_spi(
            static_cast<esp_lcd_spi_bus_handle_t>(config.host_id), config.getControlPanelFullConfig(),
            &control_panel
        ), false, "create panel IO failed"
    );
    ESP_UTILS_LOGD("Create control panel @%p", control_panel);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    setState(State::BEGIN);

    return true;
}

bool BusSPI::del()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    // Delete the control panel if valid
    if (isControlPanelValid()) {
        ESP_UTILS_CHECK_FALSE_RETURN(delControlPanel(), false, "Delete control panel failed");
    }

    // Release the host instance if valid
    if (_host != nullptr) {
        _host = nullptr;
        auto &config = getConfig();
        ESP_UTILS_CHECK_FALSE_RETURN(
            HostSPI::tryReleaseInstance(config.host_id), false, "Release SPI host(%d) failed",
            config.host_id
        );
    }

    setState(State::DEINIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
