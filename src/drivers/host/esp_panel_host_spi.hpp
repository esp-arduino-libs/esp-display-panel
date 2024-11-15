/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/spi_master.h"
#include "esp_panel_host_template.hpp"

namespace esp_panel::drivers {

/**
 * @brief The SPI bus host class
 *
 */
class BusHost_SPI : public BusHost<BusHost_SPI, spi_bus_config_t, static_cast<int>(SPI_HOST_MAX)> {
public:
    /* Add friend class to allow them to access the private member */
    // To access `BusHost_SPI()`
    template <typename U>
    friend struct esp_utils::GeneralMemoryAllocator;
    // To accdess `del()`, `calibrateConfig()`
    template <class Instance, typename Config, int N>
    friend class BusHost;

    /**
     * @brief Destroy the host
     *
     */
    ~BusHost_SPI() override;

    /**
     * @brief Startup the host
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;

private:
    /* Make constructor private to prevent users from constructing it directly */
    BusHost_SPI(int id, const spi_bus_config_t &config):
        BusHost<BusHost_SPI, spi_bus_config_t, static_cast<int>(SPI_HOST_MAX)>(id, config) {}

    /**
     * @brief Calibrate the configuration when the host is already exist
     *
     * @param[in] config The new configuration
     *
     * @return true if success, otherwise false
     *
     */
    bool calibrateConfig(const spi_bus_config_t &config) override;
};

} // namespace esp_panel::drivers
