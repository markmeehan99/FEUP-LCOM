#pragma once
#include "bitmap.h"
#include <stdbool.h>


/** @defgroup Crosshair Crosshair
 * @{
 * Functions for controlling crosshair
 */

typedef struct {
    int x_pos;  ///<Position on the x axis to draw the crosshair's bitmap.
    int y_pos;  ///<Position on the y axis to draw the crosshair's bitmap.
    bool pressed; ///<Bool that indicates if the left button of the mouse is pressed.
    Bitmap * bmp; ///<Crosshair's bitmap.
} Crosshair;

/**
 * @brief Loads crosshair's bitmap to a Crosshair objects and initializes it's members.
 * 
 * @return Pointer to an object of the Crosshair struct. 
 */
Crosshair* make_ch();

/**
 * @brief Updates crosshair's position using the packet received from the mouse.
 * 
 * @param ch Pointer to an object of the Crosshair struct.
 * @param pp Packet received from the mouse.
 */
void update_ch(Crosshair *ch, struct packet *pp);

/**
 * @brief Draw's the crosshair's bitmap.
 * 
 * @param ch Pointer to an object of the Crosshair struct.
 */
void draw_ch(Crosshair *ch);

/**@}*/
