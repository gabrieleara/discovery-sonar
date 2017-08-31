/*
 * widget.h
 *
 * This file contains all declaration of public functions and data types defined
 * in the widget.c file.
 *
 */

#ifndef WIDGET_H
#define WIDGET_H

#include "fonts.h"
#include "../types.h"

/* ---------------------------
 * Data types
 * ---------------------------
 */

/*
 * Determines the type of a widget, which can be a background, a text widget or
 * the sonar widget.
 */
typedef enum
{
    WIDGET_BACK,
    WIDGET_TEXT,
    WIDGET_SONAR,
} wid_type_t;

/*
 * Contains all the informations of a widget. It has also a pointer to
 * additional data used for text widgets and the sonar widget.
 */
typedef struct WID_STRUCT
{
    const int_t     posx;
    const int_t     posy;
    const int_t     width;
    const int_t     height;
    char_t*         background;

    wid_type_t type;

    void* data;
} widget_t;


/*
 * The alignment that must be specified for a text widget.
 */
typedef enum
{
    PRINT_LEFT,
    PRINT_RIGHT,
    PRINT_MIDDLE_LEFT,
    PRINT_MIDDLE_RIGHT,
} text_print_t;

/*
 * Contains all the informations needed to print a given text on the screen,
 * redrawing also its background if needed.
 */
typedef struct WID_TEXT_STRUCT
{
    bool_t  is_static;          // Tells whether the text changes or not
    char_t  string[32];         // The last text printed (or the static text)

    sFONT*  font;

    text_print_t print_type;    // Used only for non static texts.
} widget_text_t;

/*
 * Contains all the sonar widget informations.
 */
typedef struct WID_SONAR_STRUCT
{
    // TODO: the following field is just to not have an error
    const int_t line_length;
    const int_t pivot_x;
    const int_t pivot_y;
    int_t       pos;
    int_t       max_distance;
    int_t       objects[171]; // TODO: constant
} widget_sonar_t;


/* ---------------------------
 * Public functions
 * ---------------------------
 */

/*
 * Draws a widget background if present.
 */
extern void widget_draw_background(widget_t* wid);

/*
 * Prints a text in the given widget. If the string is NULL or the widget is
 * static, prints the content of the widget.
 */
extern void widget_print_text(widget_t* wid, const char_t* str);

/*
 * Prints the content of a text widget.
 */
extern void widget_print(widget_t* wid);


/*
 * Prints a number in the given widget, taking into account the alignment of the
 * widget.
 */
extern void widget_print_num(widget_t* wid, int_t num);

// TODO: documentation
extern void widget_sonar_refresh(widget_t* wid, int_t pos);

extern void widget_sonar_set_object(widget_t* wid, int_t pos, int_t distance);
extern void widget_sonar_set_max_dist(widget_t* wid, int_t max_distance);

#endif
