/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "stm8s_gpio.h"
#include "display.h"
#include "tiny_utils.h"

enum {
  digit_on = 0,
  digit_off = 1
};

#define seg_none 0x00
#define seg_a 0x01
#define seg_b 0x02
#define seg_c 0x04
#define seg_d 0x08
#define seg_e 0x10
#define seg_f 0x20
#define seg_g 0x40
#define seg_p 0x80

#define output(_port, _pin) \
  {                         \
    GPIO##_port,            \
      (1 << _pin)           \
  }

typedef struct {
  GPIO_TypeDef* port;
  uint8_t mask;
} output_t;

static const output_t digit[] = {
  output(D, 4), // 0
  output(B, 5), // 1
  output(B, 4), // 2
};

static const output_t segment[] = {
  output(D, 5), // A
  output(A, 2), // B
  output(C, 7), // C
  output(D, 3), // D
  output(D, 1), // E
  output(A, 1), // F
  output(C, 6), // G
  output(D, 2), // P
};

static const uint8_t segment_map[] = {
  /* ' ' */ seg_none,
  /* '!' */ seg_none,
  /* '"' */ seg_none,
  /* '#' */ seg_none,
  /* '$' */ seg_none,
  /* '%' */ seg_none,
  /* '&' */ seg_none,
  /* ''' */ seg_none,
  /* '(' */ seg_a | seg_d | seg_e | seg_f,
  /* ')' */ seg_a | seg_b | seg_c | seg_d,
  /* '*' */ seg_none,
  /* '+' */ seg_none,
  /* ',' */ seg_none,
  /* '-' */ seg_none,
  /* '.' */ seg_none,
  /* '/' */ seg_none,
  /* '0' */ seg_a | seg_b | seg_c | seg_d | seg_e | seg_f,
  /* '1' */ seg_b | seg_c,
  /* '2' */ seg_a | seg_b | seg_d | seg_e | seg_g,
  /* '3' */ seg_a | seg_b | seg_c | seg_d | seg_g,
  /* '4' */ seg_b | seg_c | seg_f | seg_g,
  /* '5' */ seg_a | seg_c | seg_d | seg_f | seg_g,
  /* '6' */ seg_a | seg_c | seg_d | seg_e | seg_f | seg_g,
  /* '7' */ seg_a | seg_b | seg_c,
  /* '8' */ seg_a | seg_b | seg_c | seg_d | seg_e | seg_f | seg_g,
  /* '9' */ seg_a | seg_b | seg_c | seg_d | seg_f | seg_g,
  /* ':' */ seg_none,
  /* ';' */ seg_none,
  /* '<' */ seg_none,
  /* '=' */ seg_none,
  /* '>' */ seg_none,
  /* '?' */ seg_none,
  /* '@' */ seg_none,
  /* 'A' */ seg_a | seg_b | seg_c | seg_e | seg_f | seg_g,
  /* 'B' */ seg_c | seg_d | seg_e | seg_f | seg_g,
  /* 'C' */ seg_a | seg_d | seg_e | seg_f,
  /* 'D' */ seg_b | seg_c | seg_d | seg_e | seg_g,
  /* 'E' */ seg_a | seg_d | seg_e | seg_f | seg_g,
  /* 'F' */ seg_a | seg_e | seg_f | seg_g,
  /* 'G' */ seg_a | seg_b | seg_c | seg_d | seg_f | seg_g,
  /* 'H' */ seg_b | seg_c | seg_e | seg_f | seg_g,
  /* 'I' */ seg_b | seg_c,
  /* 'J' */ seg_b | seg_c | seg_d | seg_e,
  /* 'K' */ seg_d | seg_e | seg_f | seg_g,
  /* 'L' */ seg_d | seg_e | seg_f,
  /* 'M' */ seg_c | seg_e | seg_g,
  /* 'N' */ seg_c | seg_e | seg_g,
  /* 'O' */ seg_a | seg_b | seg_c | seg_d | seg_e | seg_f,
  /* 'P' */ seg_a | seg_b | seg_e | seg_f | seg_g,
  /* 'Q' */ seg_a | seg_b | seg_c | seg_f | seg_g,
  /* 'R' */ seg_e | seg_g,
  /* 'S' */ seg_a | seg_c | seg_d | seg_f | seg_g,
  /* 'T' */ seg_d | seg_e | seg_f | seg_g,
  /* 'U' */ seg_b | seg_c | seg_d | seg_e | seg_f,
  /* 'V' */ seg_c | seg_d | seg_e,
  /* 'W' */ seg_c | seg_d | seg_e,
  /* 'X' */ seg_b | seg_c | seg_e | seg_f | seg_g,
  /* 'Y' */ seg_b | seg_c | seg_d | seg_f | seg_g,
  /* 'Z' */ seg_a | seg_b | seg_d | seg_e | seg_g,
};

static tiny_event_subscription_t interrupt_subscription;
static uint8_t current_digit;
static char display_string_buffer[3];
static char display_string[3];

static void init_output(const output_t* output) {
  output->port->DDR |= output->mask;
  output->port->CR1 |= output->mask;
}

static void write_output(const output_t* output, uint8_t state) {
  if(state) {
    output->port->ODR |= output->mask;
  }
  else {
    output->port->ODR &= ~output->mask;
  }
}

static void display_character(char c) {
  uint8_t index = c - ' ';

  for(uint8_t i = 0; i < element_count(segment); i++) {
    write_output(&segment[i], segment_map[index] & (1 << i));
  }
}

static void scan(void* context, const void* args) {
  (void)context;
  (void)args;

  if(current_digit == 0) {
    display_string[0] = display_string_buffer[0];
    display_string[1] = display_string_buffer[1];
    display_string[2] = display_string_buffer[2];
  }

  display_character(' ');
  write_output(&digit[current_digit], digit_off);
  current_digit = (current_digit + 1) % element_count(digit);
  display_character(display_string[current_digit]);
  write_output(&digit[current_digit], digit_on);
}

void display_init(i_tiny_event_t* interrupt) {
  for(uint8_t i = 0; i < element_count(digit); i++) {
    init_output(&digit[i]);
  }

  for(uint8_t i = 0; i < element_count(segment); i++) {
    init_output(&segment[i]);
  }

  tiny_event_subscription_init(&interrupt_subscription, NULL, scan);
  tiny_event_subscribe(interrupt, &interrupt_subscription);
}

void display_write(const char* s) {
  display_string_buffer[0] = s[0];
  display_string_buffer[1] = s[1];
  display_string_buffer[2] = s[2];
}
