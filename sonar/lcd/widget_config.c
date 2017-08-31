// Contains NULL declaration
#include <stddef.h>

#include "widget_config.h"
#include "../constants.h"
#include "../res/pictures.h"
#include "../motor.h"

/* ---------------------------
 * Constants
 * ---------------------------
 */

// ------- Screen size and start of coordinates ------

#define WID_SCREEN_X    (0)
#define WID_SCREEN_Y    (0)

#define WID_SCREEN_MX   (320)
#define WID_SCREEN_MY   (240)

// ------------- Text messages positions -------------

// ------------- Static texts positions --------------


// Calibration message
#define WID_MSG_CALIB1  "Align the arm"
#define WID_MSG_CALIB2  "with the guidelines,"
#define WID_MSG_CALIB3  "then press the button."

// They start all at the same x, different y
#define WID_CALIB_X     (WID_SCREEN_MX / 32)
#define WID_CALIB1_Y    (WID_SCREEN_MY / 4)
#define WID_CALIB2_Y    (WID_CALIB1_Y + 24)
#define WID_CALIB3_Y    (WID_CALIB2_Y + 24)



// Program title
#define WID_MSG_TITLE1  "Discovery Radar"
#define WID_MSG_TITLE2  "v" SOFTWARE_VERSION

// They start all at the same x, different y
#define WID_TITLE_X     (12)
#define WID_TITLE1_Y    (WID_SCREEN_MY - 38)
#define WID_TITLE2_Y    (WID_TITLE1_Y + 16)



// Zoom indicator label
#define WID_MSG_ZOOM_LABEL1   "Zoom"
#define WID_MSG_ZOOM_LABEL2   "level:"

// They start all at the same x, different y
#define WID_ZOOM_LABEL_X      (WID_SCREEN_MX - 54)
#define WID_ZOOM_LABEL1_Y     (12)
#define WID_ZOOM_LABEL2_Y     (WID_ZOOM_LABEL1_Y + 16)



// Unit indicator label
#define WID_MSG_UNIT_LABEL1  "Scale"
#define WID_MSG_UNIT_LABEL2  "unit:"

// They start all at the same x, different y
#define WID_UNIT_LABEL_X      (WID_SCREEN_MX - 54)
#define WID_UNIT_LABEL1_Y     (WID_SCREEN_MY / 2 + 24)
#define WID_UNIT_LABEL2_Y     (WID_UNIT_LABEL1_Y + 16)


// Start of coordinates indicator
#define WID_MSG_DIST0       " 0"

#define WID_DIST0_X         ((WID_SCREEN_MX-64) / 2 - 12)
// The y is defined down there

// ------------- Dynamic texts positions ---------------


/*
 * Distance indicators:
 * - the first and the latter show the maximum displayed distance with the
 *   current zoom level;
 * - the second and the third show the middle distance;
 * - the one called 0 is displayed between the second and the third and is never
 *   changed.
 */


// They start all at the same y, different x
#define WID_DIST_Y      (WID_SCREEN_MY / 2 + 28)
#define WID_DIST1_X     (8)
#define WID_DIST2_X     (WID_DIST1_X + 52)
#define WID_DIST3_X     (WID_DIST0_X + 57)
#define WID_DIST4_X     (WID_DIST3_X + 50)

#define WID_DIST_W      (30)        // NOTE: it depends on the font
#define WID_DIST_H      (12)        // NOTE: it depends on the font


/*
 * Zoom level indicator, it shows the current zoom level, which is a single
 * digit number.
 */
#define WID_ZOOM_X      (WID_ZOOM_LABEL_X + 8)
#define WID_ZOOM_Y      (WID_ZOOM_LABEL2_Y + 32)
/*
 * The background of this indicator spans these positions:
 * x = 274      y = 60
 * width = 32   height = 48
 */
#define WID_ZOOM_W      (32)        // NOTE: it depends on the font
#define WID_ZOOM_H      (48)        // NOTE: it depends on the font



/*
 * Unit indicator, it shows the unit used to express the distance labels on the
 * sonar.
 */
#define WID_UNIT_X    (WID_UNIT_LABEL_X + 4)
#define WID_UNIT_Y    (WID_UNIT_LABEL2_Y + 24)

/*
 * The background of this indicator spans these positions:
 * x = 270      y = 184
 * width = 40   height = 24
 */
#define WID_UNIT_W    (40)   // NOTE: it depends on the font
#define WID_UNIT_H    (24)   // NOTE: it depends on the font





// ------------- Other dynamic elements ----------------


/*
 * Sonar indicator, it shows the identified objects.
 *
 * The background of the sonar spans these positions:
 * x = 10       y = 10
 * width = 234  height = 122
 */
#define WID_SONAR_X    (10)
#define WID_SONAR_Y    (10)
#define WID_SONAR_W    (234)
#define WID_SONAR_H    (122)

// The pivot element is the center of the sonar, from which it starts the range

#define WID_SONAR_PIVOT_X   (WID_SCREEN_MX / 5 * 2)
#define WID_SONAR_PIVOT_Y   (WID_SCREEN_MY / 2 + 10)

// The length of the line that indicates the motor position
#define WID_SONAR_LINE_LENGTH (113)








// ----------------------- Fonts -----------------------

/*
 * Available fonts:
 * - Font48x72;
 * - Font32x48;
 * - Font24x36;
 * - Font16x24;
 * - Font12x12;
 * - Font8x12;
 * - Font8x8;
 */
#define WID_FONT_CALIB      (&Font12x12)
#define WID_FONT_TITLE      (&Font12x12)
#define WID_FONT_ZOOM_LABEL (&Font8x12)
#define WID_FONT_UNIT_LABEL (&Font8x12)
#define WID_FONT_DIST       (&Font8x12)
#define WID_FONT_ZOOM       (&Font32x48)
#define WID_FONT_UNIT       (&Font16x24)


/* ---------------------------
 * Objects
 * ---------------------------
 */

// Calibration text

static widget_text_t calib1 =
{
    .is_static = true,
    .string = WID_MSG_CALIB1,
    .font = WID_FONT_CALIB,
    .print_type = PRINT_LEFT,
};

static widget_text_t calib2 =
{
    .is_static = true,
    .string = WID_MSG_CALIB2,
    .font = WID_FONT_CALIB,
    .print_type = PRINT_LEFT,
};

static widget_text_t calib3 =
{
    .is_static = true,
    .string = WID_MSG_CALIB3,
    .font = WID_FONT_CALIB,
    .print_type = PRINT_LEFT,
};

// Title
static widget_text_t title1 =
{
    .is_static = true,
    .string = WID_MSG_TITLE1,
    .font = WID_FONT_TITLE,
    .print_type = PRINT_LEFT,
};

static widget_text_t title2 =
{
    .is_static = true,
    .string = WID_MSG_TITLE2,
    .font = WID_FONT_TITLE,
    .print_type = PRINT_LEFT,
};

static widget_text_t zoom_label1 =
{
    .is_static = true,
    .string = WID_MSG_ZOOM_LABEL1,
    .font = WID_FONT_ZOOM_LABEL,
    .print_type = PRINT_LEFT,
};

static widget_text_t zoom_label2 =
{
    .is_static = true,
    .string = WID_MSG_ZOOM_LABEL2,
    .font = WID_FONT_ZOOM_LABEL,
    .print_type = PRINT_LEFT,
};


static widget_text_t unit_label1 =
{
    .is_static = true,
    .string = WID_MSG_UNIT_LABEL1,
    .font = WID_FONT_UNIT_LABEL,
    .print_type = PRINT_LEFT,
};

static widget_text_t unit_label2 =
{
    .is_static = true,
    .string = WID_MSG_UNIT_LABEL2,
    .font = WID_FONT_UNIT_LABEL,
    .print_type = PRINT_LEFT,
};

static widget_text_t dist0 =
{
    .is_static = true,
    .string = WID_MSG_DIST0,
    .font = WID_FONT_DIST,
    .print_type = PRINT_LEFT,
};

// Dynamic texts

static widget_text_t dist1 =
{
    .is_static = false,
    .string = "",
    .font = WID_FONT_DIST,
    .print_type = PRINT_LEFT,
};

static widget_text_t dist2 =
{
    .is_static = false,
    .string = "",
    .font = WID_FONT_DIST,
    .print_type = PRINT_MIDDLE_RIGHT
};

static widget_text_t dist3 =
{
    .is_static = false,
    .string = "",
    .font = WID_FONT_DIST,
    .print_type = PRINT_MIDDLE_LEFT
};
static widget_text_t dist4 =
{
    .is_static = false,
    .string = "",
    .font = WID_FONT_DIST,
    .print_type = PRINT_RIGHT
};

static widget_text_t zoom =
{
    .is_static = false,
    .string = "",
    .font = WID_FONT_ZOOM,
    .print_type = PRINT_LEFT,
};

static widget_text_t unit =
{
    .is_static = false,
    .string = "",
    .font = WID_FONT_UNIT,
    .print_type = PRINT_LEFT,
};

static widget_sonar_t sonar =
{
    .line_length = WID_SONAR_LINE_LENGTH,
    .pivot_x = WID_SONAR_PIVOT_X,
    .pivot_y = WID_SONAR_PIVOT_Y,
    .pos = 0,
    .objects = { [0 ... USR_MAX_POS] = 113 }, // TODO: change DISPLAY_DIST_MAX_CM
};


/*
 * All the widgets inside the screen:
 */
widget_t widgets[WID_NUM] =
{
    {WID_SCREEN_X, WID_SCREEN_Y, WID_SCREEN_MX, WID_SCREEN_MY, background, WID_BACK, NULL},
    {WID_SCREEN_X, WID_SCREEN_Y, WID_SCREEN_MX, WID_SCREEN_MY, background_with_interface, WID_BACK, NULL},

    {WID_CALIB_X, WID_CALIB1_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &calib1},
    {WID_CALIB_X, WID_CALIB2_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &calib2},
    {WID_CALIB_X, WID_CALIB3_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &calib3},

    {WID_TITLE_X, WID_TITLE1_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &title1},
    {WID_TITLE_X, WID_TITLE2_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &title2},

    {WID_ZOOM_LABEL_X, WID_ZOOM_LABEL1_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &zoom_label1},
    {WID_ZOOM_LABEL_X, WID_ZOOM_LABEL2_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &zoom_label2},

    {WID_UNIT_LABEL_X, WID_UNIT_LABEL1_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &unit_label1},
    {WID_UNIT_LABEL_X, WID_UNIT_LABEL2_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &unit_label2},

    {WID_DIST0_X, WID_DIST_Y, 0, 0, NULL, WIDGET_TEXT, (void*) &dist0},

    {WID_DIST1_X, WID_DIST_Y, WID_DIST_W, WID_DIST_H, background_dist1, WIDGET_TEXT, (void*) &dist1},
    {WID_DIST2_X, WID_DIST_Y, WID_DIST_W, WID_DIST_H, background_dist2, WIDGET_TEXT, (void*) &dist2},
    {WID_DIST3_X, WID_DIST_Y, WID_DIST_W, WID_DIST_H, background_dist3, WIDGET_TEXT, (void*) &dist3},
    {WID_DIST4_X, WID_DIST_Y, WID_DIST_W, WID_DIST_H, background_dist4, WIDGET_TEXT, (void*) &dist4},

    {WID_ZOOM_X, WID_ZOOM_Y, WID_ZOOM_W, WID_ZOOM_H, background_zoom, WIDGET_TEXT, (void*) &zoom},
    {WID_UNIT_X, WID_UNIT_Y, WID_UNIT_W, WID_UNIT_H, background_unit, WIDGET_TEXT, (void*) &unit},

    {WID_SONAR_X, WID_SONAR_Y, WID_SONAR_W, WID_SONAR_H, background_sonar, WIDGET_SONAR, (void*) &sonar},
};
