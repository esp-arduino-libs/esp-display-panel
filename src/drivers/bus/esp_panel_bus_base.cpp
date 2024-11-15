/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include "esp_panel_utils.h"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

bool Bus::readRegisterData(uint32_t address, void *data, uint32_t data_size)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIO_HandleValid(), false, "Not begun");

    ESP_UTILS_LOGD("Param: address(0x%" PRIx32 "), data(%p), data_size(%d)", address, data, data_size);
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_io_rx_param(io_handle, address, data, data_size), false, "Read register failed",
        address
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Bus::writeRegisterData(uint32_t address, const void *data, uint32_t data_size)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIO_HandleValid(), false, "Not begun");

    ESP_UTILS_LOGD("Param: address(0x%" PRIx32 "), data(%p), data_size(%d)", address, data, data_size);
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_io_tx_param(io_handle, address, data, data_size), false, "Write register failed",
        address
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Bus::writeColorData(uint32_t address, const void *color, uint32_t color_size)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIO_HandleValid(), false, "Not begun");

    ESP_UTILS_LOGD("Param: address(0x%" PRIx32 "), color(%p), color_size(%d)", address, color, color_size);
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_io_tx_param(io_handle, address, color, color_size), false, "Write color failed",
        address
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Bus::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIO_HandleValid(), false, "Not begun");

    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_io_del(io_handle), false, "Delete panel IO failed");
    ESP_UTILS_LOGD("Delete panel IO @%p", io_handle);
    io_handle = nullptr;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
