#pragma once
#include "bitmap.h"
#include <stdbool.h>


/** @defgroup Chicken Chicken
 * @{
 * Functions for chicken related funtions
 */

typedef struct {
    int x_pos;  ///<Positon on the x axis to draw the chicken's bitmap.
    int y_pos;  ///<Positon on the y axis to draw the chicken's bitmap.
    bool jumping; ///<Bool that indicates if the chicken is jumping.
    int vertical_velocity; ///<Chickens vertical velocity.
    bool ducking; ///<Bool that indicates if the chicken is ducking (crouching).
    int walkingStage; ///<Indicates which walking stage the chicken is on (0,1 or 2).

    /** @name  Chicken's bitmaps*/
	/**@{
	*/
    Bitmap * step_rightLeg_bmp; 
    Bitmap * step_midStage_bmp;
    Bitmap * step_leftLeg_bmp;
    Bitmap * ducking_bmp;
    /** @} end of the chicken's bitmaps */
} Chicken;

/**
 * @brief Loads all the chicken's bitmaps to a Chicken object and initializes it's members.
 * 
 * @return Pointer to an object of the Chicken struct.
 */
Chicken* make_chick();

/**
 * @brief Deletes bitmaps and frees allocated memory.
 * 
 * @param chicken Pointer to an object of the chicken struct.
 */
void delete_chick(Chicken * chicken);

/**
 * @brief Draw's chicken's bitmaps depending on which walking stage the chicken's on.
 * 
 * @param chicken Pointer to an object of the chicken struct.
 */
void draw_Chicken(Chicken * chicken);

/**
 * @brief Updates all the values related to the jump action.
 * 
 * @param chicken Pointer to an object of the chicken struct.
 */
void jump(Chicken * chicken);

/**@}*/
