/*!
 * @file
 * @brief
 */

#ifndef key_event_h
#define key_event_h

#include "key.h"
#include "key_action.h"

typedef struct {
  key_t key;
  key_action_t key_action;
} key_event_t;

#endif
