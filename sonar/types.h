#ifndef TYPES_H
#define TYPES_H

/* -----------------------------------------------------------------------------
 * This header contains a simulation of the data types in C++ standard library
 * and the definition of the integer types generally used by the program.
 *
 * The types have been chosen in such a way that they are compatible with
 * expected type of the arguments of system calls.
 * -----------------------------------------------------------------------------
 */

// Contains NULL declaration
#include <stddef.h>
#include <stdint.h>

// Defines a boolean type to be used in C
typedef enum { false, true } bool_t;

// Converts an expression to bool_t
#define BOOL(expr) (!!(expr))

// Definition of basic integer data types used in this program

typedef int16_t     int_t;      // Default integer type
typedef uint16_t    uint_t;     // Default unsigned integer type
typedef uint8_t     char_t;     // Default char type

typedef uint_t      color_t;    // Default color type

// Executes the cast of the expression expr to the type type
#define STATIC_CAST(type, expr) ((type)(expr))

#endif
