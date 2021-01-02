/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "stm8s.h"
#include "keypad.h"
#include "data_model.h"
#include "tiny_utils.h"

enum {
  poll_period_msec = 15,
  debounce_count = 3,

  pin_3 = (1 << 3),
  pin_4 = (1 << 4),
  pin_5 = (1 << 5),
};

typedef struct {
  key_t key;
  uint8_t mask;
  bool debounced_state;
  bool last_state;
  uint8_t debounce_count;
} keymap_key_t;

static i_tiny_key_value_store_t* key_value_store;
static tiny_timer_t timer;
static keymap_key_t key[] = {
  { key_left, pin_3, 1, 1, 0 },
  { key_center, pin_4, 1, 1, 0 },
  { key_right, pin_5, 1, 1, 0 },
};

static void poll(tiny_timer_group_t* timer_group, void* context)
{
  (void)context;

  uint8_t port_state = GPIOC->IDR;

  for(uint8_t i = 0; i < element_count(key); i++) {
    uint8_t pin_state = !!(port_state & key[i].mask);

    if(pin_state != key[i].last_state) {
      key[i].debounce_count = 1;
      key[i].last_state = pin_state;
    }
    else if(key[i].debounce_count < debounce_count) {
      key[i].debounce_count++;

      if((key[i].debounce_count == debounce_count) && (key[i].debounced_state != pin_state)) {
        key[i].debounced_state = pin_state;

        key_event_t data = {
          key[i].key,
          pin_state ? key_action_release : key_action_press
        };

        tiny_key_value_store_write(key_value_store, key_key_event, &data);
      }
    }
  }

  tiny_timer_start(timer_group, &timer, poll_period_msec, poll, NULL);
}

void keypad_init(i_tiny_key_value_store_t* _key_value_store, tiny_timer_group_t* timer_group)
{
  key_value_store = _key_value_store;

  for(uint8_t i = 0; i < element_count(key); i++) {
    GPIOC->DDR &= ~key[i].mask;
    GPIOC->CR1 |= key[i].mask;
  }

  poll(timer_group, NULL);
}
