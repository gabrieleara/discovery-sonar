/*
 * graphic.c
 *
 * This file contains all the functions and tasks that interact with the lcd
 * monitor used for this project.
 *
 * */

/* ---------------------------
 * Includes
 * ---------------------------
 */

#include <math.h>

#include "stm32f4_discovery_lcd.h"

#include "constants.h"
#include "lcd/widget_config.h"
#include "lcd/widget.h"
#include "gui.h"

/* ---------------------------
 * Constants
 * ---------------------------
 */

#define GUI_ZOOM_LEVEL_START    (0)
#define GFX_ZOOM_LEVEL_MAX    (sizeof(_gui_max_distance) / sizeof(int_t))

/*
 * Defines the maximum distance for each zoom level used in the program,
 * expressed in centimeters.
 *
 * They shall be:
 * - divisible by 2;
 * - integer;
 * - no more than 3 digits long,
 * - no more than 10.
 */
static const int_t _gui_max_distance[] =
{
    600,
    400,
    200,
    100,
    50,
    20,
};

// Used to convert a distance_unit_t to string.
static const char_t _gui_unit_strings[2][3] = {"m", "cm"};


/* --------------------
 * Data types
 * --------------------
 */

/*
 * Defines the possible units that need to be displayed when changing zoom level
 */
typedef enum
{
    UNIT_METER = 0,     // The zero is used to convert it to string
    UNIT_CENTIMETER
} d_unit_t;

/*
 * Defines all the global variables that are needed for this module
 */
typedef struct GUI_STRUCT
{
    int_t   zoom_level;         // The current zoom level
    bool_t  zoom_level_changed; // If the zoom level changed or not since last
                                // refresh
    int_t   motor_pos;          // The current position of the motor
} gui_state_t;


/* --------------------
 * Globals
 * --------------------
 */

gui_state_t gui_state =
{
    .zoom_level = GUI_ZOOM_LEVEL_START,
    .zoom_level_changed = true,         // This way the first refresh will
                                        // update all the gui
    .motor_pos  = 0,
};


/* --------------------
 * Macros
 * --------------------
 */

/*
 * Returns the maximum distance that can be displayed for the given zoom
 * level.
 */
#define ZOOM_LEVEL_MAX_DISTANCE(zoom_level) (_gui_max_distance[zoom_level])

/*
 * Returns the distance unit that has to be displayed for the given zoom level.
 */
#define ZOOM_LEVEL_TO_UNIT(zoom_level) \
    ((ZOOM_LEVEL_MAX_DISTANCE(zoom_level) > 100 ? UNIT_METER : UNIT_CENTIMETER))


/*
 * Converts a distance unit identifier to string
 */
#define D_UNIT_TO_STRING(unit) (_gui_unit_strings[unit])


/* --------------------
 * Public functions
 * --------------------
 */

/*
 * Initializes the gui module.
 */
void gui_init()
{
    STM32f4_Discovery_LCD_Init();
}


/*
 * Changes the zoom level to the next one.
 */
void gui_change_zoom_level()
{
    gui_state.zoom_level = (gui_state.zoom_level + 1) % GFX_ZOOM_LEVEL_MAX;
    gui_state.zoom_level_changed = true;
}

/*
 * Sets the current position of the motor and the distance that has been
 * measured for that distance.
 */
void gui_set_position(int_t pos, int_t distance)
{
    gui_state.motor_pos = pos;

    widget_sonar_set_object(&widgets[WID_SONAR], pos, distance);
}

/*
 * Shows on the screen the calibration message.
 */
void gui_show_calibration_message()
{
    widget_draw_background(&widgets[WID_BACK]);

    widget_print(&widgets[WID_CALIB1]);
    widget_print(&widgets[WID_CALIB2]);
    widget_print(&widgets[WID_CALIB3]);
}

/*
 * Initializes the interface with all the static texts and background.
 */
void gui_interface_init()
{
    widget_draw_background(&widgets[WID_BACK_WITH_INT]);

    widget_print(&widgets[WID_TITLE1]);
    widget_print(&widgets[WID_TITLE2]);
    widget_print(&widgets[WID_ZOOM_LABEL1]);
    widget_print(&widgets[WID_ZOOM_LABEL2]);
    widget_print(&widgets[WID_UNIT_LABEL1]);
    widget_print(&widgets[WID_UNIT_LABEL2]);
    widget_print(&widgets[WID_DIST0]);
}

/*
 * Refreshes the screen.
 */
void gui_refresh()
{
    if(gui_state.zoom_level_changed)
    {
        gui_state.zoom_level_changed = false;

        int_t       zoom_level  = gui_state.zoom_level;
        d_unit_t    zoom_unit   = ZOOM_LEVEL_TO_UNIT(zoom_level);
        int_t       dist        = ZOOM_LEVEL_MAX_DISTANCE(zoom_level);

        if (zoom_unit == UNIT_METER)
            dist /= 100;

        widget_print_num(&widgets[WID_DIST1], dist);
        widget_print_num(&widgets[WID_DIST4], dist);

        dist /= 2;

        widget_print_num(&widgets[WID_DIST2], dist);
        widget_print_num(&widgets[WID_DIST3], dist);

        widget_print_num(&widgets[WID_ZOOM], zoom_level);

        widget_print_text(&widgets[WID_UNIT], D_UNIT_TO_STRING(zoom_unit));

        widget_sonar_set_max_dist(
                &widgets[WID_SONAR],
                ZOOM_LEVEL_MAX_DISTANCE(gui_state.zoom_level)
                );
    }

    widget_sonar_refresh(&widgets[WID_SONAR], gui_state.motor_pos);
}
