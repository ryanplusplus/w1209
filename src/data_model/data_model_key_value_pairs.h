/*!
 * @file
 * @brief
 */

#ifndef data_model_key_value_pairs_h
#define data_model_key_value_pairs_h

#include <stdbool.h>
#include "tiny_ram_key_value_store_macros.h"
#include "key_event.h"
#include "temperature.h"

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_display_string,              char[3]) \
  pair(key_key_event,                   key_event_t) \
  pair(key_current_temperature_f,       temperature_t) \
  pair(key_relay_state,                 bool) \
// clang-format on

enumerate_ram_key_value_pairs(data_model_key_value_pairs);

#endif
