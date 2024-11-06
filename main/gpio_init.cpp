#include "gpio_init.h"

void io_dir_out(byte pin) {
  pinMode(pin, OUTPUT);
}

void io_out_low(byte pin) {
  digitalWrite(pin, LOW);
}

void gpio_out_low(byte pin) {
  io_dir_out(pin);
  io_out_low(pin);
}