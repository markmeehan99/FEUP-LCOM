#include <lcom/lcf.h>
#include "bitmap.h"
#include "obstacles.h"
#include "game.h"
#include "macros.h"
#include "video_gr.h"
#include "chicken.h"
#include "crosshair.h"

Obstacle *obstacles[NUM_OBS];

extern int backgroundWidth;
//extern bool powerUp;
extern uint32_t powerUpCounter;
extern int powerUps;

Obstacle *make_obstacle()
{
  int r = rand() % 2 + 0; // r = 0 -> traffic cone ; r = 1 -> head level obstacle

  Obstacle *ob = (Obstacle *)malloc(sizeof(Obstacle));
  ob->x_pos = BACKGROUND_WIDTH;
  ob->isShot = false;

  if (r == 0)
  {
    ob->bmp = loadBitmap("/home/lcom/labs/proj/res/traffic_cone.bmp");
    ob->x_dim = ob->bmp->bitmapInfoHeader.width;
    ob->y_dim = ob->bmp->bitmapInfoHeader.height;
    ob->y_pos = FLOOR_LEVEL - CONE_HEIGHT;
  }
  else if (r == 1)
  {
    ob->bmp = loadBitmap("/home/lcom/labs/proj/res/bees.bmp"); 
    ob->x_dim = ob->bmp->bitmapInfoHeader.width;
    ob->y_dim = ob->bmp->bitmapInfoHeader.height;
    ob->y_pos = FLOOR_LEVEL - BEES_LEVEL - BEES_HEIGHT;
  }

  return ob;
}

void delete_obstacle(Obstacle *ob)
{
  free(ob);
}

void delete_obstacles()
{
  for (int i = 0; i < NUM_OBS; i++)
    free(obstacles[i]);
}

void draw_obstacle(Obstacle *ob)
{
  drawBitmapTrans(ob->bmp, ob->x_pos, ob->y_pos, ALIGN_LEFT, 0);
}

void update_obstacle(Obstacle *ob, background *backGround)
{
  ob->x_pos -= backGround->ground_speed;
  if (!ob->isShot)
  {
    draw_obstacle(ob);
  }
}

void updateOnScreen_obstacles(background *backGround)
{
  for (int i = 0; i < NUM_OBS; i++)
  {
    if (obstacles[i] != NULL)
      update_obstacle(obstacles[i], backGround);
  }

  if (obstacles[0] != NULL)
  {
    if (obstacles[0]->x_pos + obstacles[0]->x_dim < 0)
    {
      deleteOffScreen_obstacles();
    }
  }
}

bool obstacleHit(Obstacle *ob, Crosshair *ch)
{
  if (ch->pressed)
  {
    if (ch->x_pos > ob->x_pos && ch->x_pos < (ob->x_pos + ob->x_dim))
      if (ch->y_pos > ob->y_pos && ch->y_pos < (ob->y_dim + ob->y_pos))
        return true;
  }
  return false;
}

void insert_obstacle()
{
  Obstacle *ob = make_obstacle();
  for (int i = 0; i < NUM_OBS; i++)
  {
    if (obstacles[i] == NULL)
    {
      obstacles[i] = ob;
      break;
    }
  }
}

void deleteOffScreen_obstacles()
{
  delete_obstacle(obstacles[0]);
  for (int i = 1; i < NUM_OBS; i++)
  {
    if (obstacles[i] == NULL)
    {
      obstacles[i - 1] = NULL;
    }
    else
      obstacles[i - 1] = obstacles[i];
  }
}

void checkObstacles(Crosshair *ch)
{
  for (int i = 0; i < NUM_OBS; i++)
  {
    if (obstacles[i] != NULL)
      if (!obstacles[i]->isShot)
        if (obstacleHit(obstacles[i], ch))
        {
          if (powerUps > 0)
          {
            obstacles[i]->isShot = true;
            powerUps--;
          }
          powerUpCounter = 0;
        }
  }
}

void reset_obstacles()
{
  for (int i = 0; i < NUM_OBS; i++)
  {
    if (obstacles[i] != NULL)
    {
      obstacles[i] = NULL;
    }
  }
}

PowerUps *makePowerUps()
{
  PowerUps *pups = (PowerUps *)malloc(sizeof(PowerUps));

  pups->pu0 = loadBitmap("/home/lcom/labs/proj/res/power0.bmp");
  pups->pu1 = loadBitmap("/home/lcom/labs/proj/res/power1.bmp");
  pups->pu2 = loadBitmap("/home/lcom/labs/proj/res/power2.bmp");
  pups->pu3 = loadBitmap("/home/lcom/labs/proj/res/power3.bmp");

  return pups;
}

void deletePowerUps(PowerUps* pu)
{
  free(pu);
}


void displayPowerUps(PowerUps *pu)
{
  switch (powerUps)
  {
  case 0:
    drawBitmapTrans(pu->pu0, POWER_UPS_HOR, POWER_UPS_VER, ALIGN_LEFT, 0);
    break;
  case 1:
    drawBitmapTrans(pu->pu1, POWER_UPS_HOR, POWER_UPS_VER, ALIGN_LEFT, 0);
    break;
  case 2:
    drawBitmapTrans(pu->pu2, POWER_UPS_HOR, POWER_UPS_VER, ALIGN_LEFT, 0);
    break;
  case 3:
    drawBitmapTrans(pu->pu3, POWER_UPS_HOR, POWER_UPS_VER, ALIGN_LEFT, 0);
    break;
  }
}
