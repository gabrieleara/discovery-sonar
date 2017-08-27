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

#include "ee.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "lcd_log.h"

#include <stdio.h>

#include "constants.h"
#include "sensor.h"

bool_t hey = false;

int32_t sensor_left = SENSOR_DIST_MAX;
int32_t sensor_right = SENSOR_DIST_MAX;


ISR2(systick_handler)
{
    /* count the interrupts, waking up expired alarms */
    CounterTick(sysCounter);

    sensors_read();
}

static int32_t dist = SENSOR_DIST_MAX;

TASK(TaskOut)
{
    static bool_t keepalive = false;

    LCD_UsrLog("Right: %d ticks.\r\n", sensor_right);
    LCD_UsrLog("Left %d ticks.\r\n", sensor_left);
    LCD_UsrLog("Keepalive %d.\r\n", keepalive);

    keepalive = ! keepalive;
}

TASK(TaskIncrement)
{
    sensors_trigger();
    dist = sensors_get_last_distance();

    // TODO: stuff with the distance
}

TASK(TaskStopTrigger)
{
    sensors_trigger();

    // TODO: stuff with the distance
}

int main(void)
{

	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	 * Setup the microcontroller system.
	 * Initialize the Embedded Flash Interface, the PLL and update the
	 * SystemFrequency variable.
	 * For default settings look at:
	 * pkg/mcu/st_stm32_stm32f4xx/src/system_stm32f4xx.c
	 */
	SystemInit();

	/*Initialize Erika related stuffs*/
	EE_system_init();

	/*Initialize systick */
	EE_systick_set_period(MICROSECONDS_TO_TICKS(SYST_PERIOD, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	/*Initialize the LCD*/
    STM32f4_Discovery_LCD_Init();
    LCD_LOG_Init();
    LCD_LOG_SetHeader("Hi ... Erika is running!");
    LCD_LOG_SetFooter("Erika RTOS LCD log Demo");
    LCD_UsrLog("Test-> Log Initialized!\r\n");
    LCD_DbgLog("Test-> DBG message!\r\n");
    LCD_ErrLog("Test-> ERR message!\r\n");

	sensors_init();

	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	// TODO: constants
	SetRelAlarm(Task1Alarm, 10, MOTOR_PERIOD_TICKS);
	SetRelAlarm(Task2Alarm, 20, MOTOR_PERIOD_TICKS);
	SetRelAlarm(TaskOutAlarm, 100, 1000 * 1000 / SYST_PERIOD);


	/* Forever loop: background activities (if any) should go here */
	for (;;);

}

