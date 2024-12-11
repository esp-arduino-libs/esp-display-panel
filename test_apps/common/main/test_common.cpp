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

#define TEST_LCD_ENABLE_ATTACH_CALLBACK     (0)
#define TEST_LCD_SHOW_TIME_MS               (5000)

#define TEST_TOUCH_ENABLE_ATTACH_CALLBACK   (0)
#define TEST_TOUCH_READ_POINTS_NUM          (5)
#define TEST_TOUCH_READ_TIME_MS             (3000)
#define TEST_TOUCH_READ_DELAY_MS            (30)

#define delay(x)     vTaskDelay(pdMS_TO_TICKS(x))

using namespace std;

static const char *TAG = "test_panel";

#if TEST_LCD_ENABLE_ATTACH_CALLBACK
IRAM_ATTR static bool onLcdRefreshFinishCallback(void *user_data)
{
    esp_rom_printf("Refresh finish callback\n");

    return false;
}
#endif

#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK && (ESP_PANEL_BOARD_TOUCH_INT_IO >= 0)
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

class Test {
public:
    int data[100 * 1024 * 1024];
};

#include <tuple>
#include <iostream>

TEST_CASE("Test board", "[board]")
{
    // esp_panel::Board board;
    // board.init();
    // board.begin();

    // while (1) {
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }

    // ESP_LOGI(TAG, "Board: %d", board.getLcdHeight());
// }

// TEST_CASE("Test component common drivers", "[common]")
// {
    shared_ptr<esp_panel::Board> panel = make_shared<esp_panel::Board>(esp_panel::BOARD_ESP32_S3_LCD_EV_BOARD_V1_5_CONFIG);
    // shared_ptr<esp_panel::Board> panel = make_shared<esp_panel::Board>();
    TEST_ASSERT_NOT_NULL_MESSAGE(panel, "Create panel object failed");

    ESP_LOGI(TAG, "Initialize display panel");
    TEST_ASSERT_TRUE_MESSAGE(panel->init(), "Panel init failed");
    TEST_ASSERT_TRUE_MESSAGE(panel->begin(), "Panel begin failed");

    ESP_PanelLcd *lcd = panel->getLcd();
    ESP_PanelTouch *touch = panel->getTouch();
    ESP_PanelBacklight *backlight = panel->getBacklight();

    if (backlight != nullptr) {
        ESP_LOGI(TAG, "Turn off the backlight");
        backlight->off();
    } else {
        ESP_LOGI(TAG, "Backlight is not available");
    }

    if (lcd != nullptr) {
#if TEST_LCD_ENABLE_ATTACH_CALLBACK
        TEST_ASSERT_TRUE_MESSAGE(
            lcd->attachRefreshFinishCallback(onLcdRefreshFinishCallback, NULL), "Attach refresh callback failed"
        );
#endif
        ESP_LOGI(TAG, "Draw color bar from top to bottom, the order is B - G - R");
        TEST_ASSERT_TRUE_MESSAGE(
            lcd->colorBarTest(panel->getLcdWidth(), panel->getLcdHeight()), "LCD color bar test failed"
        );
    } else {
        ESP_LOGI(TAG, "LCD is not available");
    }

    if (backlight != nullptr) {
        ESP_LOGI(TAG, "Turn on the backlight");
        TEST_ASSERT_TRUE_MESSAGE(backlight->on(), "Backlight on failed");
    }

    if (lcd != nullptr) {
        ESP_LOGI(TAG, "Wait for %d ms to show the color bar", TEST_LCD_SHOW_TIME_MS);
        vTaskDelay(pdMS_TO_TICKS(TEST_LCD_SHOW_TIME_MS));
    }

    panel->del();

//     if (touch != nullptr) {
// #if TEST_LCD_ENABLE_ATTACH_CALLBACK && (ESP_PANEL_BOARD_TOUCH_INT_IO >= 0)
//         TEST_ASSERT_TRUE_MESSAGE(
//             touch->attachInterruptCallback(onTouchInterruptCallback, NULL), "Attach touch interrupt callback failed"
//         );
// #endif
//         uint32_t t = 0;
//         ESP_PanelTouchPoint point[TEST_TOUCH_READ_POINTS_NUM];
//         int read_touch_result = 0;

//         ESP_LOGI(TAG, "Reading touch_device point...");
//         while (t++ < TEST_TOUCH_READ_TIME_MS / TEST_TOUCH_READ_DELAY_MS) {
//             read_touch_result = touch->readPoints(point, TEST_TOUCH_READ_POINTS_NUM, TEST_TOUCH_READ_DELAY_MS);
//             if (read_touch_result > 0) {
//                 for (int i = 0; i < read_touch_result; i++) {
//                     ESP_LOGI(TAG, "Touch point(%d): x %d, y %d, strength %d\n", i, point[i].x, point[i].y, point[i].strength);
//                 }
//             } else if (read_touch_result < 0) {
//                 ESP_LOGE(TAG, "Read touch_device point failed");
//             }
//             if (!touch->isInterruptEnabled()) {
//                 delay(TEST_TOUCH_READ_DELAY_MS);
//             }
//         }
//     } else {
//         ESP_LOGI(TAG, "Touch is not available");
//     }
}
