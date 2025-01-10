/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ESP_PanelLog.h"
#include "CHSC6540.h"

static const char *TAG = "CHSC6540_CPP";

ESP_PanelTouch_CHSC6540::ESP_PanelTouch_CHSC6540(ESP_PanelBus *bus, uint16_t width, uint16_t height,
        int rst_io, int int_io):
    ESP_PanelTouch(bus, width, height, rst_io, int_io)
{
}

ESP_PanelTouch_CHSC6540::ESP_PanelTouch_CHSC6540(ESP_PanelBus *bus, const esp_lcd_touch_config_t &config):
    ESP_PanelTouch(bus, config)
{
}

ESP_PanelTouch_CHSC6540::~ESP_PanelTouch_CHSC6540()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (handle == NULL) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete device failed");
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

bool ESP_PanelTouch_CHSC6540::begin(void)
{
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    ESP_PANEL_CHECK_ERR_RET(esp_lcd_touch_new_i2c_chsc6540(bus->getHandle(), &config, &handle), false, "New driver failed");

    return true;
}
