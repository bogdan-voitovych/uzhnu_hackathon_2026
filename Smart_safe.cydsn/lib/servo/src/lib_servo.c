/********************************************************************************
 **********                           INCLUDE FILES                   ***********
*********************************************************************************/
#include "lib_servo.h"
#include "project.h"
#include "PWM_SERVO.h"

#define LOG_LEVEL LOG_LEVEL_INFO
#include "log_dbg.h"
#define TAG "SERVO"


/********************************************************************************
 **********                        PRIVATE DEFINITIONS                ***********
 *********************************************************************************/

/**
 * Servo pulse width limits in microseconds.
 * Standard hobby servos typically respond to 1 ms..2 ms pulses.
 */
#define SERVO_MIN_PULSE_US 500u
#define SERVO_MAX_PULSE_US 2400u
#define SERVO_MID_ANGLE_DEG 90u
#define SERVO_STEP_DELAY_MS 10u

static uint16_t g_servo_current_angle_deg = SERVO_MID_ANGLE_DEG;


/********************************************************************************
 **********                        PRIVATE FUNCTIONS                  ***********
*********************************************************************************/

/**
 * @brief Convert a servo angle to a PWM compare value.
 * @param angle_deg Angle in degrees, clamped to 0..180.
 * @return PWM compare value for the generated PWM_SERVO component.
 */
static uint16_t lib_servo_angle_to_compare(uint16_t angle_deg)
{
	if (angle_deg > 180u)
	{
		angle_deg = 180u;
	}

	uint32_t pulse_us = SERVO_MIN_PULSE_US +
		((uint32_t)angle_deg * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US) + 90u) / 180u;

	return (uint16_t)pulse_us;
}

/********************************************************************************
 **********                         PUBLIC FUNCTIONS                  ***********
*********************************************************************************/

/**
 * @brief Initialize and start the servo motor PWM control
 * @details Initializes the PWM2_SERVO component, enables it, and sets the servo
 *          to the middle position (90 degrees). Configures the servo for standard
 *          hobby servo pulse width modulation (1-2 ms at 50 Hz frequency)
 * @return void
 * @note Must be called once during system initialization before using lib_servo_set_angle()
 * @see lib_servo_set_angle()
 */
void lib_servo_init(void)
{
    LOG_I(TAG, "Initializing servo motor...");
    PWM2_SERVO_Init();
    LOG_D(TAG, "PWM2_SERVO initialized");
    PWM2_SERVO_Enable();
    LOG_D(TAG, "PWM2_SERVO enabled");
	PWM2_SERVO_Start();
	LOG_D(TAG, "PWM2_SERVO started");
	lib_servo_set_angle(SERVO_MID_ANGLE_DEG);
	LOG_I(TAG, "Servo initialization complete, set to 90 degrees");
}

/**
 * @brief Set the servo motor to a specific angle
 * @param angle_deg Desired angle in degrees (0-180). Values outside this range
 *                   are clamped to the valid range
 * @details Calculates the required PWM pulse width for the specified angle and
 *          updates the PWM compare value. Uses smooth motion with 1-degree
 *          incremental steps and 5ms delays for mechanical stability.
 *          - 0° = minimum pulse width (~700 μs)
 *          - 90° = middle pulse width (~1500 μs)
 *          - 180° = maximum pulse width (~2300 μs)
 * @return void
 * @note Can be called repeatedly; servo will smoothly transition between angles
 * @see lib_servo_init()
 */
void lib_servo_set_angle(uint16_t angle_deg)
{
	if (angle_deg > 180u)
	{
		angle_deg = 180u;
	}

	while (g_servo_current_angle_deg != angle_deg)
	{
		if (g_servo_current_angle_deg < angle_deg)
		{
			g_servo_current_angle_deg++;
		}
		else
		{
			g_servo_current_angle_deg--;
		}

		PWM2_SERVO_WriteCompare(lib_servo_angle_to_compare(g_servo_current_angle_deg));
		CyDelay(SERVO_STEP_DELAY_MS);
	}

	LOG_D(TAG, "Servo angle set to %d degrees", angle_deg);
}

/* [] END OF FILE */
