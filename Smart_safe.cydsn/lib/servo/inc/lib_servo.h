/**
 * @file lib_servo.h
 * @brief Servo motor control interface.
 */

#ifndef LIB_SERVO_H
#define LIB_SERVO_H

#include "lib_common_types.h"

/**
 * @brief Initialize the servo driver.
 */
void lib_servo_init(void);

/**
 * @brief Set the servo position.
 *
 * @param angle_deg Target servo angle in degrees.
 */
void lib_servo_set_angle(uint16_t angle_deg);

#endif // LIB_SERVO_H

/* [] END OF FILE */
