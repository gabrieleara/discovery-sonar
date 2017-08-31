#ifndef CONST_H
#define CONST_H

#include "types.h"

#define SOFTWARE_VERSION    "0.1 alpha"

/* -----------------------------------------------------------------------------
 * Constants used by all modules of the program
 * -----------------------------------------------------------------------------
 */

#define SYST_PERIOD     (10)// Defines the systick period in microseconds
#define MOTOR_PERIOD    (20000)
                            // Defines the interval in microseconds between one
                            // motor move and the following one, must be an
                            // integer multiple of SYST_PERIOD

#define MODTOR_PERIOD_TICKS (MOTOR_PERIOD / SYST_PERIOD)
                            // Defines the interval in number of ticks between
                            // one motor move and the following one

#define MOTOR_INC   (10)    // The increment used each time we want to move
                            // the motor at each MOTOR_PERIOD

// FIXME:
// Maximum (displayable?) distance in cm
#define DISPLAY_DIST_MAX_CM     (400)

#endif
