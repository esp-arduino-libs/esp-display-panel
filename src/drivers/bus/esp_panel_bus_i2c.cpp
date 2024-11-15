/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_panel_utils.h"
#include "drivers/host/esp_panel_host_i2c.hpp"
#include "esp_panel_bus_i2c.hpp"

namespace esp_panel::drivers {

Bus_I2C::~Bus_I2C()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cPullupEnable(bool sda_pullup_en, bool scl_pullup_en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: sda_pullup_en(%d), scl_pullup_en(%d)", sda_pullup_en, scl_pullup_en);
    _host_config.sda_pullup_en = sda_pullup_en ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    _host_config.scl_pullup_en = scl_pullup_en ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cFreqHz(uint32_t hz)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: hz(%d)", hz);
    _host_config.master.clk_speed = hz;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cAddress(uint32_t address)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: address(0x%02X)", address);
    _io_config.dev_addr = address;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cCtrlPhaseBytes(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    _io_config.control_phase_bytes = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cDcBitOffset(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    _io_config.dc_bit_offset = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cCommandBits(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    _io_config.lcd_cmd_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cParamBits(uint32_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    _io_config.lcd_param_bits = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_I2C::configI2cFlags(bool dc_low_on_data, bool disable_control_phase)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: dc_low_on_data(%d), disable_control_phase(%d)", dc_low_on_data, disable_control_phase);
    _io_config.flags.dc_low_on_data = dc_low_on_data;
    _io_config.flags.disable_control_phase = disable_control_phase;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool Bus_I2C::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "Already initialized");

    if (!_flags.skip_init_host) {
        _host = BusHost_I2C::getInstance(_host_id, _host_config);
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get I2C host(%d) instance failed", _host_id);
        ESP_UTILS_LOGD("Get I2C host(%d) instance", _host_id);
    }

    _flags.is_init = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Bus_I2C::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "Already begun");

    if (!checkIsInit()) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    if (_host != nullptr) {
        ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "init host(%d) failed", _host_id);
        ESP_UTILS_LOGD("Begin I2C host(%d)", _host_id);
    }

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 2, 0)
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)_host_id, &_io_config, &io_handle), false,
        "Create panel IO failed"
    );
#else
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_i2c_v1((esp_lcd_i2c_bus_handle_t)_host_id, &_io_config, &io_handle), false,
        "Create panel IO failed"
    );
#endif
    ESP_UTILS_LOGD("Create panel IO @%p", io_handle);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Bus_I2C::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIO_HandleValid()) {
        ESP_UTILS_CHECK_FALSE_RETURN(Bus::del(), false, "Delete base bus failed");
    }

    if (checkIsInit()) {
        if (_host != nullptr) {
            _host = nullptr;
            ESP_UTILS_CHECK_FALSE_RETURN(
                BusHost_I2C::tryReleaseInstance(_host_id), false, "Release I2C host(%d) failed", _host_id
            );
        }
        _flags.is_init = false;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
