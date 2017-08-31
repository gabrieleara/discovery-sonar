/*
 * motor.c
 *
 * This file contains all the functions and tasks that interact with the servo
 * motor used for this project.
 *
 * */

// ---------------------------
// Includes
// ---------------------------

#include "motor.h"
#include <math.h>

// ---------------------------
// Globals
// ---------------------------

motor my_motor;	// Motor instance

// ---------------------------
// Functions
// ---------------------------

int_t motor_get_motor_pos(int_t user_pos) {
	double motor_pos;

	if(user_pos < USR_MIN_POS)
		return MOTOR_MIN;
	else if(user_pos > USR_MAX_POS)
		return MOTOR_MAX;

	return MOTOR_MIN + 1.0/USR_RANGE_SLOPE * (user_pos - USR_MIN_POS);
}

void motor_init(int_t init_user_pos, direction_t init_dir) {

	// Set initial direction and position
	my_motor.curr_pos = motor_get_motor_pos(init_user_pos);
	my_motor.curr_dir = init_dir;

    // Set PWM to frequency on timer
    TM_PWM_InitTimer(TIMER, &my_motor.TIM_Data, MOTOR_FRQ);

    // Initialize PWM on TIMER, CHANNEL and PINS_PACK chosen
    TM_PWM_InitChannel(&my_motor.TIM_Data, CHANNEL, PINS_PACK);

    // Write init position on TIM
    TM_PWM_SetChannelMicros(&my_motor.TIM_Data, CHANNEL, my_motor.curr_pos);
}

void motor_set_pos(int_t user_pos) {

	// Set new motor position
	my_motor.curr_pos = motor_get_motor_pos(user_pos);

	// Write new position on TIM
	TM_PWM_SetChannelMicros(&my_motor.TIM_Data, CHANNEL, my_motor.curr_pos);
}

void motor_set_dir(direction_t direction) {

	// Set new value for the motor direction (LEFT, RIGHT, STOP)
	my_motor.curr_dir = direction;
}

void motor_step() {

	// Motor is in halt state
	if(my_motor.curr_dir == STOP)
		return;

	// Set new motor position
	my_motor.curr_pos += MOTOR_STP * my_motor.curr_dir;

	// Check motor physical limit
	if(my_motor.curr_pos >= MOTOR_MAX || my_motor.curr_pos <= MOTOR_MIN)
		motor_invert_dir();

	// Write new position on TIM
	TM_PWM_SetChannelMicros(&my_motor.TIM_Data, CHANNEL, my_motor.curr_pos);
}

double motor_pos_to_angle(int_t usr_pos)
{
    return STATIC_CAST(double, usr_pos-USR_MIN_POS) * M_PI / USR_RANGE;
}
