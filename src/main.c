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
#include "pc5_heartbeat.h"
#include "tiny_timer.h"
#include "watchdog.h"
#include "display.h"

static tiny_timer_group_t timer_group;

void main(void) {
  disableInterrupts();
  {
    watchdog_init();
    clock_init();
    tiny_timer_group_init(&timer_group, tim4_system_tick_init());
    pa3_heartbeat_init(&timer_group);
    display_init(tim4_system_tick_interrupt());
    display_write("XYZ");
  }
  enableInterrupts();

  while(true) {
    tiny_timer_group_run(&timer_group);
    watchdog_kick();
    wfi();
  }
}
