/*!
 * @file
 * @brief
 */

#ifndef keypad_h
#define keypad_h

#include "i_tiny_message_bus.h"
#include "tiny_timer.h"

void keypad_init(i_tiny_message_bus_t* message_bus, tiny_timer_group_t* timer_group);

#endif
