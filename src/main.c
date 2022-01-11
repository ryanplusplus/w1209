/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "stm8s.h"
#include "clock.h"
#include "interrupts.h"
#include "tim4_system_tick.h"
#include "tiny_timer.h"
#include "watchdog.h"
#include "data_model.h"
#include "keypad.h"
#include "display.h"
#include "test.h"
#include "relay.h"

static tiny_timer_group_t timer_group;
static data_model_t data_model;

void main(void)
{
  interrupts_disable();
  {
    clock_init();
    tiny_timer_group_init(&timer_group, tim4_system_tick_init());
    watchdog_init(&timer_group);
    data_model_init(&data_model);
    display_init(tim4_system_tick_interrupt(), data_model_key_value_store(&data_model));
    keypad_init(data_model_key_value_store(&data_model), &timer_group);
    relay_init(data_model_key_value_store(&data_model));
  }
  interrupts_enable();

  test(data_model_key_value_store(&data_model));

  while(true) {
    tiny_timer_group_run(&timer_group);
    interrupts_wait_for_interrupt();
  }
}
