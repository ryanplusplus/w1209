/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "test.h"
#include "data_model_key_value_pairs.h"
#include "tiny_utils.h"

static tiny_event_subscription_t data_changed_subscription;
static i_tiny_key_value_store_t* key_value_store;

static void write_to_display(const char* s)
{
  tiny_key_value_store_write(key_value_store, key_display_string, s);
}

static void message_received(void* context, const void* _args)
{
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  (void)context;

  if(args->key == key_key_event) {
    reinterpret(key_event, args->value, const key_event_t*);

    switch(key_event->key) {
      case key_left:
        write_to_display(key_event->key_action == key_action_press ? "LFT" : "   ");
        break;

      case key_center:
        write_to_display(key_event->key_action == key_action_press ? "CTR" : "   ");
        break;

      case key_right:
        write_to_display(key_event->key_action == key_action_press ? "RGT" : "   ");
        break;
    }
  }
}

void test(i_tiny_key_value_store_t* _key_value_store)
{
  key_value_store = _key_value_store;

  tiny_event_subscription_init(&data_changed_subscription, NULL, message_received);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &data_changed_subscription);
}
