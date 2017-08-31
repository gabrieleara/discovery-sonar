/*
 * sensor.h
 *
 * This file contains all declaration of public functions and global variables
 * defined in the sensor.c file.
 *
 * */

#ifndef SENSOR_H
#define SENSOR_H

/*
 * Constants
 *
 */

// TODO: change
#define SENSOR_DIST_MAX (9999)  // Maximum distance in number of ticks


/* ---------------------------
 * Public functions
 * ---------------------------
 */

extern void sensors_init();

/*
 * Reads both sensors value, see the read_sensor function.
 * */
extern void sensors_read();

/*
 * Sends/stops the trigger signal. At the moment of sending the trigger it also
 * updates the global calculated distance at the previous step.
 * */
extern void sensors_trigger();

/*
 * Returns the last calculated distance.
 * */
extern int32_t sensors_get_last_distance();


#define COUNT_TO_CM(val) (val * 340 * SYST_PERIOD / 2 / 10000)
#define COUNT_TO_MM(val) (val * 340 * SYST_PERIOD / 2 / 1000)
#define COUNT_TO_UM(val) (val * 340 * SYST_PERIOD / 2 )

#endif

