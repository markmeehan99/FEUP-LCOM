#ifndef _VIDEO_GR_
#define _VIDEO_GR_

#include <stdint.h>
#include "menu.h"

/** @defgroup Video Video
 * @{
 * Functions for manipulating graphics card
 */


/**
 * @brief Function used in lab5 for drawing pattern.
 * 
 * @param no_rectagles Number os rectangles to be displayed.
 * 
 * @param first First rectangle coordinate.
 * 
 * @param step Color step from one rectangle to another.
 * 
 * @return 0 upon success and non-zero otherwise
 */
int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Function used in lab5 for xps displaying.
 * 
 * @param xpm[] XPM to be displayed.
 * 
 * @param x Horizontal coordinate.
 * 
 * @param y Vertical coordinate.
 * 
 * @return 0 upon success and non-zero otherwise
 */
int (vg_display_xpm)(const char *xpm[],  uint16_t x, uint16_t y);

/**
 * @brief Sets pixel color to a specified one.
 * 
 * @param x Horizontal coordinate of pixel.
 * 
 * @param y Vertical coordinate of pixel.
 * 
 * @ param color Color wished to be set.
 */
int set_color(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Getter for horizontal resolution.
 */
int getHorResolution();

/**
 * @brief Getter for vertical resolution.
 */
int getVerResolution();

/**
 * @brief Double buffer, allows for smooth images.
 */
void copy_draw();

/**
 * @brief Getter for drawing buffer..
 */
char * getDrawingBuffer();

/**
 * @brief Getter for graphics buffer..
 */
char * getGraphicsBuffer();

/**@}*/

#endif
