
/*!
 * @file
 * @brief
 */

#ifndef display_h
#define display_h

#include "i_tiny_event.h"

void display_init(i_tiny_event_t* interrupt);
void display_write(const char* s);

#endif
