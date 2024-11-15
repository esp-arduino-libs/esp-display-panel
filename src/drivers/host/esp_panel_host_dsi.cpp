/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"
#if SOC_MIPI_DSI_SUPPORTED
#include "esp_panel_utils.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_panel_host_dsi.hpp"

namespace esp_panel::drivers {

BusHost_DSI::~BusHost_DSI()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (!checkIsBegun()) {
        goto end;
    }

    {
        int id = getID();
        ESP_UTILS_CHECK_ERROR_EXIT(
            esp_lcd_del_dsi_bus(_host_handle), "Delete DSI host(%d) failed", id
        );
        ESP_UTILS_LOGI("Delete DSI host(%d)", id);
    }

end:
    flags.is_begun = false;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusHost_DSI::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIsBegun()) {
        goto end;
    }

    {
        int id = getID();
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_new_dsi_bus(&config, &_host_handle), false, "Initialize DSI host(%d) failed", id
        );
        ESP_UTILS_LOGI("Initialize DSI host(%d)", id);
    }

    flags.is_begun = true;

end:
    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusHost_DSI::calibrateConfig(const esp_lcd_dsi_bus_config_t &config)
{
    if (memcmp(&config, &this->config, sizeof(esp_lcd_dsi_bus_config_t))) {
        ESP_UTILS_LOGD(
            "Original config: bus_id(%d), num_data_lanes(%d), phy_clk_src(%d), lane_bit_rate_mbps(%d)",
            this->config.bus_id, this->config.num_data_lanes, static_cast<int>(this->config.phy_clk_src),
            static_cast<int>(this->config.lane_bit_rate_mbps)
        );
        ESP_UTILS_LOGD(
            "New config: bus_id(%d), num_data_lanes(%d), phy_clk_src(%d), lane_bit_rate_mbps(%d)",
            config.bus_id, config.num_data_lanes, static_cast<int>(config.phy_clk_src),
            static_cast<int>(config.lane_bit_rate_mbps)
        );
        ESP_UTILS_CHECK_FALSE_RETURN(false, false, "Config mismatch");
    }

    return true;
}

} // namespace esp_panel::drivers

#endif // SOC_MIPI_DSI_SUPPORTED
