/*
 * sensor.h
 *
 * This file contains all declaration of public functions and global variables
 * defined in the sensor.c file.
 *
 * */

#ifndef SENSOR_H
#define SENSOR_H

#include "types.h"
#include "constants.h"

/*
 * Constants
 *
 */

#define SENSOR_DIST_MAX (41200 / SYST_PERIOD)
                    // Maximum distance in number of ticks
                    // about 7 meters

// FIXME: this is a test with 5 meters
// #define SENSOR_DIST_MAX (29430 / SYST_PERIOD)

/* ---------------------------
 * Public functions
 * ---------------------------
 */

extern void sensors_init();

/*
 * Reads both sensors value, see the read_sensor function.
 */
extern void sensors_read();

/*
 * Sends the trigger signal. At the moment of sending the trigger it also
 * updates the global calculated distance at the previous step.
 */
extern void sensors_send_trigger();

/*
 * Stops the trigger signal.
 */
extern void sensors_stop_trigger();

/*
 * Returns the last calculated distance.
 */
extern int_t sensors_get_last_distance();


// NOTICE: The following macros may overflow if the receiver container is not a
// lont_int_t variable.
#define DISTANCE_TO_CM(val) \
    (STATIC_CAST(long_int_t,val) * 340 * SYST_PERIOD / 2 / 10000)

// NOTICE: the maximum resolution of the system is the centimeter, because
// otherwise the screen is no more able to keep up with the overload.


#define DISTANCE_TO_MM(val) \
    (STATIC_CAST(long_int_t,val) * 340 * SYST_PERIOD / 2 / 1000)
#define DISTANCE_TO_UM(val) \
    (STATIC_CAST(long_int_t,val) * 340 * SYST_PERIOD / 2)

#endif

