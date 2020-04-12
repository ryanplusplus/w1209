/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <string.h>
#include "stm8s_gpio.h"
#include "display.h"

#define init_output(_port, _pin)   \
  GPIO##_port->DDR |= (1 << _pin); \
  GPIO##_port->CR1 |= (1 << _pin)

#define write_output(_port, _pin, _state) \
  if(_state)                              \
    GPIO##_port->ODR |= (1 << _pin);      \
  else                                    \
    GPIO##_port->ODR &= ~(1 << _pin)

static tiny_event_subscription_t interrupt_subscription;
static uint8_t current_digit;
static char display_string_buffer[3];
static char display_string[3];

static void digit_on(int section) {
  switch(section) {
    case 0:
      write_output(D, 4, 0);
      break;

    case 1:
      write_output(B, 5, 0);
      break;

    case 2:
      write_output(B, 4, 0);
      break;

    default:
      break;
  }
}

static void digit_off(int section) {
  switch(section) {
    case 0:
      write_output(D, 4, 1);
      break;

    case 1:
      write_output(B, 5, 1);
      break;

    case 2:
      write_output(B, 4, 1);
      break;

    default:
      break;
  }
}

static void display_character(char character) {
  switch(character) {
    case 0:
    case ' ':
      write_output(D, 5, 0); // A
      write_output(A, 2, 0); // B
      write_output(C, 7, 0); // C
      write_output(D, 3, 0); // D
      write_output(D, 1, 0); // E
      write_output(A, 1, 0); // F
      write_output(C, 6, 0); // G
      write_output(D, 2, 0); // P
      break;

    case '0':
      write_output(D, 5, 1); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 1); // D
      write_output(D, 1, 1); // E
      write_output(A, 1, 1); // F
      write_output(C, 6, 0); // G
      write_output(D, 2, 0); // P
      break;

    case '1':
      write_output(D, 5, 0); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 0); // D
      write_output(D, 1, 0); // E
      write_output(A, 1, 0); // F
      write_output(C, 6, 0); // G
      write_output(D, 2, 0); // P
      break;

    case '2':
      write_output(D, 5, 1); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 0); // C
      write_output(D, 3, 1); // D
      write_output(D, 1, 1); // E
      write_output(A, 1, 0); // F
      write_output(C, 6, 1); // G
      write_output(D, 2, 0); // P
      break;

    case '3':
      write_output(D, 5, 1); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 1); // D
      write_output(D, 1, 0); // E
      write_output(A, 1, 0); // F
      write_output(C, 6, 1); // G
      write_output(D, 2, 0); // P
      break;

    case '4':
      write_output(D, 5, 0); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 0); // D
      write_output(D, 1, 0); // E
      write_output(A, 1, 1); // F
      write_output(C, 6, 1); // G
      write_output(D, 2, 0); // P
      break;

    case '5':
      write_output(D, 5, 1); // A
      write_output(A, 2, 0); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 1); // D
      write_output(D, 1, 0); // E
      write_output(A, 1, 1); // F
      write_output(C, 6, 1); // G
      write_output(D, 2, 0); // P
      break;

    case '6':
      write_output(D, 5, 1); // A
      write_output(A, 2, 0); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 1); // D
      write_output(D, 1, 1); // E
      write_output(A, 1, 1); // F
      write_output(C, 6, 1); // G
      write_output(D, 2, 0); // P
      break;

    case '7':
      write_output(D, 5, 1); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 0); // D
      write_output(D, 1, 0); // E
      write_output(A, 1, 0); // F
      write_output(C, 6, 0); // G
      write_output(D, 2, 0); // P
      break;

    case '8':
      write_output(D, 5, 1); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 1); // D
      write_output(D, 1, 1); // E
      write_output(A, 1, 1); // F
      write_output(C, 6, 1); // G
      write_output(D, 2, 0); // P
      break;

    case '9':
      write_output(D, 5, 1); // A
      write_output(A, 2, 1); // B
      write_output(C, 7, 1); // C
      write_output(D, 3, 1); // D
      write_output(D, 1, 0); // E
      write_output(A, 1, 1); // F
      write_output(C, 6, 1); // G
      write_output(D, 2, 0); // P
      break;

    default:
      break;
  }
}

static void do_stuff(void* context, const void* args) {
  (void)context;
  (void)args;

  if(current_digit == 2) {
    display_string[0] = display_string_buffer[0];
    display_string[1] = display_string_buffer[1];
    display_string[2] = display_string_buffer[2];
  }

  display_character(' ');
  digit_off(current_digit);
  current_digit = (current_digit + 1) % 3;
  display_character(display_string[current_digit]);
  digit_on(current_digit);
}

void display_init(i_tiny_event_t* interrupt) {
  // Turns everything on
  init_output(D, 5); // A
  init_output(A, 2); // B
  init_output(C, 7); // C
  init_output(D, 3); // D
  init_output(D, 1); // E
  init_output(A, 1); // F
  init_output(C, 6); // G
  init_output(D, 2); // P

  init_output(D, 4); // 1
  init_output(B, 5); // 2
  init_output(B, 4); // 3

  digit_off(0);
  digit_off(1);
  digit_off(2);

  tiny_event_subscription_init(&interrupt_subscription, NULL, do_stuff);
  tiny_event_subscribe(interrupt, &interrupt_subscription);
}

void display_write(const char* s) {
  display_string_buffer[0] = s[0];
  display_string_buffer[1] = s[1];
  display_string_buffer[2] = s[2];
}
