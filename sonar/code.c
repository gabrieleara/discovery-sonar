/*
 * This file contains the main and the task's definitions of the system.
 */

/* Include core modules */
#include "ee.h"

#include "lib/tm_stm32f4_pwm.h"
#include "lib/tm_stm32f4_disco.h"

#include "motor.h"
#include "constants.h"

#include "gui.h"

static int_t    pos = 0;
static bool_t   start = true;

/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
    /* count the interrupts, waking up expired alarms */
    CounterTick(sysCount);
}

static int_t rand_seed = 15456;
static int_t rand_modulus = 1373;
static int_t rand_mul = 5651;

int rand()
{
    rand_seed = (rand_seed*rand_mul) % rand_modulus;
    return rand_seed;
}

int srand(int seed)
{
    rand_seed = seed;

    return rand();
}


/*
 * This task is called each time the radar needs to move to a new position. It
 * tells the motor to move and the sensor to send a signal.
 */
TASK(TaskStep)
{
    static int sign = 1;
    static int dist = 550;
    static int changes = 0;

    TM_DISCO_LedToggle(LED_RED);

    pos = motor_get_pos();

    if(!start)
    {
        motor_step();
        // TODO: send sensor trigger

        // TODO: read sensor value

        // TODO: remove begin
        changes = changes+1 % 4;

        if(changes == 0)
            dist += rand() % 50 - 25;
        else
            dist += rand() % 20 - 10;

        if(dist < 0)
            dist = 0;
        else if(dist > 700)
            dist = 700;
        // TODO: remove end

        gui_set_position(pos, dist);
    } else
    {
        start = false;
        // TODO: send sensor trigger
    }
}

TASK(TaskGui)
{
    if(TM_DISCO_ButtonOnPressed())
        gui_change_zoom_level();

    gui_refresh();
}


void system_init()
{
    SystemInit();
    EE_system_init();
}

void systick_init()
{
    EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
    EE_systick_enable_int();
    EE_systick_start();
}

void arm_calibration_wait(void)
{
    gui_show_calibration_message();

    while(!TM_DISCO_ButtonOnPressed())
        motor_set_pos(USR_MID_POS);
}

int main(void) {
    system_init();
    systick_init();

    /* Initialize leds on board */
    TM_DISCO_LedInit();

    /* Initialize button on board */
    TM_DISCO_ButtonInit();

    gui_init();

    motor_init(MOTOR_MID, LEFT);

    arm_calibration_wait();

    motor_set_pos(USR_MIN_POS);

    gui_interface_init();

    SetRelAlarm(AlarmStep, 10, 20);

    SetRelAlarm(AlarmGui, 20, 80);

    while (1) {}
}
