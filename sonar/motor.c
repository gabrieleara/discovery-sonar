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

// ---------------------------
// Globals
// ---------------------------

motor my_motor;	// Motor instance

// ---------------------------
// Functions
// ---------------------------

void motor_init(uint32_t init_pos, dir init_dir, uint32_t init_stp) {

	// Set initial direction and position
	my_motor.curr_pos = init_pos;
	my_motor.curr_dir = init_dir;
	my_motor.curr_stp = init_stp;

    // Set PWM to frequency on timer
    TM_PWM_InitTimer(TIMER, &my_motor.TIM_Data, MOTOR_FRQ);

    // Initialize PWM on TIMER, CHANNEL and PINS_PACK chosen
    TM_PWM_InitChannel(&my_motor.TIM_Data, CHANNEL, PINS_PACK);

    // Write init position on TIM
    TM_PWM_SetChannelMicros(&my_motor.TIM_Data, CHANNEL, my_motor.curr_pos);
}

void motor_set_pos(uint32_t position) {

	// Set new motor position
	my_motor.curr_pos = position;

	// Write new position on TIM
	TM_PWM_SetChannelMicros(&my_motor.TIM_Data, CHANNEL, my_motor.curr_pos);
}

void motor_set_stp(uint32_t step) {

	// Set new value for the step increment
	my_motor.curr_stp = step;
}

void motor_set_dir(dir direction) {

	// Set new value for the motor direction (LEFT, RIGHT, STOP)
	my_motor.curr_dir = direction;
}

void motor_step() {

	// Motor is in halt state
	if(my_motor.curr_dir == STOP)
		return;

	// Set new motor position
	my_motor.curr_pos += my_motor.curr_stp * my_motor.curr_dir;

	// Check motor physical limit
	if(my_motor.curr_pos >= MOTOR_MAX || my_motor.curr_pos <= MOTOR_MIN)
		motor_invert_dir();

	// Write new position on TIM
	TM_PWM_SetChannelMicros(&my_motor.TIM_Data, CHANNEL, my_motor.curr_pos);
}

// ---------------------------
// Tasks
// ---------------------------

/*
 * Insert all tasks here
 * */
