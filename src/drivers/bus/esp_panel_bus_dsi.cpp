/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
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

Bus_DSI::~Bus_DSI()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void Bus_DSI::configDpiFrameBufferNumber(uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIO_HandleValid(), "This function should be called before `begin()`");

    ESP_UTILS_LOGD("Param: num(%d)", (int)num);
    _panel_config.num_fbs = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool Bus_DSI::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "Already initialized");

    _host = BusHost_DSI::getInstance(_host_config.bus_id, _host_config);
    ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get DSI host(%d) instance failed", _host_config.bus_id);
    ESP_UTILS_LOGD("Get DSI host(%d) instance", _host_config.bus_id);

    _flags.is_init = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Bus_DSI::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "Already begun");

    if (!checkIsInit()) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    if (_phy_ldo_config.chan_id >= 0) {
        // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_ldo_acquire_channel(&_phy_ldo_config, &_phy_ldo_handle), false, "Acquire LDO channel failed"
        );
        ESP_UTILS_LOGD("MIPI DSI PHY (LDO %d) Powered on", _phy_ldo_config.chan_id);
    }

    if (_host != nullptr) {
        ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "init host(%d) failed", _host_config.bus_id);
        ESP_UTILS_LOGD("Begin DSI host(%d)", _host_config.bus_id);
    }

    ESP_UTILS_LOGD("Param: host handle(@%p)", _host->getHandle());
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_dbi(_host->getHandle(), &_io_config, &io_handle), false, "Create panel IO failed"
    );
    ESP_UTILS_LOGD("Create panel IO @%p", io_handle);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Bus_DSI::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIO_HandleValid()) {
        ESP_UTILS_CHECK_FALSE_RETURN(Bus::del(), false, "Delete base bus failed");
    }

    if (_host != nullptr) {
        _host = nullptr;
        ESP_UTILS_CHECK_FALSE_RETURN(
            BusHost_DSI::tryReleaseInstance(_host_config.bus_id), false, "Release DSI host(%d) failed",
            _host_config.bus_id
        );
    }

    if (_phy_ldo_handle != nullptr) {
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_ldo_release_channel(_phy_ldo_handle), false, "Release LDO channel(%d) failed", _phy_ldo_config.chan_id
        );
        _phy_ldo_handle = nullptr;
        ESP_UTILS_LOGD("MIPI DSI PHY (LDO %d) Powered off", _phy_ldo_config.chan_id);
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

/* Implement the function here to avoid error: invalid use of incomplete type `BusHost_DSI` */
esp_lcd_dsi_bus_handle_t Bus_DSI::getDSI_Handle(void)
{
    return (_host == nullptr) ? nullptr : _host->getHandle();
}

} // namespace esp_panel::drivers

#endif /* SOC_MIPI_DSI_SUPPORTED */
