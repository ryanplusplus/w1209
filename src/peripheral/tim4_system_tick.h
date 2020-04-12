/*!
 * @file
 * @brief
 */

#ifndef tim4_system_tick_h
#define tim4_system_tick_h

#include "stm8s_itc.h"
#include "i_tiny_time_source.h"
#include "i_tiny_event.h"

i_tiny_time_source_t* tim4_system_tick_init(void);
i_tiny_event_t* tim4_system_tick_interrupt(void);

void tim4_system_tick_isr(void) __interrupt(ITC_IRQ_TIM4_OVF);

#endif
