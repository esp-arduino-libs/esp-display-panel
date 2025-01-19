/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
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
#include "esp_display_panel.hpp"

using namespace std;
using namespace esp_panel::drivers;

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
#define TEST_TOUCH_RST_ACTIVE_LEVEL     (0)
#define TEST_TOUCH_PIN_NUM_INT          (-1)

#define TEST_TOUCH_ENABLE_ATTACH_CALLBACK   (1)
#define TEST_TOUCH_READ_DELAY_MS        (30)
#define TEST_TOUCH_READ_TIME_MS         (5000)

#define delay(x)     vTaskDelay(pdMS_TO_TICKS(x))

static const char *TAG = "test_spi_touch";

static const Touch::Config touch_config = {
    .device = Touch::DevicePartialConfig{
        .x_max = TEST_TOUCH_WIDTH,
        .y_max = TEST_TOUCH_HEIGHT,
        .rst_gpio_num = TEST_TOUCH_PIN_NUM_RST,
        .int_gpio_num = TEST_TOUCH_PIN_NUM_INT,
        .levels_reset = TEST_TOUCH_RST_ACTIVE_LEVEL,
    },
};

#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK && (TEST_TOUCH_PIN_NUM_INT >= 0)
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

static void run_test(shared_ptr<Touch> touch, bool use_config)
{
    if (!use_config) {
        touch->configResetActiveLevel(TEST_TOUCH_RST_ACTIVE_LEVEL);
    }
    TEST_ASSERT_TRUE_MESSAGE(touch->init(), "Touch init failed");
    TEST_ASSERT_TRUE_MESSAGE(touch->begin(), "Touch begin failed");
#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK && (TEST_TOUCH_PIN_NUM_INT >= 0)
    TEST_ASSERT_TRUE_MESSAGE(
        touch->attachInterruptCallback(onTouchInterruptCallback, NULL), "Touch attach interrupt callback failed"
    );
#endif

    ESP_LOGI(TAG, "Reading touch point...");

    auto point_ptr = std::make_shared<TouchPoint []>(touch->getBasicAttributes().max_points_num);
    TEST_ASSERT_NOT_NULL_MESSAGE(point_ptr, "Create point array failed");

    uint32_t t = 0;
    auto point = point_ptr.get();
    int touch_cnt = 0;
    while (t++ < TEST_TOUCH_READ_TIME_MS / TEST_TOUCH_READ_DELAY_MS) {
        touch_cnt = touch->readPoints(point, -1, TEST_TOUCH_READ_DELAY_MS);
        if (touch_cnt > 0) {
            for (int i = 0; i < touch_cnt; i++) {
                ESP_LOGI(TAG, "Touch point(%d): x %d, y %d, strength %d", i, point[i].x, point[i].y, point[i].strength);
            }
        } else if (touch_cnt < 0) {
            ESP_LOGE(TAG, "Read touch point failed");
        }
#if TEST_TOUCH_PIN_NUM_INT < 0
        delay(TEST_TOUCH_READ_DELAY_MS);
#endif
    }
}

#define CREATE_BUS_WITH_CONFIG(name, config) \
    ({ \
        ESP_LOGI(TAG, "Create bus with config"); \
        auto bus = make_shared<BusSPI>(config); \
        TEST_ASSERT_NOT_NULL_MESSAGE(bus, "Create bus object failed"); \
        TEST_ASSERT_TRUE_MESSAGE(bus->begin(), "Bus begin failed"); \
        bus; \
    })

#define CREATE_BUS(name) \
    ({ \
        ESP_LOGI(TAG, "Create bus with default parameters"); \
        auto bus = make_shared<BusSPI>( \
            TEST_TOUCH_PIN_NUM_SPI_SCK, TEST_TOUCH_PIN_NUM_SPI_MOSI, TEST_TOUCH_PIN_NUM_SPI_MISO, \
            (BusSPI::ControlPanelFullConfig)ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, TEST_TOUCH_PIN_NUM_SPI_CS)); \
        TEST_ASSERT_NOT_NULL_MESSAGE(bus, "Create bus object failed"); \
        TEST_ASSERT_TRUE_MESSAGE(bus->configSPI_FreqHz(TEST_TOUCH_SPI_FREQ_HZ), "Bus config SPI frequency failed"); \
        TEST_ASSERT_TRUE_MESSAGE(bus->begin(), "Bus begin failed"); \
        bus; \
    })

template<typename T>
decltype(auto) create_touch_impl(Bus *bus, const Touch::Config &config)
{
    ESP_LOGI(TAG, "Create touch with config");
    return make_shared<T>(bus, config);
}

template<typename T>
decltype(auto) create_touch_impl(Bus *bus, std::nullptr_t)
{
    ESP_LOGI(TAG, "Create touch with default parameters");
    return make_shared<T>(bus, TEST_TOUCH_WIDTH, TEST_TOUCH_HEIGHT, TEST_TOUCH_PIN_NUM_RST, TEST_TOUCH_PIN_NUM_INT);
}

#define CREATE_TOUCH(name, bus, config) \
    ({ \
        auto touch = create_touch_impl<Touch##name>(bus, config); \
        TEST_ASSERT_NOT_NULL_MESSAGE(touch, "Create touch object failed"); \
        touch; \
    })
#define CREATE_TEST_CASE(name) \
    TEST_CASE("Test touch (" #name ") to read touch point", "[touch][i2c][" #name "]") \
    { \
        /* 1. Test with default parameters */ \
        auto bus = CREATE_BUS(name); \
        auto touch = CREATE_TOUCH(name, bus.get(), nullptr); \
        run_test(touch, false); \
        bus = nullptr; \
        touch = nullptr; \
        /* 2. Test with config */ \
        BusSPI::Config bus_config = { \
            .host = BusSPI::HostPartialConfig{ \
                .mosi_io_num = TEST_TOUCH_PIN_NUM_SPI_MOSI, \
                .miso_io_num = TEST_TOUCH_PIN_NUM_SPI_MISO, \
                .sclk_io_num = TEST_TOUCH_PIN_NUM_SPI_SCK, \
            }, \
            .control_panel = \
                (BusSPI::ControlPanelFullConfig)ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(name, TEST_TOUCH_PIN_NUM_SPI_CS), \
        }; \
        bus = CREATE_BUS_WITH_CONFIG(name, bus_config); \
        touch = CREATE_TOUCH(name, bus.get(), touch_config); \
        run_test(touch, true); \
        bus = nullptr; \
        touch = nullptr; \
        gpio_uninstall_isr_service(); \
    }

/**
 * Here to create test cases for different touchs
 */
CREATE_TEST_CASE(XPT2046)
