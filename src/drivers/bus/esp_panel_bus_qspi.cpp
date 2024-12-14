/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "drivers/host/esp_panel_host_spi.hpp"
#include "esp_panel_bus_qspi.hpp"

namespace esp_panel::drivers {

BusQSPI::~BusQSPI()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusQSPI::configQspiMode(uint8_t mode)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: mode(%d)", (int)mode);
    _io_config.spi_mode = mode;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusQSPI::configQspiFreqHz(uint32_t hz)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_LOGD("Param: hz(%d)", (int)hz);
    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    _io_config.pclk_hz = hz;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void BusQSPI::configQspiTransQueueDepth(uint8_t depth)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_LOGD("Param: depth(%d)", (int)depth);
    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `init()`");

    _io_config.trans_queue_depth = depth;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusQSPI::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "Already initialized");

    if (!_flags.skip_init_host) {
        _host = HostSPI::getInstance(_host_id, _host_config);
        ESP_UTILS_CHECK_NULL_RETURN(_host, false, "Get SPI host(%d) instance failed", _host_id);
        ESP_UTILS_LOGD("Get SPI host(%d) instance", _host_id);
    }

    _flags.is_init = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusQSPI::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "Already begun");

    if (!checkIsInit()) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    if (_host != nullptr) {
        ESP_UTILS_CHECK_FALSE_RETURN(_host->begin(), false, "init host(%d) failed", _host_id);
        ESP_UTILS_LOGD("Begin SPI host(%d)", _host_id);
    }

    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)_host_id, &_io_config, &io_handle), false,
        "Create panel IO failed"
    );
    ESP_UTILS_LOGD("Create panel IO @%p", io_handle);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusQSPI::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIO_HandleValid()) {
        ESP_UTILS_CHECK_FALSE_RETURN(Bus::del(), false, "Delete base bus failed");
    }

    if (checkIsInit()) {
        if (_host != nullptr) {
            _host = nullptr;
            ESP_UTILS_CHECK_FALSE_RETURN(
                HostSPI::tryReleaseInstance(_host_id), false, "Release SPI host(%d) failed", _host_id
            );
        }
        _flags.is_init = false;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
