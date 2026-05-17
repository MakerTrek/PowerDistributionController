#pragma once
#include <stdint.h>
#include <inttypes.h>

/*
simple CAN wrapper library for interfacing VESC,
taken from https://github.com/UESTC-LIMITI/VESC/blob/master/documentation/comm_can.md.
Uses CAN internally
*/

int vesc_init(const struct device * can_dev);
void vesc_set_duty(uint8_t controller_id, float duty);
void vesc_set_current(uint8_t controller_id, float current);
void vesc_set_current_off_delay(uint8_t controller_id, float current, float off_delay);
void vesc_set_current_brake(uint8_t controller_id, float current);
void vesc_set_rpm(uint8_t controller_id, float rpm);
void vesc_set_pos(uint8_t controller_id, float pos);
void vesc_set_current_rel(uint8_t controller_id, float current_rel);
/**
 * Same as above, but also sets the off delay. Note that this command uses 6 bytes now. The off delay is useful to set to keep the current controller running for a while even after setting currents below the minimum current.
 */
void vesc_set_current_rel_off_delay(uint8_t controller_id, float current_rel, float off_delay);
void vesc_set_current_brake_rel(uint8_t controller_id, float current_rel);
void vesc_set_handbrake(uint8_t controller_id, float current);
void vesc_set_handbrake_rel(uint8_t controller_id, float current_rel);