#pragma once
#include <stdint.h>

int joystick_init(void (*joystick_pressed_cb)(), void (*joystick_released_cb)());

int joystick_get_position(int16_t *x, int16_t *y);

