/*
 * This file contains the main and the task's definitions of the system.
 */

/* ---------------------------
 * Includes
 * ---------------------------
 */

#include "ee.h"

#include "lib/tm_stm32f4_disco.h"

#include "constants.h"

#include "motor.h"
#include "sensor.h"
#include "gui.h"

/* ---------------------------
 * Interrupt handler
 * ---------------------------
 */

static bool_t   started = false;

ISR2(systick_handler)
{
    CounterTick(sysCount);
    if(started)
        sensors_read();
}



/* ---------------------------
 * Tasks
 * ---------------------------
 */

/*
 * This task is executed each time the radar needs to move to a new position.
 * It tells the motor to move and sends a new trigger signal to both sensors.
 */
TASK(TaskStep)
{
    int_t   dist;
    int_t   pos;

    TM_DISCO_LedToggle(LED_RED);

    pos = motor_get_pos();

    if(started)
    {
        motor_step();
        sensors_trigger();

        dist = DISTANCE_TO_CM(sensors_get_last_distance());

        gui_set_position(pos, dist);
    } else
    {
        started = true;
        sensors_trigger();
    }
}

/*
 * This task is executed only to stop sending the trigger signal to sensors.
 */
TASK(TaskStopTrigger)
{
    sensors_trigger();
}

/*
 * This task is executed to refresh the screen content. It also checks if the
 * user pressed the button and notifies the gui module to change zoom level.
 */
TASK(TaskGui)
{
    if(TM_DISCO_ButtonOnPressed())
        gui_change_zoom_level();

    gui_refresh();
}

/* ---------------------------
 * Main and wrapper functions
 * ---------------------------
 */

/*
 * Initialize the system and Erika.
 */
void system_init()
{
    SystemInit();
    EE_system_init();
}

/*
 * Initialize the systick.
 */
void systick_init()
{
    EE_systick_set_period(MICROSECONDS_TO_TICKS(SYST_PERIOD, SystemCoreClock));
    EE_systick_enable_int();
    EE_systick_start();
}

/*
 * Shows a calibration message on the screen and waits the user to press the
 * button.
 */
void arm_calibration_wait(void)
{
    gui_show_calibration_message();

    while(!TM_DISCO_ButtonOnPressed())
        motor_set_pos(USR_MID_POS);
}

int main(void) {
    system_init();
    systick_init();

    // Initialize leds
    TM_DISCO_LedInit();

    // Initialize button
    TM_DISCO_ButtonInit();

    // Initialize all the program modules
    gui_init();
    sensors_init();

    // Initialize motor for calibration and wait the calibration to finish
    motor_init(MOTOR_MID, LEFT);
    arm_calibration_wait();

    // Move the motor to the initial position and initialize interface
    motor_set_pos(USR_MIN_POS);
    gui_interface_init();

    // Set alarms to trigger tasks activation
    SetRelAlarm(AlarmStep, 10, STEP_PERIOD_TICKS);
    SetRelAlarm(AlarmStep, 20, STEP_PERIOD_TICKS);
    SetRelAlarm(AlarmGui, 50, SCREEN_PERIOD_TICKS);

    // Forever loop
    while (1) {}
}
