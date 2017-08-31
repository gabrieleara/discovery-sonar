/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

/* Include core modules */
//#include "ee.h"
//#include "stm32f4xx.h"
//
///* Include my libraries here */
//#include "motor.h"
//#include "constants.h"
//#include "lib/tm_stm32f4_pwm.h"
//#include "lib/tm_stm32f4_disco.h"
//
///*
// * SysTick ISR2
// */
//ISR2(systick_handler)
//{
//	/* count the interrupts, waking up expired alarms */
//	CounterTick(myCounter);
//}
//
//TASK(TaskIOToggle)
//{
//	TM_DISCO_LedToggle(LED_RED);
//	motor_step();
//}
//
//void System_Init() {
//	SystemInit();
//	EE_system_init();
//}
//
//void Systick_Init() {
//	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
//	EE_systick_enable_int();
//	EE_systick_start();
//}
//
//void Calibrazione(void) {
//	while(!TM_DISCO_ButtonPressed())
//		motor_set_pos(USR_MID_POS);
//}
//
//int main(void) {
//	System_Init();
//	Systick_Init();
//
//	/* Initialize leds on board */
//	TM_DISCO_LedInit();
//
//	/* Initialize button on board */
//	TM_DISCO_ButtonInit();
//
//	motor_init(USR_MID_POS, LEFT);
//	Calibrazione();
//
//	SetRelAlarm(AlarmToggle, 10, 20);
//
//	while (1) {}
//}












//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


/*
 * This file contains the main and the task's definitions of the system.
 */


/* Include core modules */
#include "ee.h"

/* Include my libraries here */

#include "lib/tm_stm32f4_pwm.h"
#include "lib/tm_stm32f4_disco.h"

#include "motor.h"
#include "constants.h"

#include "gui.h"

static int_t pos = 0;

















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

    motor_step();

    pos = pos + sign;

    if (pos == 170 || pos == 0)
    {
        sign = - sign;
    }

    changes = changes+1 % 4;

    if(changes == 0)
        dist += rand() % 50 - 25;
    else
        dist += rand() % 20 - 10;

    if(dist < 0)
        dist = 0;
    else if(dist > 700)
        dist = 700;

    gui_set_position(pos, dist);

    // TODO: send sensor trigger

    // TODO: read sensor value and store with the angle
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

// TODO: wrapper
void Calibrazione(void)
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

    Calibrazione();

    gui_interface_init();

    SetRelAlarm(AlarmStep, 10, 20);

    SetRelAlarm(AlarmGui, 20, 80);

    while (1) {}
}
