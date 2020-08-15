#pragma once
#include <lcom/lcf.h>
#include "bitmap.h"
#include <stdlib.h>
#include "crosshair.h"
#include "gameState.h"
#include "menu.h"

/** @defgroup Obstacles Obstacles
 * @{
 * Functions for manipulating obstacles
 */

// OBSTACLES

typedef struct
{
  int x_pos;  ///<Obstacle's position on the x axis.
  int y_pos;  ///<Obstacle's position on the x axis.
  int x_dim;  ///<Width of the obstacle
  int y_dim;  ///<Heigth of the obstacle
  bool isShot;  ///<Indicates if the user clicked on the obstacle.
  Bitmap *bmp;  ///<Obstacles bitmap.
} Obstacle;

/**
 * @brief Creates an obstacle object, loads it's bitmap and initializes the objects members.
 * It can either be a cone or a swarm of bees.
 * 
 * @return Pointer to an object of the Obstacle struct. 
 */
Obstacle *make_obstacle();

/**
 * @brief Frees memory allocated.
 * 
 * @param ob Pointer to an object of the Obstacle struct. 
 */
void delete_obstacle(Obstacle *ob);

/**
 * @brief Frees the memory allocated by all obstacles displayed on screen.
 * 
 */
void delete_obstacles();

/**
 * @brief Draws obstacle's bitmap on screen.
 * 
 * @param ob Pointer to an object of the Obstacle struct. 
 */
void draw_obstacle(Obstacle *ob);

/**
 * @brief Updates obstacle's position and checks if it was "shot".
 * 
 * @param ob Pointer to an object of the Obstacle struct. 
 * @param backGround Pointer to an object of the background struct. 
 */
void update_obstacle(Obstacle *ob, background *backGround);

/**
 * @brief Checks if the user clicked on the obstacle.
 * 
 * @param ob Pointer to an object of the Obstacle struct. 
 * @param ch Pointer to an object of the Crosshair struct. 
 * @return true If the user clicked on the obstacle.
 * @return false If the user didn't click on the obstacle.
 */
bool obstacleHit(Obstacle *ob, Crosshair *ch);

/**
 * @brief Creates an obstacle and inserts it in the obstacles array.
 * 
 */
void insert_obstacle();

/**
 * @brief Deletes obstacles not being shown on the screen.
 * 
 */
void deleteOffScreen_obstacles();

/**
 * @brief Updates all obstacles being shown on the screen.
 * 
 */
void updateOnScreen_obstacles();

/**
 * @brief Checks if an obstacle can be shot and updates power up counter.
 * 
 * @param ch Pointer to an object of the Crosshair struct.
 */
void checkObstacles(Crosshair *ch);

/**
 * @brief Empties obstacles array.
 * 
 */
void reset_obstacles();

// POWER UPS

typedef struct
{
  /** @name  PowerUp bars bitmaps*/
	/**@{
	*/
  Bitmap *pu0; 
  Bitmap *pu1;
  Bitmap *pu2;
  Bitmap *pu3;
  /** @} end of the bars bitmaps */
} PowerUps;

/**
 * @brief Loads power up bar bitmaps.
 * 
 * @return Pointer to an object of the PowerUps struct.
 */
PowerUps *makePowerUps();

/**
 * @brief Frees allocated memory.
 * 
 * @param pu Pointer to an object of the PowerUps struct.
 */
void deletePowerUps(PowerUps* pu);

/**
 * @brief Draws power up bar on the screen.
 * 
 */
void displayPowerUps();

/**@}*/
