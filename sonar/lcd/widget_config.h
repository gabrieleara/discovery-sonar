/*
 * widget_config.h
 *
 * This file contains all declaration of public functions and global variables
 * defined in the widget_config.c file and all the constants used within it.
 *
 */

#ifndef WIDGET_CONFIG_H
#define WIDGET_CONFIG_H

#include "widget.h"

// Backgrounds
#define WID_BACK            0
#define WID_BACK_WITH_INT   1

// Calibration text
#define WID_CALIB1          2
#define WID_CALIB2          3
#define WID_CALIB3          4

// Static texts on the screen
#define WID_TITLE1          5
#define WID_TITLE2          6

#define WID_ZOOM_LABEL1     7
#define WID_ZOOM_LABEL2     8

#define WID_UNIT_LABEL1     9
#define WID_UNIT_LABEL2     10

#define WID_DIST0           11

// Dynamic texts, change with button press

#define WID_DIST1           12
#define WID_DIST2           13
#define WID_DIST3           14
#define WID_DIST4           15

#define WID_ZOOM            16
#define WID_UNIT            17

// Sonar widget, change each frame
#define WID_SONAR           18


#define WID_NUM             19

/*
 * All the widgets inside the screen:
 */
extern widget_t widgets[WID_NUM];


// ---------------------- Colors -----------------------

// NOTE: notation is 16-bit RGB 5-6-5

#define WID_COLOR_TEXT  (0x6d66u)       // Light green
#define WID_COLOR_POINT (0xb9a8u)       // Reddish purple


#endif
