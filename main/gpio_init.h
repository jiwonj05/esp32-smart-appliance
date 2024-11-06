#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#include <Arduino.h>

// Integrate more functions if other configurations of registers required
void io_dir_out(byte pin);
void io_out_low(byte pin);
void gpio_out_low(byte pin);

#endif // GPIO_INIT_H