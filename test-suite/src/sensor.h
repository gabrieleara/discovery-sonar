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

#define SENSORS_SEPARATION_CM   (3.4)
                    // The separation between the two
                    // sensors in cm, used by the
                    // triangolation approximation

#define SENSORS_SEPARATION      (SENSORS_SEPARATION_CM * 2 * 10000 / 340 / SYST_PERIOD)
                    // The separation between the two
                    // sensors in number of ticks, used by
                    // the triangolation approximation

#define SENSORS_MARGIN          (0.8)   // The margin under which two objects
                    // are considered to be the same


/*
 * Defines the possible states the sensor can be in.
 */
typedef enum
{
    SENSOR_ECHO_NEXT_OK = 0,// The previous echo value was not ok, but the
                            // following one will be hopefully good
    SENSOR_ECHO_OK,         // Everything works as expected
    SENSOR_ECHO_LOST,       // No echo arrived
    SENSOR_ECHO_LONG,       // Echo is taking too long to complete

} sensor_echo_state_t;

typedef struct
{
    sensor_echo_state_t echo_state;     // Current state
    bool_t              trigger_sent;   // Current output
} sensor_t;

// Possible inputs to state machine
typedef enum
{
    REC_TRIG = 0,
    REC_NTRIG,
    NREC_TRIG,
    NREC_NTRIG,
    STATE_INPUT_MAX,    // Used only to stop testing
} sensor_state_input_t;

// Converts an input in the pair of boolean variables
// accepted by the update state function
#define IN_TO_TRIGGER_SENT(in) (in == REC_TRIG || in == NREC_TRIG)
#define IN_TO_RECORDING(in) (in == REC_TRIG || in == REC_NTRIG)

/* ---------------------------
 * Public functions
 * ---------------------------
 */

/*
 * Checks if at the moment of sending a new trigger the previous echo was
 * already finished. If not, marks the previous value as an error and
 * invalidates next record.
 * */
extern void check_finished(sensor_t* sensor, bool_t recording, bool_t trig_sent);
 
/*
 * Calculates the current distance measured by the system based
 * on the two distances measured by each sensor.
 */
extern int_t triangolation_original(int_t dist_lx, int_t dist_rx);
extern int_t triangolation_fixed(int_t dist_lx, int_t dist_rx);

#endif

