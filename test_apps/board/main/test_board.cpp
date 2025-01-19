/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <memory>
#include <thread>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "esp_display_panel.hpp"

#define TEST_LCD_ENABLE_PRINT_FPS           (1)
#define TEST_LCD_PRINT_FPS_PERIOD_MS        (1000)
#define TEST_LCD_COLOR_BAR_SHOW_TIME_MS     (5000)

#define TEST_TOUCH_ENABLE_ATTACH_CALLBACK   (1)
#define TEST_TOUCH_READ_PERIOD_MS           (30)
#define TEST_TOUCH_READ_TIME_MS             (5000)

#define delay(x)     vTaskDelay(pdMS_TO_TICKS(x))

using namespace std;
using namespace esp_panel;

static const char *TAG = "test_board";

#if TEST_LCD_ENABLE_PRINT_FPS
#define TEST_LCD_FPS_COUNT_MAX  (100)
#ifndef millis
#define millis()                (esp_timer_get_time() / 1000)
#endif

DRAM_ATTR int frame_count = 0;
DRAM_ATTR int fps = 0;
DRAM_ATTR long start_time = 0;

IRAM_ATTR bool onLCD_VsyncCallback(void *user_data)
{
    long frame_start_time = *(long *)user_data;
    if (frame_start_time == 0) {
        (*(long *)user_data) = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= TEST_LCD_FPS_COUNT_MAX) {
        fps = TEST_LCD_FPS_COUNT_MAX * 1000 / (millis() - frame_start_time);
        frame_count = 0;
        (*(long *)user_data) = millis();
    }

    return false;
}
#endif

#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

TEST_CASE("Test board", "[board]")
{
    shared_ptr<Board> board = make_shared<Board>();
    TEST_ASSERT_NOT_NULL_MESSAGE(board, "Create board object failed");

    ESP_LOGI(TAG, "Initialize display panel");
    TEST_ASSERT_TRUE_MESSAGE(board->init(), "Board init failed");
    TEST_ASSERT_TRUE_MESSAGE(board->begin(), "Board begin failed");

    auto lcd = board->getLCD();
    auto touch = board->getTouch();
    auto backlight = board->getBacklight();

    if (backlight != nullptr) {
        ESP_LOGI(TAG, "Turn off the backlight");
        backlight->off();
    } else {
        ESP_LOGI(TAG, "Backlight is not available");
    }

    thread lcd_thread;
    if (lcd != nullptr) {
#if TEST_LCD_ENABLE_PRINT_FPS
        TEST_ASSERT_TRUE_MESSAGE(
            lcd->attachRefreshFinishCallback(onLCD_VsyncCallback, (void *)&start_time), "Attach refresh callback failed"
        );
#endif

        ESP_LOGI(TAG, "Draw color bar from top left to bottom right, the order is B - G - R");
        auto width = board->getConfig().width;
        auto height = board->getConfig().height;
        TEST_ASSERT_TRUE_MESSAGE(lcd->colorBarTest(width, height), "LCD color bar test failed");

        lcd_thread = std::thread([&]() {
#if TEST_LCD_ENABLE_PRINT_FPS
            ESP_LOGI(TAG, "Wait for %d ms to show the color bar", TEST_LCD_COLOR_BAR_SHOW_TIME_MS);
            int i = 0;
            while (i++ < TEST_LCD_COLOR_BAR_SHOW_TIME_MS / TEST_LCD_PRINT_FPS_PERIOD_MS) {
                ESP_LOGI(TAG, "FPS: %d", fps);
                vTaskDelay(pdMS_TO_TICKS(TEST_LCD_PRINT_FPS_PERIOD_MS));
            }
#else
            vTaskDelay(pdMS_TO_TICKS(TEST_LCD_COLOR_BAR_SHOW_TIME_MS));
#endif
        });
    } else {
        ESP_LOGI(TAG, "LCD is not available");
    }

    if (backlight != nullptr) {
        ESP_LOGI(TAG, "Turn on the backlight");
        TEST_ASSERT_TRUE_MESSAGE(backlight->on(), "Backlight on failed");
    }

    thread touch_thread;
    if (touch != nullptr) {
#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK
        if (touch->isInterruptEnabled()) {
            TEST_ASSERT_TRUE_MESSAGE(
                touch->attachInterruptCallback(onTouchInterruptCallback, NULL), "Attach touch interrupt callback failed"
            );
        }
#endif

        touch_thread = std::thread([&]() {
            ESP_LOGI(TAG, "Reading touch_device point...");

            uint32_t t = 0;
            utils::vector<drivers::TouchPoint> points;
            utils::vector<drivers::TouchButton> buttons;

            while (t++ < TEST_TOUCH_READ_TIME_MS / TEST_TOUCH_READ_PERIOD_MS) {
                TEST_ASSERT_TRUE_MESSAGE(touch->readRawData(-1, -1, TEST_TOUCH_READ_PERIOD_MS), "Read touch raw data failed");
                TEST_ASSERT_TRUE_MESSAGE(touch->getPoints(points), "Read touch points failed");
                TEST_ASSERT_TRUE_MESSAGE(touch->getButtons(buttons), "Read touch buttons failed");
                int i = 0;
                for (auto &point : points) {
                    ESP_LOGI(TAG, "Point(%d): x(%d), y(%d), strength(%d)", i++, point.x, point.y, point.strength);
                }
                for (auto &button : buttons) {
                    ESP_LOGI(TAG, "Button(%d): %d", i++, button.second);
                }
                if (!touch->isInterruptEnabled()) {
                    delay(TEST_TOUCH_READ_PERIOD_MS);
                }
            }
        });
    } else {
        ESP_LOGI(TAG, "Touch is not available");
    }

    if (lcd_thread.joinable()) {
        lcd_thread.join();
    }
    if (touch_thread.joinable()) {
        touch_thread.join();
    }

    board->del();
}
