/*
 * sensor.c
 *
 * This file contains all the functions and tasks that interact with the servo
 * motor used for this project.
 *
 * */

/* ---------------------------
 * Includes
 * ---------------------------
 */

#include "lib/tm_stm32f4_gpio.h"

#include "constants.h"
#include "sensor.h"

/* ---------------------------
 * Private constants
 * ---------------------------
 */

// Defines the GPIO ports and pins used to send trigger signals and reach echos
#define SENSOR_LX_TRIG_PORT     (GPIOE)
#define SENSOR_LX_TRIG_PIN      (GPIO_Pin_6)

#define SENSOR_RX_TRIG_PORT     (GPIOE)
#define SENSOR_RX_TRIG_PIN      (GPIO_Pin_4)

#define SENSOR_LX_ECHO_PORT     (GPIOE)
#define SENSOR_LX_ECHO_PIN      (GPIO_Pin_2)

#define SENSOR_RX_ECHO_PORT     (GPIOE)
#define SENSOR_RX_ECHO_PIN      (GPIO_Pin_5)

// Undefine or set to zero this to use two different ports for the triggers
#define SENSOR_TRIG_SAME_PORT   1

// Undefine or set to zero this to use two different ports for the echoes
#define SENSOR_ECHO_SAME_PORT   1

/* ---------------------------
 * Data types
 * ---------------------------
 */

typedef uint16_t        pin_t;
typedef GPIO_TypeDef    port_t;

enum sensor_id { SENSOR_LX = 0, SENSOR_RX = 1 };

typedef struct SENSOR_STRUCT
{
    bool_t      trig_sent;      // Whether the trigger signal has been sent or
                                // not
    bool_t      recording;      // Whether the sensor is waiting the end of the
                                // echo or not
    int_t    last_distance;  // Last calculated distance in number of ticks
    int_t    time_counter;   // Counter used to count the number of ticks
                                // since last start of recording

    bool_t      skipped;        // Whether the last trigger has been skipped or
                                // not because the previous echo was too long,
                                // this should never get to be true

    bool_t      skipping;       // TODO:

    bool_t      previous_value; // The previous value of the echo, to check
                                // whether a rising edge arrived

    port_t*     echo_port;      // Echo port identifier
    pin_t       echo_pin;       // Echo pin identifier
} sensor_t;

typedef struct SENSOR_STATE_STRUCT
{
    sensor_t    sensors[2];
    int_t     last_distance;
} sensor_state_t;


/* ---------------------------
 * Global variables
 * ---------------------------
 */

#define RESET_SENSOR {\
.trig_sent = false,\
.recording = false,\
.last_distance = SENSOR_DIST_MAX,\
.time_counter = 0,\
.skipped = false,\
}



static sensor_state_t sensor_state =
{
    .last_distance = SENSOR_DIST_MAX,
    .sensors = { RESET_SENSOR, RESET_SENSOR },
};

/* ---------------------------
 * Private functions
 * ---------------------------
 */

/*
 * Gets the value of the sensor echo pin. Returns true if the pin is high, false
 * otherwise.
 * */
bool_t get_echo_value(sensor_t* sensor)
{
    return BOOL(TM_GPIO_GetInputPinValue((sensor->echo_port), (sensor->echo_pin)));
}

/*
 * Reads a sensor value, starting to count if a positive edge is encountered,
 * stopping if instead it's a negative edge.
 * As soon as it stops it updates the sensor last distance.
 * */
void read_sensor(sensor_t* sensor)
{
    bool_t  echo_value;

    echo_value = get_echo_value(sensor);

    if(sensor->recording)
    {
        ++sensor->time_counter;

        if(!echo_value)
        {
            // I have to stop recording!
            sensor->recording = false;
            sensor->last_distance = sensor->time_counter;
        }
    }
    else if(sensor->trig_sent && echo_value /* && !sensor->skipped */)
    {
        sensor->time_counter = 0;
        sensor->recording = true;
        sensor->trig_sent = false;
    }
}


/*
 * Sends the trigger signal to both sensors.
 * */
void send_trigger()
{
#if SENSOR_TRIG_SAME_PORT
    TM_GPIO_SetPinHigh(SENSOR_LX_TRIG_PORT, SENSOR_LX_TRIG_PIN | SENSOR_RX_TRIG_PIN);
#else
    TM_GPIO_SetPinHigh(SENSOR_LX_TRIG_PORT, SENSOR_LX_TRIG_PIN);
    TM_GPIO_SetPinHigh(SENSOR_RX_TRIG_PORT, SENSOR_RX_TRIG_PIN);
#endif
}

/*
 * Stops the trigger signal to both sensors.
 * */
void stop_trigger()
{
#if SENSOR_TRIG_SAME_PORT
    TM_GPIO_SetPinLow(SENSOR_LX_TRIG_PORT, SENSOR_LX_TRIG_PIN | SENSOR_RX_TRIG_PIN);
#else
    TM_GPIO_SetPinLow(SENSOR_LX_TRIG_PORT, SENSOR_LX_TRIG_PIN);
    TM_GPIO_SetPinLow(SENSOR_RX_TRIG_PORT, SENSOR_RX_TRIG_PIN);
#endif
}

/*
 * Checks if at the moment of sending a new trigger the previous echo was
 * already finished. If not, marks the previous value as an error and
 * invalidates next record. TODO: invalidate next record.
 * */
void check_finished(sensor_t* sensor)
{
    sensor->skipped = false;



    // If still recording or no echo arrived at all
    if(sensor->recording || sensor->trig_sent)
    {
        // Should never happen, if it happens this is the handle
        sensor->last_distance = SENSOR_DIST_MAX;
        sensor->skipped = true;
    }

    // TODO: decide if keep it or not
    // sensor->recording = false;
}

/*
 * Updates the distance based on the values read by both sensors.
 */
void update_distance()
{
    // NOTICE: If both sensors skipped the last measurement, they both measured
    // SENSOR_DIST_MAX

    if(sensor_state.sensors[SENSOR_LX].skipped)
    {
        sensor_state.last_distance =
                sensor_state.sensors[SENSOR_RX].last_distance;
        return;
    }

    if(sensor_state.sensors[SENSOR_RX].skipped)
    {
        sensor_state.last_distance =
                sensor_state.sensors[SENSOR_LX].last_distance;
        return;
    }

    // TODO: check if simple average is a good approximation
    sensor_state.last_distance = (sensor_state.sensors[SENSOR_LX].last_distance
            + sensor_state.sensors[SENSOR_RX].last_distance) / 2;
}

/* ---------------------------
 * Public functions
 * ---------------------------
 */

void sensors_init()
{
    // Saving all the sensors parameters
    sensor_state.sensors[SENSOR_LX].echo_port   = SENSOR_LX_ECHO_PORT;
    sensor_state.sensors[SENSOR_LX].echo_pin    = SENSOR_LX_ECHO_PIN;

    sensor_state.sensors[SENSOR_RX].echo_port   = SENSOR_RX_ECHO_PORT;
    sensor_state.sensors[SENSOR_RX].echo_pin    = SENSOR_RX_ECHO_PIN;

    // Ports and pins initialization
#if SENSOR_TRIG_SAME_PORT
    TM_GPIO_Init(SENSOR_LX_TRIG_PORT,
            SENSOR_LX_TRIG_PIN | SENSOR_RX_TRIG_PIN,
            TM_GPIO_Mode_OUT,
            TM_GPIO_OType_PP,
            TM_GPIO_PuPd_NOPULL,
            TM_GPIO_Speed_High);
#else
    TM_GPIO_Init(SENSOR_LX_TRIG_PORT,
                SENSOR_LX_TRIG_PIN,
                TM_GPIO_Mode_OUT,
                TM_GPIO_OType_PP,
                TM_GPIO_PuPd_NOPULL,
                TM_GPIO_Speed_High);

    TM_GPIO_Init(SENSOR_RX_TRIG_PORT,
                SENSOR_RX_TRIG_PIN,
                TM_GPIO_Mode_OUT,
                TM_GPIO_OType_PP,
                TM_GPIO_PuPd_NOPULL,
                TM_GPIO_Speed_High);
#endif

#if SENSOR_ECHO_SAME_PORT
    TM_GPIO_Init(SENSOR_LX_ECHO_PORT,
            SENSOR_LX_ECHO_PIN | SENSOR_RX_ECHO_PIN,
            TM_GPIO_Mode_IN,
            TM_GPIO_OType_PP,
            TM_GPIO_PuPd_NOPULL,
            TM_GPIO_Speed_High);
#else
    TM_GPIO_Init(SENSOR_LX_ECHO_PORT,
                SENSOR_LX_ECHO_PIN,
                TM_GPIO_Mode_IN,
                TM_GPIO_OType_PP,
                TM_GPIO_PuPd_NOPULL,
                TM_GPIO_Speed_High);

    TM_GPIO_Init(SENSOR_RX_ECHO_PORT,
                SENSOR_RX_ECHO_PIN,
                TM_GPIO_Mode_IN,
                TM_GPIO_OType_PP,
                TM_GPIO_PuPd_NOPULL,
                TM_GPIO_Speed_High);
#endif
}

/*
 * Reads both sensors value, see the read_sensor function.
 * */
void sensors_read()
{
    read_sensor(&sensor_state.sensors[SENSOR_LX]);
    read_sensor(&sensor_state.sensors[SENSOR_RX]);
}

/*
 * Sends/stops the trigger signal. At the moment of sending the trigger it also
 * updates the global calculated distance at the previous step.
 * */
void sensors_trigger()
{
static bool_t set_reset = false;

    if(set_reset)
    {
        check_finished(&sensor_state.sensors[SENSOR_LX]);
        check_finished(&sensor_state.sensors[SENSOR_RX]);

        update_distance();

        send_trigger();

        sensor_state.sensors[SENSOR_LX].trig_sent =
                sensor_state.sensors[SENSOR_RX].trig_sent = true;
    }
    else
        stop_trigger();

    set_reset = !set_reset;
}

/*
 * Returns the last calculated distance.
 * */
int_t sensors_get_last_distance()
{
    return sensor_state.last_distance;
}
