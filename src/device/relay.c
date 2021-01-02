/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "relay.h"
#include "data_model.h"

enum {
  pin_3 = (1 << 3)
};

static tiny_event_subscription_t data_change_subscription;

static void data_changed(void* context, const void* _args)
{
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  (void)context;

  if(args->key == key_relay_state) {
    reinterpret(state, args->value, const bool*);

    if(*state) {
      GPIOA->ODR |= pin_3;
    }
    else {
      GPIOA->ODR &= ~pin_3;
    }
  }
}

void relay_init(i_tiny_key_value_store_t* key_value_store)
{
  // Configure push pull output
  GPIOA->CR1 |= pin_3;
  GPIOA->DDR |= pin_3;

  tiny_event_subscription_init(&data_change_subscription, NULL, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &data_change_subscription);
}
