/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "esp_panel_library.hpp"
#include "lvgl.h"
#include "lvgl_port_v8.h"
#include "lv_demos.h"

#define TEST_DISPLAY_SHOW_TIME_MS   (5000)

#define delay(x)     vTaskDelay(pdMS_TO_TICKS(x))

using namespace std;

static const char *TAG = "test_lvgl_port";

TEST_CASE("Test panel lvgl port to show demo", "[panel][lvgl]")
{
    shared_ptr<ESP_Panel> panel = make_shared<ESP_Panel>();
    TEST_ASSERT_NOT_NULL_MESSAGE(panel, "Create panel object failed");

    ESP_LOGI(TAG, "Initialize display panel");
    TEST_ASSERT_TRUE_MESSAGE(panel->init(), "Panel init failed");
#if LVGL_PORT_AVOID_TEAR
    // When avoid tearing function is enabled, configure the bus according to the LVGL configuration
    ESP_PanelBus *lcd_bus = panel->getLcd()->getBus();
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
    static_cast<ESP_PanelBus_RGB *>(lcd_bus)->configRgbBounceBufferSize(LVGL_PORT_RGB_BOUNCE_BUFFER_SIZE);
    static_cast<ESP_PanelBus_RGB *>(lcd_bus)->configRgbFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI
    static_cast<ESP_PanelBus_DSI *>(lcd_bus)->configDpiFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
#endif
#endif
    TEST_ASSERT_TRUE_MESSAGE(panel->begin(), "Panel begin failed");

    ESP_LOGI(TAG, "Initialize LVGL");
    lvgl_port_init(panel->getLcd(), panel->getTouch());

    ESP_LOGI(TAG, "Create UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    // lv_demo_widgets();
    // lv_demo_benchmark();
    lv_demo_music();
    // lv_demo_stress();

    /* Release the mutex */
    lvgl_port_unlock();

    delay(TEST_DISPLAY_SHOW_TIME_MS);

    lvgl_port_deinit();
}
