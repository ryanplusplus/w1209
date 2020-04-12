/*!
 * @file
 * @brief
 */

#ifndef keypad_h
#define keypad_h

#include "i_tiny_key_value_store.h"
#include "tiny_timer.h"

void keypad_init(i_tiny_key_value_store_t* key_value_store, tiny_timer_group_t* timer_group);

#endif
