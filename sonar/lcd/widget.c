/*
 * widget.c
 *
 * This file contains all the functions that are used to display widgets on the
 *
 */

/* ---------------------------
 * Includes
 * ---------------------------
 */

#include <math.h>

#include "stm32f4_discovery_lcd.h"

#include "widget.h"
#include "widget_config.h"
#include "../sensor.h"


/* ---------------------------
 * Private functions
 * ---------------------------
 */

/*
 * Converts a single unit number to its corresponding ASCII character.
 */
char_t num_to_char(int_t num)
{
    return STATIC_CAST(char_t, num) + 48;
}

/*
 * Returns the number of units an integer number can be represented in.
 * Return values are either 1, 2 or 3.
 */
int_t num_digits(int_t num)
{
    int_t digits = 0;

    do
    {
        ++digits;
        num /= 10;
    } while(num > 0);

    return digits;
}

/*
 * Simple replacement for the standard strcpy function, to avoid including
 * string.h.
 */
void string_cpy(unsigned char* dest, const unsigned char* src)
{
    int_t i = 0;
    do
    {
        dest[i] = src[i];
        ++i;
    } while(dest[i-1] != '\0');
}

/*
 * Shifts a string to the right by one place, by inserting spaces on the left.
 */
void string_shift(unsigned char* dest)
{
    unsigned char buf;
    unsigned char prev = ' ';
    int_t i;
    for(i = 0; prev != '\0'; ++i)
    {
        buf = dest[i];
        dest[i] = prev;
        prev = buf;
    }
    dest[i] = prev;
}

/*
 * Sets the text of a text widget, if the widget is indeed a text widget and is
 * not static.
 */
void set_text(widget_t* wid, const unsigned char* str)
{
    widget_text_t* ptr;

    if(str == NULL)
        return;

    if(wid->type != WIDGET_TEXT)
        return;

    ptr = STATIC_CAST(widget_text_t*, wid->data);

    if(ptr->is_static)
        return;

    string_cpy(ptr->string, str);
    ptr->string[sizeof(ptr->string)-1] = '\0'; // Just in case
}


/*
 * Prints a text widget content, if the widget is indeed a text widget.
 */
void print_text(widget_t* wid)
{
    widget_text_t* ptr;

    if(wid->type != WIDGET_TEXT)
        return;

    ptr = STATIC_CAST(widget_text_t*, wid->data);

    if(!ptr->is_static)
        widget_draw_background(wid);

    LCD_SetFont(ptr->font);

    LCD_SetTextColor(WID_COLOR_TEXT);

    LCD_DisplayStringXY(wid->posx, wid->posy, ptr->string);
}

/*
 * Prints the content of a text widget.
 */
void widget_print(widget_t* wid)
{
    widget_print_text(wid, NULL);
}


/*
 * Converts the given number into a string and sets it as the content of the
 * given text widget, if the widget is indeed a text widget and it is not
 * static.
 *
 * This functions aligns also the text depending on the alignment specified in
 * the widget configuration.
 */
void set_num_text(widget_t* wid, int_t num)
{
    widget_text_t* ptr;
    int_t i, digits;

    if(wid->type != WIDGET_TEXT)
        return;

    ptr = STATIC_CAST(widget_text_t*, wid->data);

    if(ptr->is_static)
        return;

    i = 0;

    digits = num_digits(num);

    switch(digits)
    {
    case 3:
        ptr->string[i] = num_to_char(num / 100);

        num %= 100;
        ++i;
    case 2:
        ptr->string[i] = num_to_char(num / 10);

        num %= 10;
        ++i;
    case 1:
        ptr->string[i] = num_to_char(num);
        ++i;
        ptr->string[i] = '\0';
    }

    switch(ptr->print_type)
    {
    case PRINT_LEFT:
        // Nothing to do.
        break;
    case PRINT_RIGHT:
        // Move the string to the right if needed
        while(digits < 3)
        {
            string_shift(ptr->string);
            ++digits;
        }
        break;
    case PRINT_MIDDLE_LEFT:
        // Move the string to the right only one time if needed
        if(digits == 1)
            string_shift(ptr->string);
        break;
    case PRINT_MIDDLE_RIGHT:
        // Move the string to the right only one time if needed
        if(digits < 3)
            string_shift(ptr->string);
        break;
    }
}


// ------------------ Functions used for the sonar ------------------


#define COORDINATE_X(wid, angle, dist) \
    STATIC_CAST(int_t, cos(angle) * dist + wid->pivot_x)

#define COORDINATE_Y(wid, angle, dist) \
    STATIC_CAST(int_t, wid->pivot_y - sin(angle) * dist)

/*
 * Draws a line at the given angle.
 */
void draw_line(widget_sonar_t* wid)
{
    int_t x;
    int_t y;
    double angle = motor_pos_to_angle(wid->pos);

    x = COORDINATE_X(wid, angle, wid->line_length);
    y = COORDINATE_Y(wid, angle, wid->line_length);

    LCD_SetTextColor(WID_COLOR_TEXT);

    LCD_DrawUniLine(wid->pivot_x, wid->pivot_y, x, y);
}

/*
 * Draws a line at the given angle.
 */
void draw_points(widget_sonar_t* wid)
{
    int_t x;
    int_t y;
    int_t dist;
    double angle;
    int_t i;

    const int_t max_distance = wid->max_distance;

    //LCD_SetTextColor(WID_COLOR_POINT);

    for(i = USR_MIN_POS; i <= USR_MAX_POS; ++i)
    {
        angle = motor_pos_to_angle(i);

        dist = wid->obstacles[i];

        if(dist > max_distance)
             continue;

        dist = dist * wid->line_length / max_distance;

        x = COORDINATE_X(wid, angle, dist);
        y = COORDINATE_Y(wid, angle, dist);

        LCD_DrawFilledRect(x-1, y-1, x+1, y+1, WID_COLOR_POINT, WID_COLOR_POINT);
    }
}


/* ---------------------------
 * Public functions
 * ---------------------------
 */

/*
 * Draws a widget background if present.
 */
void widget_draw_background(widget_t* wid)
{
    if(wid->background != NULL)
    {
        LCD_DrawPicture(
                wid->posx,
                wid->posy,
                wid->width,
                wid->height,
                wid->background);
    }
}

/*
 * Prints a text in the given widget. If the string is NULL or the widget is
 * static, prints the content of the widget.
 */
void widget_print_text(widget_t* wid, const unsigned char* str)
{
    set_text(wid, str);
    print_text(wid);
}

/*
 * Prints a number in the given widget, taking into account the alignment of the
 * widget.
 */
void widget_print_num(widget_t* wid, int_t num)
{
    set_num_text(wid, num);
    print_text(wid);
}

/*
 * Refreshes the content of the sonar, displaying the bar at the specified user
 * position.
 */
void widget_sonar_refresh(widget_t* wid, int_t pos)
{
    widget_sonar_t* ptr;

    if(wid->type != WIDGET_SONAR)
        return;

    ptr = STATIC_CAST(widget_sonar_t*, wid->data);

    ptr->pos = pos;

    widget_draw_background(wid);

    draw_line(ptr);

    draw_points(ptr);
}

/*
 * Sets the measured distance at the given user position.
 */
void widget_sonar_set_obstacle(widget_t* wid, int_t pos, int_t distance)
{
    widget_sonar_t* ptr;

    if(wid->type != WIDGET_SONAR)
        return;

    ptr = STATIC_CAST(widget_sonar_t*, wid->data);
    ptr->obstacles[pos] = distance;
}

/*
 * Sets the maximum distance (in centimeters) that can be displayed. Used mainly
 * to change zoom level.
 */
void widget_sonar_set_max_dist(widget_t* wid, int_t max_distance)
{
    widget_sonar_t* ptr;

    if(wid->type != WIDGET_SONAR)
        return;

    ptr = STATIC_CAST(widget_sonar_t*, wid->data);
    ptr->max_distance = max_distance;
}
