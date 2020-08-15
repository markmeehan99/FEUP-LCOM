#include <lcom/lcf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"
#include "chicken.h"
#include "game.h"
#include "i8042.h"
#include "keyboard.h"
#include "macros.h"
#include "mouse.h"
#include "obstacles.h"
#include "video_gr.h"
#include "crosshair.h"
#include "gameState.h"
#include "rtc.h"
#include "menu.h"

//General
bool gameOver = false;
int backgroundX = 0;
int lastStep = 0;

//bool powerUp;
int finalScore;
bool first = false;
int totalScore = 0;

//Timer
extern uint32_t counter;
uint32_t powerUpCounter;
uint32_t nextObstacle_time = 1;
uint32_t nextStep_time = GROUND_INITIAL_SPEED;
int powerUps = 0;

//Keyboard
int sys_count = 0;
int kbd_id = 1;
uint8_t scancode;

//Mouse
uint16_t byte_packet[3];
struct packet processed_packet;
uint32_t byte;

int game()
{
  //General
  message msg;
  int ipc_status;
  uint16_t r;

  //Timer
  uint8_t irq_set_timer;

  //Keyboard
  uint16_t irq_set_kbd;
  int bit_no = 0;
  bool two_bytes = false;
  uint8_t scan_bytes[2];

  //Mouse
  int mouse_id = 12;
  static uint16_t count_index = 0;

  //Make objects
  background *backGround = make_background();
  Chicken *chicken = make_chick();
  Crosshair *crosshair = make_ch();
  Time *time = makeTime();
  mainMenu *menu = make_mainMenu();
  scoreMenu *score_menu = make_scoreMenu();
  PowerUps *power_ups = makePowerUps();
  GameState *gamestate = makeGameState();

  //Enable mouse
  if (write_cmd(ENABLE) != 0)
  {
    printf("Error enabling mouse\n");
    return 1;
  }

  //Subscribes
  if (mouse_subscribe_int(&mouse_id) != 0)
  {
    printf("Error subscribing a mouse notification\n");
    return 1;
  }

  uint32_t irq_set_mouse = mouse_id;

  if (keyboard_subscribe_int(&bit_no) != OK)
  {
    return 1;
  }

  irq_set_kbd = bit_no;

  if (timer_subscribe_int(&irq_set_timer) != OK)
  {
    return 1;
  }

  //Initialize graphics mode
  if (vg_init(0x117) == NULL)
  {
    printf("Error in vg_init");
    return 1;
  }

  //Game loop. Runs until ESC key pressed or GAMEOVER.
  while (!(gamestate->state == EXIT))
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:

        //Mouse interrupts
        if (msg.m_notify.interrupts & irq_set_mouse)
        { /* subscribed interrupt */
          mouse_ih();
          if (count_index == 0 && (byte & CHECK_BYTE1))
          {
            byte_packet[0] = byte;
            count_index++;
          }
          else if (count_index == 1)
          {
            byte_packet[1] = byte;
            count_index++;
          }
          else if (count_index == 2)
          {
            byte_packet[2] = byte;
            process_packet();
            count_index = 0;

            update_ch(crosshair, &processed_packet);
          }
        }


        //Timer interrupts
        if (msg.m_notify.interrupts & irq_set_timer)
        {
          counter++;

            if (scancode == ESC_BREAK)
            {
              gamestate->state = PAUSE;
            }

            if (gameOver) gamestate->state = OVER;

          //60 FPS
          if (counter % 1 == 0)
          {
            if (gamestate->state == MAIN)
            {
              update_mainMenu(menu, gamestate, crosshair);
              draw_mainMenu(menu);
              draw_ch(crosshair);
              copy_draw();
            }


            //PLAY
            if (gamestate->state == PLAY)
            {
              
              if (counter % 20 == 0) totalScore++;

              if (counter == nextObstacle_time)
              {
                insert_obstacle();
                nextObstacle_time = (counter + (180 - (rand() % 28 + backGround->ground_speed * 9)));
              }

              powerUpCounter++;

              if (powerUpCounter % 300 == 0 && powerUps < 3)
                powerUps++;

              if (backGround->ground_speed < MAX_SPEED)
              {
                if (counter % 100 == 0)
                  backGround->ground_speed += 1;
                if (counter % 1000 == 0)
                  backGround->background_speed += 1;
              }

              update_background(backGround);

              

              if (counter == nextStep_time){
                nextStep_time = counter + backGround->ground_speed;
                if (chicken->walkingStage == 0){
                  chicken->walkingStage = 1;
                  lastStep = 0; 
                } else if (chicken->walkingStage == 2){
                  chicken->walkingStage = 1;
                  lastStep = 2;
                } else if (chicken->walkingStage == 1){
                  if (lastStep == 0) chicken->walkingStage = 2;
                  else if (lastStep == 2) chicken->walkingStage = 0;
                }
              }

              if ((scancode == UP_KEY_MAKE || scancode == W_KEY_MAKE || scancode == SPACE_KEY_MAKE) && !chicken->jumping && !chicken->ducking)
              {
                chicken->jumping = true;
                chicken->vertical_velocity = JUMP_INICIAL_SPEED;
              }

              if (chicken->jumping && !chicken->ducking)
              {
                jump(chicken);
              }

              if ((scancode == CTRL_KEY_MAKE || scancode == S_KEY_MAKE) && !chicken->ducking && !chicken->jumping)
              {
                chicken->ducking = true;
              }

              if ((scancode == CTRL_KEY_BREAK || scancode == S_KEY_BREAK) && chicken->ducking)
              {
                chicken->ducking = false;
              }

              checkObstacles(crosshair);
              draw_background(backGround);
              updateOnScreen_obstacles(backGround);
              draw_Chicken(chicken);
              draw_ch(crosshair);

              displayScore(time->num, totalScore);

              getCurrentTime(time);

              displayTime(time, time->num);
              displayDate(time, time->num);

              displayPowerUps(power_ups);

              copy_draw();
            }


            if (gamestate->state == OVER)
            {
              nextStep_time = GROUND_INITIAL_SPEED;
              update_scoreMenu(score_menu, gamestate, crosshair);
              draw_scoreMenu(score_menu);
              draw_ch(crosshair);
              displayScore(time->num, totalScore);
              drawBitmapTrans(score_menu->over, 15,270,ALIGN_LEFT,0);
              copy_draw();

              if (gamestate->state == PLAY)
              {
                backGround->ground_speed = GROUND_INITIAL_SPEED;
                backGround->background_speed = BACKGROUND_INITIAL_SPEED;
              }
            }


            if (gamestate->state == PAUSE)
            {
              update_scoreMenu(score_menu, gamestate, crosshair);
              draw_scoreMenu(score_menu);
              draw_ch(crosshair);
              displayScore(time->num, totalScore);
              drawBitmapTrans(score_menu->pause, 15,270,ALIGN_LEFT,0);
              displayPowerUps(power_ups);
              copy_draw();
            }

          }
        }

        //Keyboard interrupts
        if (msg.m_notify.interrupts & irq_set_kbd)
        {
          kbc_ih();
          if (scancode == TWO_BYTE_CODE)
          {
            two_bytes = true;
            scan_bytes[0] = scancode;
          }
          else if (two_bytes)
          {
            two_bytes = false;
            scan_bytes[1] = scancode;
          }
          else
          {
            scan_bytes[0] = scancode;
          }
        }

        break;
      default:
        break;
      }
    }
  }

  //Free some space
  delete_background(backGround);
  delete_chick(chicken);
  delete_MainMenu(menu);
  deleteTime(time);
  delete_obstacles();
  deleteGameState(gamestate);
  delete_scoreMenu(score_menu);

  //Unsubscribes
  if (keyboard_unsubscribe_int() != OK)
  {
    return 1;
  }

  if (timer_unsubscribe_int() != OK)
  {
    return 1;
  }

  if (mouse_unsubscribe_int() != 0)
  {
    printf("The program was unable to unsubscribe a mouse notification\n");
    return 1;
  }

  //Disable mouse
  if (write_cmd(DISABLE) != 0)
  {
    printf("The program failed to disable the mouse data reporting\n");
    return 1;
  }

  //Exit graphics mode
  if (vg_exit() != OK)
  {
    printf("Error in vg_init");
    return 1;
  }

  return 0;
}
