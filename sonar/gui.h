#ifndef GUI_H
#define GUI_H

#include "types.h"

/* --------------------
 * Public functions
 * --------------------
 */

/*
 * Initializes the gui module.
 */
extern void gui_init();

/*
 * Changes the zoom level to the next one.
 */
extern void gui_change_zoom_level();

/*
 * Sets the current position of the motor and the distance that has been
 * measured for that distance.
 */
extern void gui_set_position(int_t pos, int_t distance);

/*
 * Shows on the screen the calibration message.
 */
extern void gui_show_calibration_message();

/*
 * Initializes the interface with all the static texts and background.
 */
extern void gui_interface_init();

/*
 * Initializes the gui module.
 */
extern void gui_init();

/*
 * Refreshes the screen.
 */
extern void gui_refresh();

#endif
