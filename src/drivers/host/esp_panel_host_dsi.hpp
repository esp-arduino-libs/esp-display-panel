/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"
#if SOC_MIPI_DSI_SUPPORTED
#include "hal/mipi_dsi_ll.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_panel_host_template.hpp"

namespace esp_panel::drivers {

/**
 * @brief The MIPI-DSI bus host class
 *
 */
class BusHost_DSI : public BusHost<BusHost_DSI, esp_lcd_dsi_bus_config_t, MIPI_DSI_LL_NUM_BUS> {
public:
    /* Add friend class to allow them to access the private member */
    // To access `BusHost_DSI()`
    template <typename U>
    friend struct esp_utils::GeneralMemoryAllocator;
    // To accdess `del()`, `calibrateConfig()`
    template <class Instance, typename Config, int N>
    friend class BusHost;

    /**
     * @brief Destroy the host
     *
     */
    ~BusHost_DSI() override;

    /**
     * @brief Startup the host
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;

    /**
     * @brief Get the handle of the host
     *
     * @return The handle of the host
     *
     */
    esp_lcd_dsi_bus_handle_t getHandle(void)
    {
        return _host_handle;
    }

private:
    /* Make constructor private to prevent users from constructing it directly */
    BusHost_DSI(int id, const esp_lcd_dsi_bus_config_t &config):
        BusHost<BusHost_DSI, esp_lcd_dsi_bus_config_t, MIPI_DSI_LL_NUM_BUS>(id, config) {}

    /**
     * @brief Calibrate the configuration when the host is already exist
     *
     * @param[in] config The new configuration
     *
     * @return true if success, otherwise false
     *
     */
    bool calibrateConfig(const esp_lcd_dsi_bus_config_t &config) override;

    esp_lcd_dsi_bus_handle_t _host_handle = nullptr;
};

} // namespace esp_panel::drivers

#endif // SOC_MIPI_DSI_SUPPORTED
