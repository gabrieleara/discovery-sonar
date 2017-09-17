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
#include <math.h>

#include "ee.h"
//#include "stm32f4xx.h"

#include "lib/tm_stm32f4_pwm.h"

#include "types.h"
#include "motor.h"

/* ---------------------------
 * Data types
 * ---------------------------
 */

// PWM Motor data structure
typedef struct {
    int_t           user_pos;   // Motor current user position
    int_t           curr_pos;   // Motor current position
    direction_t     curr_dir;   // Motor current direction
    TM_PWM_TIM_t    TIM_Data;   // Motor timer data structure
} motor;

/* ---------------------------
 * Globals
 * ---------------------------
 */

motor motor_state;      // Motor instance

/* ---------------------------
 * Private functions
 * ---------------------------
 */

/*
 * Converts the user domain motor position in motor domain position
 * in:  void
 * ret: motor position in user range domain
 */
int_t motor_get_motor_pos(int_t user_pos) {
    double motor_pos;

    if (user_pos < USR_MIN_POS)
        return MOTOR_MIN;
    else if (user_pos > USR_MAX_POS)
        return MOTOR_MAX;

    return MOTOR_MIN + 1.0 / USR_RANGE_SLOPE * (user_pos - USR_MIN_POS);
}

/*
 * Invert motor direction
 * in:  void
 * ret: void
 */
#define motor_invert_dir() (motor_state.curr_dir *= -1)


/* ---------------------------
 * Public functions
 * ---------------------------
 */

/*
 * Initialize motor with initial parameter
 * in:  initial motor position
 *      initial motor direction
 *      default motor increment
 * ret: void
 */
void motor_init(int_t init_user_pos, direction_t init_dir) {

    // Set initial direction and position
    motor_state.curr_pos = motor_get_motor_pos(init_user_pos);
    motor_state.curr_dir = init_dir;

    // Set PWM to frequency on timer
    TM_PWM_InitTimer(TIMER, &motor_state.TIM_Data, MOTOR_FRQ);

    // Initialize PWM on TIMER, CHANNEL and PINS_PACK chosen
    TM_PWM_InitChannel(&motor_state.TIM_Data, CHANNEL, PINS_PACK);

    // Write init position on TIM
    TM_PWM_SetChannelMicros(&motor_state.TIM_Data, CHANNEL, motor_state.curr_pos);
}

/*
 * Set position of the motor and move the motor to the chosen position
 * in:  new position of the motor
 * ret: void
 */
void motor_set_pos(int_t user_pos) {
    // Set new motor position
    motor_state.curr_pos = motor_get_motor_pos(user_pos);

    // Write new position on TIM
    TM_PWM_SetChannelMicros(&motor_state.TIM_Data, CHANNEL, motor_state.curr_pos);
}

/*
 * Set a new value for the direction of the motor movement
 * in:  new value for the direction
 * ret: void
 */
void motor_set_dir(direction_t direction) {

    // Set new value for the motor direction (LEFT, RIGHT, STOP)
    motor_state.curr_dir = direction;
}

/*
 * Move the motor following the current increment and direction
 * in:  void
 * ret: void
 */
void motor_step() {

    // Motor is in halt state
    if (motor_state.curr_dir == STOP)
        return;

    // Set new motor position
    motor_state.curr_pos += MOTOR_STP * motor_state.curr_dir;

    // Check motor physical limit
    if (motor_state.curr_pos >= MOTOR_MAX || motor_state.curr_pos <= MOTOR_MIN)
        motor_invert_dir();

    // Write new position on TIM
    TM_PWM_SetChannelMicros(&motor_state.TIM_Data, CHANNEL, motor_state.curr_pos);
}

int_t motor_get_pos()
{
    return (USR_MIN_POS + USR_RANGE_SLOPE * (motor_state.curr_pos - MOTOR_MIN));
}

/*
 * Converts a user position to the corresponding angle in radians.
 */
double motor_pos_to_angle(int_t usr_pos) {
    return STATIC_CAST(double, USR_MAX_POS - usr_pos /*+ USR_MIN_POS*/) * M_PI / USR_RANGE;
}
