#pragma once

#include <lcom/lcf.h>
#include <stdlib.h>
#include "bitmap.h"
#include "macros.h"
#include "crosshair.h"
#include "gameState.h"

/** @defgroup Menu Menu
 * @{
 * Functions for manipulating all menus in game
 */

// SELECT BOX

typedef struct 
{
    /** @name  Box Coordinates*/
	/**@{
	*/
    int xi,xf,yi,yf;
    /** @} end of the Box Coordinates */
} Box;

/**
 * @brief Creates an object of the Box struct and initializes it's members.
 * 
 * @param xi Initial x coordinate for the box.
 * @param xf Final x coordinate for the box.
 * @param yi Initial y coordinate for the box.
 * @param yf Final y coordinate for the box.
 * @return Pointer to an object of the Box struct.
 */
Box * make_box(int xi, int xf, int yi, int yf);

/**
 * @brief Frees allocated memory.
 * 
 * @param box Pointer to an object of the Box struct.
 */
void delete_box(Box * box);

/**
 * @brief Checks if the crosshair is inside the box area.
 * 
 * @param box Pointer to an object of the Box struct.
 * @param ch Pointer to an object of the Crosshair struct.
 * @return int Returns 1 if the crosshair is inside the box area and 0 if otherwise.
 */
int check_select(Box * box, Crosshair* ch);

// MAIN MENU

typedef struct{

    Bitmap* background; ///<Menu's bitmap.
    Bitmap* select_box; ///<Select Box's bitmap.

    int play_selected;  ///<Indicates if the crosshair is inside the play button's area.
    int exit_selected;  ///<Indicates if the crosshair is inside the exit button's area.

    Box* play_button;   ///<Box object representing the play button on the menu.
    Box* exit_button;   ///<Box object representing the exit button on the menu.

} mainMenu;


typedef struct{

    Bitmap* background; ///<Menu's bitmap.
    Bitmap* select_box; ///<Select Box's bitmap.
    Bitmap* pause;      ///<Bitmap for the word "Pause".
    Bitmap* over;       ///<Bitmap for the word "Over".

    int exit_selected;  ///<Indicates if the crosshair is inside the play button's area.
    int play_selected;  ///<Indicates if the crosshair is inside the exit button's area.

    Box* exit_button;   ///<Box object representing the play button on the menu.
    Box* play_button;   ///<Box object representing the exit button on the menu.

} scoreMenu;

/**
 * @brief Loads bitmaps to a mainMenu object and creates the objects for the button areas.
 * 
 * @return Pointer to an object of the mainMenu struct. 
 */
mainMenu* make_mainMenu();

/**
 * @brief Checks if any button was pressed.
 * 
 * @param menu Pointer to an object of the mainMenu struct.
 * @param gs Pointer to an object of the GameState struct.
 * @param ch Pointer to an object of the Crosshair struct.
 */
void update_mainMenu(mainMenu* menu, GameState* gs, Crosshair * ch);

/**
 * @brief Draws menu's bitmap and select boxes(only if the crosshair is inside the button areas).
 * 
 * @param menu Pointer to an object of the mainMenu struct.
 */
void draw_mainMenu(mainMenu* menu);

/**
 * @brief Frees allocated memory.
 * 
 * @param menu Pointer to an object of the mainMenu struct.
 */
void delete_MainMenu(mainMenu* menu);

/**
 * @brief Loads bitmaps to a scoreMenu object and creates the objects for the button areas.
 * 
 * @return Pointer to an object of the scoreMenu struct.
 */
scoreMenu* make_scoreMenu();

/**
 * @brief Checks if any button was pressed and resets the necessary variables to default if game is over.
 * 
 * @param menu Pointer to an object of the scoreMenu struct.
 * @param gs Pointer to an object of the GameState struct.
 * @param ch Pointer to an object of the Crosshair struct.
 */
void update_scoreMenu(scoreMenu* menu, GameState* gs, Crosshair * ch);

/**
 * @brief Draws the score menu's bitmap and select boxes(only if the crosshair is inside the button areas).
 * 
 * @param menu Pointer to an object of the scoreMenu struct.
 */
void draw_scoreMenu(scoreMenu* menu);

/**
 * @brief Frees allocated memory.
 * 
 * @param menu Pointer to an object of the scoreMenu struct.
 */
void delete_scoreMenu(scoreMenu* menu);

// BACKGROUND

typedef struct{
  int backgroundX;  ///<Position on the x axis to draw background's bitmap.
  int background_speed; ///<Speed at which the background moves.
  int groundX;  ///<Position on the x axis to draw ground's bitmap.
  int ground_speed; ///<Speed at which the ground moves.
  Bitmap * background; ///<Background's bitmap.
  Bitmap * ground; ///<Ground's bitmap.
  int backgroundWidth; ///<Background's width.
} background;

/**
 * @brief Loads background bitmaps(sky and ground) to background struct.
 * 
 * @return Pointer to an object of the background struct. 
 */
background * make_background();

/**
 * @brief Deletes bitmaps and frees allocated memory.
 * 
 * @param background Pointer to an object of the background struct.
 */
void delete_background(background* backGround);

/**
 * @brief Makes the background(sky and ground) move.
 * 
 * @param backGround Pointer to an object of the background struct.
 * @param menu Pointer to an object of the menu struct.
 */
void update_background(background* backGround);

/**
 * @brief Draw the bitmaps for the sky and ground.
 * 
 * @param backGround Pointer to an object of the background struct.
 */
void draw_background(background* backGround);

/**
 * @brief Updates the background's bitmap drawing position.
 * 
 * @param backGround Pointer to an object of the background struct.
 */
void move_background(background* backGround);

/**
 * @brief Updates the ground's bitmap drawing position.
 * 
 * @param backGround Pointer to an object of the background struct.
 */
void move_ground(background* backGround);

/**
 * @brief Checks if the crosshair is inside the box's limits.
 * 
 * @param box Pointer to an object of the Box struct.
 * @param ch Pointer to an object of the Crosshair struct.
 * @return 1 if mouse is inside the box and 0 if otherwise.
 */
int mouseInsideBox (Box* box, Crosshair* ch);

/**@}*/
