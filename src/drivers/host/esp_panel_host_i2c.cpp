/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "esp_panel_host_i2c.hpp"

namespace esp_panel::drivers {

BusHost_I2C::~BusHost_I2C()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (!checkIsBegun()) {
        goto end;
    }

    {
        int id = getID();
        ESP_UTILS_CHECK_ERROR_EXIT(
            i2c_driver_delete(static_cast<i2c_port_t>(id)), "Delete I2C host(%d) failed", id
        );
        ESP_UTILS_LOGI("Delete I2C host(%d)", id);
    }

end:
    flags.is_begun = false;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusHost_I2C::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIsBegun()) {
        goto end;
    }

    {
        int id = getID();
        ESP_UTILS_CHECK_ERROR_RETURN(
            i2c_param_config(static_cast<i2c_port_t>(id), &config), false, "I2C param config failed"
        );
        ESP_UTILS_CHECK_ERROR_RETURN(
            i2c_driver_install(static_cast<i2c_port_t>(id), config.mode, 0, 0, 0), false, "I2C driver install failed"
        );
        ESP_UTILS_LOGI("Initialize I2C host(%d)", id);
    }

    flags.is_begun = true;

end:
    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusHost_I2C::calibrateConfig(const i2c_config_t &config)
{
    if (memcmp(&config, &this->config, sizeof(i2c_config_t))) {
        ESP_UTILS_LOGD(
            "Original config: mode(%d), sda_io_num(%d), scl_io_num(%d), sda_pullup_en(%d), scl_pullup_en(%d),"
            "clk_speed(%d)", this->config.mode, this->config.sda_io_num, this->config.scl_io_num,
            this->config.sda_pullup_en, this->config.scl_pullup_en, this->config.master.clk_speed
        );
        ESP_UTILS_LOGD(
            "New config: mode(%d), sda_io_num(%d), scl_io_num(%d), sda_pullup_en(%d), scl_pullup_en(%d), clk_speed(%d)",
            config.mode, config.sda_io_num, config.scl_io_num, config.sda_pullup_en, config.scl_pullup_en,
            config.master.clk_speed
        );
        ESP_UTILS_CHECK_FALSE_RETURN(false, false, "Config mismatch");
    }

    return true;
}

} // namespace esp_panel::drivers
