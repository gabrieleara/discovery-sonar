/*
 * sensor.c
 *
 * This file contains all the functions that interact with the two ultra sonic
 * sensors connected to the board.
 *
 * */

/* ---------------------------
 * Includes
 * ---------------------------
 */

#include <stdlib.h>

#include "constants.h"
#include "sensor.h"

/* ---------------------------
 * Public functions
 * ---------------------------
 */

/*
 * Checks if at the moment of sending a new trigger the previous echo was
 * already finished. If not, marks the previous value as an error and
 * invalidates next record.
 * */
void check_finished(sensor_t* sensor, bool_t recording, bool_t trig_sent)
{
    // Since in state SENSOR_ECHO_LONG trig_sent cannot be true
    if(sensor->echo_state == SENSOR_ECHO_LONG)
        trig_sent = false;
    
    // Update state
    if(recording)
    {
        // Echo is taking too long to complete
        sensor->echo_state = SENSOR_ECHO_LONG;

    } else if(trig_sent)
    {
        // Echo didn't arrive even if we sent a trigger
        sensor->echo_state = SENSOR_ECHO_LOST;

    } else if(sensor->echo_state == SENSOR_ECHO_LONG)
    {
        // Echo finished, but it doesn't need to be considered, since indeed
        // it's a previous echo influencing this window.

        // Hopefully the next measurement will be fine!

        sensor->echo_state = SENSOR_ECHO_NEXT_OK;

    } else
    {
        // The measured value is perfectly fine and can be used.
        sensor->echo_state = SENSOR_ECHO_OK;
    }

    // Update output
    if(sensor->echo_state != SENSOR_ECHO_LONG)
    {
        sensor->trigger_sent = true;
    }
    else
        sensor->trigger_sent = false;
}

/*
 * Calculates the current distance measured by the system based
 * on the two distances measured by each sensor.
 */
int_t triangolation_original(int_t dist_lx, int_t dist_rx)
{
    int_t distance;

    if(dist_lx >= SENSOR_DIST_MAX && dist_rx >= SENSOR_DIST_MAX )
    {
        // No objects in sight
        distance = SENSOR_DIST_MAX;
    } else
    {
        // At least one object is in sight
        if (abs(dist_lx-dist_rx)> SENSORS_SEPARATION*SENSORS_MARGIN)
        {
            // There are multiple objects in sight
            distance = (dist_lx < dist_rx) ? dist_lx : dist_rx;
        } else
        {
            // There is only a single object in sight
            distance = (dist_lx + dist_rx) / 2;
        }
    }

    return distance;
}

int_t triangolation_fixed(int_t dist_lx, int_t dist_rx)
{
    int_t distance;

    if(dist_lx < 0)
        dist_lx = 0;
    
    if(dist_rx < 0)
        dist_rx = 0;

    if(dist_lx >= SENSOR_DIST_MAX && dist_rx >= SENSOR_DIST_MAX )
    {
        // No objects in sight
        distance = SENSOR_DIST_MAX;
    } else
    {
        // At least one object is in sight
        if (abs(dist_lx-dist_rx)> SENSORS_SEPARATION*SENSORS_MARGIN)
        {
            // There are multiple objects in sight
            distance = (dist_lx < dist_rx) ? dist_lx : dist_rx;
        } else
        {
            // There is only a single object in sight
            distance = (dist_lx + dist_rx) / 2;
        }
    }

    return distance;
}