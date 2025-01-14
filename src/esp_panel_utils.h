/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

// Define the log tag for the current library, should be declared before `esp_panel_utils.h`
#undef ESP_UTILS_LOG_TAG
#define ESP_UTILS_LOG_TAG "Panel"
#include "esp_lib_utils.h"

#ifdef __cplusplus

namespace esp_panel::utils {

template <typename T, typename... Args>
std::shared_ptr<T> make_shared(Args &&... args)
{
    return std::allocate_shared<T, esp_utils::GeneralMemoryAllocator<T>>(
               esp_utils::GeneralMemoryAllocator<T>(), std::forward<Args>(args)...
           );
}

template <typename T>
using vector = std::vector<T, esp_utils::GeneralMemoryAllocator<T>>;

}

#endif // __cplusplus
