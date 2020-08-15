#pragma once

#include "bitmap.h"
#include <stdbool.h>

/** @defgroup GameState GameState
 * @{
 * Functions for manipulating gamestate in gmae
 */

/// Represents the game's states.
typedef enum {MAIN, PLAY, PAUSE, OVER, EXIT} State;

typedef struct {
    State state; ///< Current state.
} GameState;

/**
 * @brief Initializes the object member to MAIN (from the State enum).
 * 
 * @return Pointer to an object of the GameState struct.
 */
GameState* makeGameState();

/**
 * @brief Frees allocated memory.
 * 
 * @param gs Pointer to an object of the Gamestate struct.
 */
void deleteGameState(GameState* gs);

/**@}*/
