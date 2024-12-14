/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_panel_utils.h"
#include "esp_panel_touch_base.hpp"

namespace esp_panel::drivers {

void Touch::configLevels(int reset_level, int interrupt_level)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(!checkIsInit(), "This function should be called before `begin()`");

    ESP_UTILS_LOGD("Param: reset_level(%d), interrupt_level(%d)", reset_level, interrupt_level);
    touch_config.levels.reset = reset_level;
    touch_config.levels.interrupt = interrupt_level;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool Touch::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "Already initialized");
    ESP_UTILS_CHECK_NULL_RETURN(bus, false, "Invalid bus");

    if (touch_config.int_gpio_num >= 0) {
        ESP_UTILS_LOGD("Enable interruption");

        _callback.on_interrupt_trigger_sem = xSemaphoreCreateBinary();
        ESP_UTILS_CHECK_NULL_RETURN(_callback.on_interrupt_trigger_sem, false, "Create semaphore failed");

        _callback.data.touch_ptr = this;
        touch_config.interrupt_callback = onInterruptTrigger;
        touch_config.user_data = &_callback.data;
    } else {
        ESP_UTILS_LOGD("Disable interruption");
    }

    _flags.is_init = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Touch::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIsBegun()) {
        // ESP_UTILS_CHECK_ERROR_RETURN(
        //     esp_lcd_touch_del(touch_handle), false, "Delete touch panel(@%p) failed", touch_handle
        // );
        ESP_UTILS_LOGD("Touch panel(@%p) deleted", touch_handle);
        touch_handle = nullptr;
    }

    if (_callback.on_interrupt_trigger_sem != nullptr) {
        vSemaphoreDelete(_callback.on_interrupt_trigger_sem);
        _callback.on_interrupt_trigger_sem = nullptr;
    }

    _flags = {};
    _state = {};
    _callback = {};

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Touch::attachInterruptCallback(InterruptTriggerCallback callback, void *user_data)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInterruptEnabled(), false, "Interruption is not enabled");

    ESP_UTILS_LOGD("Param: callback(@%p), user_data(@%p)", callback, user_data);
    _callback.on_interrupt_trigger_callback = callback;
    _callback.data.user_data = user_data;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Touch::swapXY(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Swap XY: %d", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_touch_set_swap_xy(touch_handle, en), false, "Swap axes failed");
    _state.swap_xy = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Touch::mirrorX(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_touch_set_mirror_x(touch_handle, en), false, "Mirror X failed");
    _state.mirror_x = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Touch::mirrorY(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_touch_set_mirror_y(touch_handle, en), false, "Mirror Y failed");
    _state.mirror_y = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Touch::readRawData(uint8_t max_points_num, int timeout_ms)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: max_points_num(%d), timeout_ms(%d)", max_points_num, timeout_ms);

    // Limit the max points number
    if (max_points_num > CONFIG_ESP_LCD_TOUCH_MAX_POINTS) {
        max_points_num = CONFIG_ESP_LCD_TOUCH_MAX_POINTS;
        ESP_UTILS_LOGE("The max points number out of range(%d/%d)", max_points_num, CONFIG_ESP_LCD_TOUCH_MAX_POINTS);
    }

    // Wait for the interruption if it is enabled
    if (_callback.on_interrupt_trigger_sem != nullptr) {
        BaseType_t timeout_ticks = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
        if (xSemaphoreTake(_callback.on_interrupt_trigger_sem, timeout_ticks) != pdTRUE) {
            ESP_UTILS_LOGD("Wait for interruption timeout");
            return true;
        }
    }

    // Read the raw data
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_touch_read_data(touch_handle), false, "Read _callback.data failed");

    // Get the coordinates
    if (max_points_num > 0) {
        uint16_t x[CONFIG_ESP_LCD_TOUCH_MAX_POINTS] = {};
        uint16_t y[CONFIG_ESP_LCD_TOUCH_MAX_POINTS] = {};
        uint16_t strength[CONFIG_ESP_LCD_TOUCH_MAX_POINTS] = {};
        _points_num = 0;
        esp_lcd_touch_get_coordinates(touch_handle, x, y, strength, &_points_num, max_points_num);
        for (int i = 0; i < _points_num; i++) {
            _points[i].x = x[i];
            _points[i].y = y[i];
            _points[i].strength = strength[i];
            ESP_UTILS_LOGD(
                "Get touched point: x(%d), y(%d), strength(%d)", _points[i].x, _points[i].y, _points[i].strength
            );
        }
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

int Touch::getPoints(Point points[], uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: points(@%p), num(%d)", points, num);
    ESP_UTILS_CHECK_FALSE_RETURN((num == 0) || (points != nullptr), -1, "Invalid points or num");

    int i = 0;
    for (; (i < num) && (i < _points_num); i++) {
        points[i] = _points[i];
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return i;
}

int Touch::getButtonState(uint8_t index)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: index(%d)", index);
    uint8_t button_state = 0;
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_touch_get_button_state(touch_handle, index, &button_state), -1, "Get button state failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return button_state;
}

int Touch::readPoints(Point points[], uint8_t num, int timeout_ms)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: points(@%p), num(%d), timeout_ms(%d)", points, num, timeout_ms);
    ESP_UTILS_CHECK_FALSE_RETURN(readRawData(num, timeout_ms), -1, "Read raw data failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return getPoints(points, num);
}

int Touch::readButtonState(uint8_t index, int timeout_ms)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: index(%d), timeout_ms(%d)", index, timeout_ms);
    ESP_UTILS_CHECK_FALSE_RETURN(readRawData(0, timeout_ms), -1, "Read raw data failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return getButtonState(index);
}

void Touch::onInterruptTrigger(esp_lcd_touch_handle_t tp)
{
    if ((tp == nullptr) || (tp->config.user_data == nullptr)) {
        return;
    }

    Touch *panel = (Touch *)((CallbackData *)tp->config.user_data)->touch_ptr;
    if (panel == nullptr) {
        return;
    }

    BaseType_t need_yield = pdFALSE;
    if (panel->_callback.on_interrupt_trigger_callback != nullptr) {
        need_yield = panel->_callback.on_interrupt_trigger_callback(panel->_callback.data.user_data) ? pdTRUE : need_yield;
    }
    if (panel->_callback.on_interrupt_trigger_sem != nullptr) {
        xSemaphoreGiveFromISR(panel->_callback.on_interrupt_trigger_sem, &need_yield);
    }
    if (need_yield == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

} // namespace esp_panel::drivers
