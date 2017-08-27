#ifndef CONST_H
#define CONST_H

/* -----------------------------------------------------------------------------
 * Emulation of the C++ standard library
 * -----------------------------------------------------------------------------
 */

// Defines a boolean type to be used in C
typedef enum { false, true } bool_t;

#define BOOL(expr) (!!(expr))

// Executes the cast of the expression expr to the type type
#define STATIC_CAST(type, expr) ((type)(expr))


/* -----------------------------------------------------------------------------
 * Constants of the program
 * -----------------------------------------------------------------------------
 * */


#define SYST_PERIOD     (10)// Defines the systick period in microseconds
#define MOTOR_PERIOD    (25000)
                            // Defines the interval in microseconds between one
                            // motor move and the following one, must be an
                            // integer multiple of SYST_PERIOD

#define MOTOR_PERIOD_TICKS (MOTOR_PERIOD / SYST_PERIOD)
                            // Defines the interval in number of ticks between
                            // one motor move and the following one

#define MOTOR_MIN   (550)   // Defines the min position of the motor (+90°)
#define MOTOR_MAX   (2250)  // Defines the max position of the motor (-90°)
#define MOTOR_RANGE (POSITION_MAX-POSITION_MIN)
                            // The range of the positions of the motor

#define MOTOR_INC   (10)    // The increment used each time we want to move
                            // the motor at each MOTOR_PERIOD


/* -----------------------------------------------------------------------------
 * MACRO FUNCTIONS
 * -----------------------------------------------------------------------------
 * */

// Value of PI copied from math.h, used only to define the POSITION_TO_ANGLE
// macro without needing to include math.h
#define CONST_PI        (3.14159265358979323846)

// Converts a motor position to a single precision floating point angle
#define POSITION_TO_ANGLE(pos) \
    STATIC_CAST(float, (STATIC_CAST(double, pos) / POSITION_RANGE * 2.0 * MATH_PI))

// Removes the constant used to define POSITION_TO_ANGLE
#undef CONST_PI

#endif
