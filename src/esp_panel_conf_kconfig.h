/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Touch Driver Configurations //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef ESP_PANEL_CONF_TOUCH_MAX_POINTS
#ifdef CONFIG_ESP_PANEL_CONF_TOUCH_MAX_POINTS
#define ESP_PANEL_CONF_TOUCH_MAX_POINTS CONFIG_ESP_PANEL_CONF_TOUCH_MAX_POINTS
#else
#define ESP_PANEL_CONF_TOUCH_MAX_POINTS      (5)
#endif
#endif
#ifndef ESP_PANEL_CONF_TOUCH_MAX_BUTTONS
#ifdef CONFIG_ESP_PANEL_CONF_TOUCH_MAX_BUTTONS
#define ESP_PANEL_CONF_TOUCH_MAX_BUTTONS CONFIG_ESP_PANEL_CONF_TOUCH_MAX_BUTTONS
#else
#define ESP_PANEL_CONF_TOUCH_MAX_BUTTONS     (1)
#endif
#endif
#ifndef ESP_PANEL_CONF_TOUCH_XPT2046_Z_THRESHOLD
#ifdef CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_Z_THRESHOLD
#define ESP_PANEL_CONF_TOUCH_XPT2046_Z_THRESHOLD CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_Z_THRESHOLD
#else
#define ESP_PANEL_CONF_TOUCH_XPT2046_Z_THRESHOLD     (400)
#endif
#endif
#ifndef ESP_PANEL_CONF_TOUCH_XPT2046_INTERRUPT_MODE
#ifdef CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_INTERRUPT_MODE
#define ESP_PANEL_CONF_TOUCH_XPT2046_INTERRUPT_MODE CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_INTERRUPT_MODE
#else
#define ESP_PANEL_CONF_TOUCH_XPT2046_INTERRUPT_MODE  (0)
#endif
#endif
#ifndef ESP_PANEL_CONF_TOUCH_XPT2046_VREF_ON_MODE
#ifdef CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_VREF_ON_MODE
#define ESP_PANEL_CONF_TOUCH_XPT2046_VREF_ON_MODE CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_VREF_ON_MODE
#else
#define ESP_PANEL_CONF_TOUCH_XPT2046_VREF_ON_MODE    (0)
#endif
#endif
#ifndef ESP_PANEL_CONF_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS
#ifdef CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS
#define ESP_PANEL_CONF_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS
#else
#define ESP_PANEL_CONF_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS   (1)
#endif
#endif
#ifndef ESP_PANEL_CONF_TOUCH_XPT2046_ENABLE_LOCKING
#ifdef CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_ENABLE_LOCKING
#define ESP_PANEL_CONF_TOUCH_XPT2046_ENABLE_LOCKING CONFIG_ESP_PANEL_CONF_TOUCH_XPT2046_ENABLE_LOCKING
#else
#define ESP_PANEL_CONF_TOUCH_XPT2046_ENABLE_LOCKING  (1)
#endif
#endif

// *INDENT-OFF*
