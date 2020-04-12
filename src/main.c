/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "stm8s.h"
#include "clock.h"
#include "tim4_system_tick.h"
#include "pa3_heartbeat.h"
#include "tiny_timer.h"
#include "tiny_message_bus.h"
#include "watchdog.h"
#include "keypad.h"
#include "display.h"
#include "test.h"

static tiny_timer_group_t timer_group;
static tiny_message_bus_t message_bus;

void main(void) {
  disableInterrupts();
  {
    watchdog_init();
    clock_init();
    tiny_timer_group_init(&timer_group, tim4_system_tick_init());
    tiny_message_bus_init(&message_bus);
    pa3_heartbeat_init(&timer_group);
    display_init(tim4_system_tick_interrupt(), &message_bus.interface);
    keypad_init(&message_bus.interface, &timer_group);
  }
  enableInterrupts();

  test(&message_bus.interface);

  while(true) {
    tiny_timer_group_run(&timer_group);
    watchdog_kick();
    wfi();
  }
}
