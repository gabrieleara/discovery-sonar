#ifndef CONST_H
#define CONST_H

#include "types.h"

#define SOFTWARE_VERSION    "1.0 beta"

/* -----------------------------------------------------------------------------
 * Constants used by all modules of the program
 * -----------------------------------------------------------------------------
 */

#define SYST_PERIOD    (50) // Defines the systick period in microseconds
#define STEP_PERIOD    (70000)
                            // Defines the interval in microseconds between one
                            // motor move and the following one, must be an
                            // integer multiple of SYST_PERIOD

#define SCREEN_PERIOD  (90000)
                            // Defines the interval in microseconds between one
                            // screen refresh and the following one, must be an
                            // integer multiple of SYST_PERIOD

#define STEP_PERIOD_TICKS (STEP_PERIOD / SYST_PERIOD)
                            // Defines the interval in number of ticks between
                            // one motor move and the following one

#define SCREEN_PERIOD_TICKS (SCREEN_PERIOD / SYST_PERIOD)
                            // Defines the interval in number of ticks between
                            // one screen refresh and the following one
#endif
