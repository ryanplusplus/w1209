/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include "test.h"
#include "message.h"
#include "tiny_utils.h"

static tiny_event_subscription_t message_received_subscription;
static i_tiny_message_bus_t* message_bus;

static void write_to_display(const char* s) {
  message_write_display_data_t data = { s };
  tiny_message_bus_send(message_bus, message_write_display, &data);
}

static void message_received(void* context, const void* _args) {
  reinterpret(args, _args, const tiny_message_bus_on_receive_args_t*);
  (void)context;

  if(args->message == message_key_event) {
    reinterpret(key_event, args->data, const message_key_event_data_t*);

    switch(key_event->key) {
      case key_left:
        write_to_display(key_event->key_event == key_event_press ? "L P" : "L R");
        break;

      case key_center:
        write_to_display(key_event->key_event == key_event_press ? "C P" : "C R");
        break;

      case key_right:
        write_to_display(key_event->key_event == key_event_press ? "R P" : "R R");
        break;
    }
  }
}

void test(i_tiny_message_bus_t* _message_bus) {
  message_bus = _message_bus;

  write_to_display("123");

  tiny_event_subscription_init(&message_received_subscription, NULL, message_received);
  tiny_event_subscribe(tiny_message_bus_on_receive(message_bus), &message_received_subscription);
}
