/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "unity.h"
#include "unity_test_utils.h"
#include "esp_display_panel.hpp"

// Some resources are lazy allocated in the LCD driver, the threadhold is left for that case
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI
#define TEST_MEMORY_LEAK_THRESHOLD (800)
#elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
#define TEST_MEMORY_LEAK_THRESHOLD (500)
#else
#define TEST_MEMORY_LEAK_THRESHOLD (300)
#endif

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
void setUp(void)
{
    unity_utils_record_free_mem();
}

void tearDown(void)
{
    esp_reent_cleanup();    //clean up some of the newlib's lazy allocations
    unity_utils_evaluate_leaks_direct(TEST_MEMORY_LEAK_THRESHOLD);
}
#else
static size_t before_free_8bit;
static size_t before_free_32bit;

static void check_leak(size_t before_free, size_t after_free, const char *type)
{
    ssize_t delta = before_free - after_free;
    printf("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta < TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

void setUp(void)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
}

void tearDown(void)
{
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}
#endif

extern "C" void app_main(void)
{
    /**
     *  _______    ______   __    __  ________  __
     * |       \  /      \ |  \  |  \|        \|  \
     * | $$$$$$$\|  $$$$$$\| $$\ | $$| $$$$$$$$| $$
     * | $$__/ $$| $$__| $$| $$$\| $$| $$__    | $$
     * | $$    $$| $$    $$| $$$$\ $$| $$  \   | $$
     * | $$$$$$$ | $$$$$$$$| $$\$$ $$| $$$$$   | $$
     * | $$      | $$  | $$| $$ \$$$$| $$_____ | $$_____
     * | $$      | $$  | $$| $$  \$$$| $$     \| $$     \
     *  \$$       \$$   \$$ \$$   \$$ \$$$$$$$$ \$$$$$$$$
     */
    printf(" _______    ______   __    __  ________  __\r\n");
    printf("|       \\  /      \\ |  \\  |  \\|        \\|  \\\r\n");
    printf("| $$$$$$$\\|  $$$$$$\\| $$\\ | $$| $$$$$$$$| $$\r\n");
    printf("| $$__/ $$| $$__| $$| $$$\\| $$| $$__    | $$\r\n");
    printf("| $$    $$| $$    $$| $$$$\\ $$| $$  \\   | $$\r\n");
    printf("| $$$$$$$ | $$$$$$$$| $$\\$$ $$| $$$$$   | $$\r\n");
    printf("| $$      | $$  | $$| $$ \\$$$$| $$_____ | $$_____\r\n");
    printf("| $$      | $$  | $$| $$  \\$$$| $$     \\| $$     \\\r\n");
    printf(" \\$$       \\$$   \\$$ \\$$   \\$$ \\$$$$$$$$ \\$$$$$$$$\r\n");
    unity_run_menu();
}
