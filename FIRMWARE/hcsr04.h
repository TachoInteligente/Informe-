#ifndef _HCSR04_H_
#define _HCSR04_H_

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Prototipos de funciones

void hcsr04_init(uint8_t trigger_gpio, uint8_t echo_gpio);
float hcsr04_get_distance_cm(uint8_t trigger_gpio, uint8_t echo_gpio);

#endif