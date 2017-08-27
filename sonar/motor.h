/*
 * motor.h
 *
 * This file contains all declaration of public functions and global variables
 * defined in the motor.c file.
 *
 * */

#ifndef MOTOR_H
#define MOTOR_H

// ---------------------------
// Includes
// ---------------------------

// Include core modules
#include "ee.h"
#include "stm32f4xx.h"

// Include TM libraries
#include "Lib/tm_stm32f4_pwm.h"

// ---------------------------
// PWM motor constant
// ---------------------------

#define MOTOR_FRQ	(50)	// Defines the motor working frequency (Mhz)
#define MOTOR_MIN   (550)   // Defines the min position of the motor (+90°)
#define MOTOR_MAX   (2250)  // Defines the max position of the motor (-90°)

#define MOTOR_MID	((MOTOR_MAX-MOTOR_MIN)/2 + MOTOR_MIN)
							// Defines the mid position of the motor (0°)
#define MOTOR_RANGE (POSITION_MAX-POSITION_MIN)
                            // The range of the positions of the motor

// ------------------------
// STM32F4 timer/pwm pinout
// ------------------------

#define TIMER TIM2					// Timer to be used (on board)
#define CHANNEL TM_PWM_Channel_1	// Channel of pin to be used
#define PINS_PACK TM_PWM_PinsPack_2	// PinsPack to be used
									// --> TIM2 on GPIOA PA5

// ---------------------------
// Data structures and data types
// ---------------------------

typedef enum {		// Possible direction enumerator
	RIGHT 	= -1,	// Right direction (clockwise)
	STOP 	=  0,	// Halt
	LEFT 	= +1	// Left direction (counter clockwise)
} dir;

typedef struct {				// PWM Motor data structure
	uint32_t 		curr_pos;	// Motor current position
	dir 			curr_dir;	// Motor current direction
	uint32_t 		curr_stp;	// Motor current step increment
	TM_PWM_TIM_t 	TIM_Data;	// Motor timer data structure
} motor;

// ---------------------------
// Functions
// ---------------------------

#define motor_invert_dir() (my_motor.curr_dir *= -1)

/*
 * Initialize motor with initial parameter
 * in:	initial motor position
 * 		initial motor direction
 * 		default motor increment
 * ret: void
 */
void motor_init(uint32_t init_pos, dir init_dir, uint32_t init_stp);

/*
 * Set position of the motor and move the motor to the chosen position
 * in:	new position of the motor
 * ret: void
 */
void motor_set_pos(uint32_t position);

/*
 * Set a new value for the single step increment of the motor
 * in:	new value for the step increment
 * ret: void
 */
void motor_set_stp(uint32_t step);

/*
 * Set a new value for the direction of the motor movement
 * in:	new value for the direction
 * ret: void
 */
void motor_set_dir(dir direction);

/*
 * Move the motor following the current increment and direction
 * in:	void
 * ret: void
 */
void motor_step();

#endif
