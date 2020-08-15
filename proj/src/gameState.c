#include <lcom/lcf.h>
#include "gameState.h"

GameState* makeGameState()
{
    GameState *gs = (GameState *) malloc(sizeof(GameState));
    gs->state = MAIN;

    return gs;
}

void deleteGameState(GameState* gs)
{
    free(gs);
}
