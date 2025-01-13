/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/spi_master.h"
#include "esp_panel_host.hpp"

namespace esp_panel::drivers {

/**
 * @brief SPI bus host class
 */
class HostSPI : public Host<HostSPI, spi_bus_config_t, static_cast<int>(SPI_HOST_MAX)> {
public:
    /* Add friend class to allow them to access the private member */
    template <typename U>
    friend struct esp_utils::GeneralMemoryAllocator;    // To access `HostSPI()`
    template <class Instance, typename Config, int N>
    friend class Host;                                  // To access `del()`, `calibrateConfig()`

    /**
     * @brief Destroy the host
     */
    ~HostSPI() override;

    /**
     * @brief Startup the host
     *
     * @return `true` if successful, `false` otherwise
     */
    bool begin() override;

private:
    /**
     * @brief Private constructor to prevent direct instantiation
     *
     * @param[in] id Host ID
     * @param[in] config Host configuration
     */
    HostSPI(int id, const spi_bus_config_t &config):
        Host<HostSPI, spi_bus_config_t, static_cast<int>(SPI_HOST_MAX)>(id, config) {}

    /**
     * @brief Calibrate configuration when host already exists
     *
     * @param[in] config New configuration
     * @return `true` if successful, `false` otherwise
     */
    bool calibrateConfig(const spi_bus_config_t &config) override;
};

} // namespace esp_panel::drivers
