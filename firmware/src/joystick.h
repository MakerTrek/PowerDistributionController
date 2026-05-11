#pragma once
#include <stdint.h>

int joystick_init();

int joystick_get_position(int16_t *x, int16_t *y);