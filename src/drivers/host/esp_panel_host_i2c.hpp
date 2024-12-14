/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/i2c.h"
#include "esp_panel_host_template.hpp"

namespace esp_panel::drivers {

/**
 * @brief The I2C bus host class
 *
 */
class HostI2C : public Host<HostI2C, i2c_config_t, static_cast<int>(I2C_NUM_MAX)> {
public:
    /* Add friend class to allow them to access the private member */
    // To access `HostI2C()`
    template <typename U>
    friend struct esp_utils::GeneralMemoryAllocator;
    // To accdess `del()`, `calibrateConfig()`
    template <class Instance, typename Config, int N>
    friend class Host;

    /**
     * @brief Destroy the host
     *
     */
    ~HostI2C() override;

    /**
     * @brief Startup the host
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;

private:
    /* Make constructor private to prevent users from constructing it directly */
    HostI2C(int id, const i2c_config_t &config):
        Host<HostI2C, i2c_config_t, static_cast<int>(I2C_NUM_MAX)>(id, config) {}

    /**
     * @brief Calibrate the configuration when the host is already exist
     *
     * @param[in] config The new configuration
     *
     * @return true if success, otherwise false
     *
     */
    bool calibrateConfig(const i2c_config_t &config) override;
};

} // namespace esp_panel::drivers
