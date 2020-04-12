/*!
 * @file
 * @brief
 */

#ifndef message_h
#define message_h

#include "i_tiny_message_bus.h"
#include "key.h"
#include "key_event.h"

enum {
  message_write_display,
  message_key_event,
};

typedef struct {
  const char* s;
} message_write_display_data_t;

typedef struct {
  key_t key;
  key_event_t key_event;
} message_key_event_data_t;

#endif
