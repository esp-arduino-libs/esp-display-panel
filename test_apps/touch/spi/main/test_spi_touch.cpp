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

using namespace std;

/* The following default configurations are for the board 'Espressif: Custom, XPT2046' */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your touch_device spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_TOUCH_WIDTH                (240)
#define TEST_TOUCH_HEIGHT               (320)
#define TEST_TOUCH_SPI_FREQ_HZ          (1 * 1000 * 1000)
#define TEST_TOUCH_READ_POINTS_NUM      (1)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_TOUCH_PIN_NUM_SPI_CS       (46)
#define TEST_TOUCH_PIN_NUM_SPI_SCK      (10)
#define TEST_TOUCH_PIN_NUM_SPI_MOSI     (14)
#define TEST_TOUCH_PIN_NUM_SPI_MISO     (8)
#define TEST_TOUCH_PIN_NUM_RST          (-1)
#define TEST_TOUCH_PIN_NUM_INT          (-1)

#define TEST_READ_TOUCH_DELAY_MS        (30)
#define TEST_READ_TOUCH_TIME_MS         (3000)

static const char *TAG = "test_spi_touch";

#define delay(x)     vTaskDelay(pdMS_TO_TICKS(x))

#if TEST_TOUCH_PIN_NUM_INT >= 0
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

static void run_test(shared_ptr<ESP_PanelTouch> touch_device)
{
    touch_device->init();
    touch_device->begin();
#if TEST_TOUCH_PIN_NUM_INT >= 0
    touch_device->attachInterruptCallback(onTouchInterruptCallback, NULL);
#endif

    ESP_LOGI(TAG, "Reading touch_device point...");

    uint32_t t = 0;
    while (t++ < TEST_READ_TOUCH_TIME_MS / TEST_READ_TOUCH_DELAY_MS) {
        ESP_PanelTouchPoint point[TEST_TOUCH_READ_POINTS_NUM];
        int read_touch_result = touch_device->readPoints(point, TEST_TOUCH_READ_POINTS_NUM, TEST_READ_TOUCH_DELAY_MS);

        if (read_touch_result > 0) {
            for (int i = 0; i < read_touch_result; i++) {
                ESP_LOGI(TAG, "Touch point(%d): x %d, y %d, strength %d\n", i, point[i].x, point[i].y, point[i].strength);
            }
        } else if (read_touch_result < 0) {
            ESP_LOGE(TAG, "Read touch_device point failed");
        }
#if TEST_TOUCH_PIN_NUM_INT < 0
        delay(TEST_READ_TOUCH_DELAY_MS);
#endif
    }
}

#define CREATE_TOUCH_BUS(name) \
    ({ \
        ESP_LOGI(TAG, "Create touch bus"); \
        shared_ptr<ESP_PanelBusSPI> touch_bus = make_shared<ESP_PanelBusSPI>( \
                    TEST_TOUCH_PIN_NUM_SPI_SCK, TEST_TOUCH_PIN_NUM_SPI_MOSI, TEST_TOUCH_PIN_NUM_SPI_MISO, \
                    (esp_lcd_panel_io_spi_config_t)ESP_PANEL_BOARD_TOUCH_SPI_PANEL_IO_CONFIG(name, TEST_TOUCH_PIN_NUM_SPI_CS) \
                ); \
        TEST_ASSERT_NOT_NULL_MESSAGE(touch_bus, "Create panel bus object failed"); \
        touch_bus->configSpiFreqHz(TEST_TOUCH_SPI_FREQ_HZ); \
        TEST_ASSERT_TRUE_MESSAGE(touch_bus->begin(), "Panel bus begin failed"); \
        touch_bus; \
    })
#define CREATE_TOUCH(name, touch_bus) \
    ({ \
        ESP_LOGI(TAG, "Create touch device: " #name); \
        shared_ptr<ESP_PanelTouch> touch_device = make_shared<ESP_PanelTouch_##name>( \
            touch_bus, TEST_TOUCH_WIDTH, TEST_TOUCH_HEIGHT, TEST_TOUCH_PIN_NUM_RST, TEST_TOUCH_PIN_NUM_INT \
        ); \
        TEST_ASSERT_NOT_NULL_MESSAGE(touch_device, "Create TOUCH object failed"); \
        touch_device; \
    })
#define CREATE_TEST_CASE(name) \
    TEST_CASE("Test touch (" #name ") to draw color bar", "[spi_touch][" #name "]") \
    { \
        shared_ptr<ESP_PanelBusSPI> touch_bus = CREATE_TOUCH_BUS(name); \
        shared_ptr<ESP_PanelTouch> touch_device = CREATE_TOUCH(name, touch_bus.get()); \
        run_test(touch_device); \
    }

/**
 * Here to create test cases for different touchs
 *
 */
CREATE_TEST_CASE(XPT2046)
