/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_panel_utils.h"
#include "drivers/host/esp_panel_host_i2c.hpp"
#include "esp_panel_bus_i2c.hpp"

namespace esp_panel::drivers {

void BusI2C::Config::convertPartialToFull()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (!skip_init_host && std::holds_alternative<HostPartialConfig>(host)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printHostConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<HostPartialConfig>(host);
        host = i2c_config_t{
            .mode = I2C_MODE_MASTER,
            .sda_io_num = config.sda_io_num,
            .scl_io_num = config.scl_io_num,
            .sda_pullup_en = config.sda_pullup_en,
            .scl_pullup_en = config.scl_pullup_en,
            .master = {
                .clk_speed = static_cast<uint32_t>(config.clk_speed),
            },
            .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
        };
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::Config::printGeneralConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_LOGI(
        "\n\t{General config}"
        "\n\t\t-> [host_id]: %d"
        "\n\t\t-> [skip_init_host]: %d"
        , host_id
        , skip_init_host
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::Config::printHostConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<HostFullConfig>(host)) {
        auto &config = std::get<HostFullConfig>(host);
        ESP_UTILS_LOGI(
            "\n\t{Full host config}:"
            "\n\t\t-> [mode]: %d"
            "\n\t\t-> [sda_io_num]: %d"
            "\n\t\t-> [scl_io_num]: %d"
            "\n\t\t-> [sda_pullup_en]: %d"
            "\n\t\t-> [scl_pullup_en]: %d"
            "\n\t\t-> [master.clk_speed]: %d"
            "\n\t\t-> [clk_flags]: %d"
            , config.mode
            , config.sda_io_num
            , config.scl_io_num
            , config.sda_pullup_en
            , config.scl_pullup_en
            , config.master.clk_speed
            , config.clk_flags
        );
    } else {
        auto &config = std::get<HostPartialConfig>(host);
        ESP_UTILS_LOGI(
            "\n\t{Partial host config}"
            "\n\t\t-> [sda_io_num]: %d"
            "\n\t\t-> [scl_io_num]: %d"
            "\n\t\t-> [sda_pullup_en]: %d"
            "\n\t\t-> [scl_pullup_en]: %d"
            "\n\t\t-> [clk_speed]: %d"
            , config.sda_io_num
            , config.scl_io_num
            , config.sda_pullup_en
            , config.scl_pullup_en
            , config.clk_speed
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::Config::printControlPanelConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_LOGI(
        "\n\t{Full control panel config}"
        "\n\t\t-> [dev_addr]: 0x%02X"
        "\n\t\t-> [control_phase_bytes]: %d"
        "\n\t\t-> [dc_bit_offset]: %d"
        "\n\t\t-> [lcd_cmd_bits]: %d"
        "\n\t\t-> [lcd_param_bits]: %d"
        "\n\t\t-> {flags}"
        "\n\t\t\t-> [dc_low_on_data]: %d"
        "\n\t\t\t-> [disable_control_phase]: %d"
        , control_panel.dev_addr
        , control_panel.control_phase_bytes
        , control_panel.dc_bit_offset
        , control_panel.lcd_cmd_bits
        , control_panel.lcd_param_bits
        , control_panel.flags.dc_low_on_data
        , control_panel.flags.disable_control_phase
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

BusI2C::~BusI2C()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_PullupEnable(bool sda_pullup_en, bool scl_pullup_en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: sda_pullup_en(%d), scl_pullup_en(%d)", sda_pullup_en, scl_pullup_en);
    auto &host_config = getHostFullConfig();
    host_config.sda_pullup_en = sda_pullup_en ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    host_config.scl_pullup_en = scl_pullup_en ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_FreqHz(uint32_t hz)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: hz(%d)", hz);
    getHostFullConfig().master.clk_speed = hz;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_Address(uint32_t address)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: address(0x%02X)", address);
    getControlPanelFullConfig().dev_addr = address;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_CtrlPhaseBytes(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().control_phase_bytes = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_DC_BitOffset(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().dc_bit_offset = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_CommandBits(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().lcd_cmd_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_ParamBits(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    getControlPanelFullConfig().lcd_param_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusI2C::configI2C_Flags(bool dc_low_on_data, bool disable_control_phase)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!isOverState(State::INIT), "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: dc_low_on_data(%d), disable_control_phase(%d)", dc_low_on_data, disable_control_phase);
    auto &control_panel_config = getControlPanelFullConfig();
    control_panel_config.flags.dc_low_on_data = dc_low_on_data;
    control_panel_config.flags.disable_control_phase = disable_control_phase;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusI2C::init()
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
        auto host_config = config.getHostFullConfig();
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get I2C host(%d) host config failed", config.host_id);

        _host = HostI2C::getInstance(config.host_id, *host_config);
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get I2C host(%d) instance failed", config.host_id);

        ESP_UTILS_LOGD("Get I2C host(%d) instance", config.host_id);
    }

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::begin()
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
        ESP_UTILS_LOGD("Begin I2C host(%d)", config.host_id);
    }

    // Create the control panel
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 2, 0)
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_i2c(
            static_cast<esp_lcd_i2c_bus_handle_t>(config.host_id), &getControlPanelFullConfig(),
            &control_panel
        ), false, "create control panel failed"
    );
#else
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_i2c_v1(
            static_cast<esp_lcd_i2c_bus_handle_t>(config.host_id), &getControlPanelFullConfig(),
            &control_panel
        ), false, "create control panel failed"
    );
#endif // ESP_IDF_VERSION
    ESP_UTILS_LOGD("Create control panel @%p", control_panel);

    setState(State::BEGIN);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusI2C::del()
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
            HostI2C::tryReleaseInstance(config.host_id), false, "Release I2C host(%d) failed",
            config.host_id
        );
    }

    setState(State::DEINIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
