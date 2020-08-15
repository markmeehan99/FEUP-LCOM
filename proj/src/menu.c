#include <lcom/lcf.h>
#include "video_gr.h"
#include "mouse.h"
#include "obstacles.h"
#include "gameState.h"
#include "rtc.h"


extern int counter;
extern bool gameOver;
extern int finalScore;
extern uint32_t nextObstacle_time;
extern Obstacle *obstacles[];
extern uint8_t scancode;
extern GameState* gamestate;
extern uint32_t powerUpCounter;
extern int powerUps;
extern int totalScore;
extern background *backGround;

Box * make_box(int xi, int xf, int yi, int yf)
{
    Box * box = (Box*) malloc(sizeof(Box));

    box->xi = xi;
    box->xf = xf;
    box->yi = yi;
    box->yf = yf;

    return box;
}

int check_select(Box * box, Crosshair* ch)
{
    return mouseInsideBox(box,ch);
}

void delete_box(Box * box)
{
    free(box);
}

mainMenu* make_mainMenu(){
    mainMenu* menu = (mainMenu*) malloc(sizeof(mainMenu));
    menu->background = loadBitmap("/home/lcom/labs/proj/res/menu_background.bmp");
    menu->select_box = loadBitmap("/home/lcom/labs/proj/res/select_box.bmp");

    int x1 = 420,x2 = 610,y1 = 330,y2 = 390;
    menu->play_button = make_box(x1,x2,y1,y2);
    menu->play_selected = 0;

    x1 = 420,x2 = 610,y1 = 430,y2 = 500;
    menu->exit_button = make_box(x1,x2,y1,y2);
    menu->exit_selected = 0;
    

    return menu;
}


void update_mainMenu(mainMenu* menu, GameState* gs, Crosshair * ch){

    if (check_select(menu->play_button,ch)) {
        menu->play_selected = 1;
        if (ch->pressed)
        {
            gs->state = PLAY;
            counter = 0;
        }
    }
    else menu->play_selected = 0;


    if (check_select(menu->exit_button,ch)) {
        menu->exit_selected = 1;
        if(ch->pressed){
            gs->state = EXIT;

        }
    }
    else menu->exit_selected = 0;
}

void draw_mainMenu(mainMenu* menu){
    drawBitmap(menu->background,0,0,ALIGN_LEFT);

    if (menu->play_selected){
        drawBitmapTrans(menu->select_box,420,330,ALIGN_LEFT,0);
    } 
    else if (menu->exit_selected){
        drawBitmapTrans(menu->select_box,420,430,ALIGN_LEFT,0);
    }
}

void delete_MainMenu(mainMenu* menu)
{
    deleteBitmap(menu->background);
    delete_box(menu->play_button);
    delete_box(menu->exit_button);
}



//SCORE MENU


scoreMenu* make_scoreMenu()
{
    scoreMenu* menu = (scoreMenu*) malloc(sizeof(scoreMenu));
    menu->background = loadBitmap("/home/lcom/labs/proj/res/menu_background.bmp");
    menu->select_box = loadBitmap("/home/lcom/labs/proj/res/select_box.bmp");
    menu->pause = loadBitmap("/home/lcom/labs/proj/res/pause.bmp");
    menu->over = loadBitmap("/home/lcom/labs/proj/res/over.bmp");

    int x1 = 420,x2 = 610,y1 = 330,y2 = 390;
    menu->play_button = make_box(x1,x2,y1,y2);
    menu->play_selected = 0;

    x1 = 420,x2 = 610,y1 = 430,y2 = 500;

    menu->exit_button = make_box(x1,x2,y1,y2);
    menu->exit_selected = 0;
    
    return menu;
}




void update_scoreMenu(scoreMenu* menu, GameState* gs, Crosshair * ch)
{
    if (gs->state == PAUSE)
    {
        if (check_select(menu->play_button,ch)) 
        {
            menu->play_selected = 1;
            if (ch->pressed)
            {
                gs->state = PLAY;
                counter = 0;
                nextObstacle_time = 1;
                gameOver = false;
                scancode = ESC_MAKE; //Para poder voltar a jogar sem problemas
            }
        }
        else menu->play_selected = 0;


        if (check_select(menu->exit_button,ch))
        {
            menu->exit_selected = 1;
            if(ch->pressed){
                gs->state = EXIT;
            }
        }
        else menu->exit_selected = 0;
    }

    if (gs->state == OVER)
    {
        if (check_select(menu->play_button,ch)) 
        {
            menu->play_selected = 1;
            if (ch->pressed)
            {
                gs->state = PLAY;
                counter = 0;
                nextObstacle_time = 1;
                gameOver = false;
                scancode = ESC_MAKE; //Para poder voltar a jogar sem problemas
                powerUpCounter = 0;
                powerUps = 0;
                totalScore = 0;

                
                for (int i = 0; i < NUM_OBS; i++)
                {
                    if(obstacles[i] != NULL)
                        obstacles[i]->isShot = true;
                }
            }
        }
        else menu->play_selected = 0;


        if (check_select(menu->exit_button,ch))
        {
            menu->exit_selected = 1;
            if(ch->pressed)
            {
                gs->state = EXIT;
            }
        }
        else menu->exit_selected = 0;   
    }
}

void draw_scoreMenu(scoreMenu* menu)
{
    drawBitmap(menu->background,0,0,ALIGN_LEFT);

    if (menu->play_selected){
        drawBitmapTrans(menu->select_box,420,330,ALIGN_LEFT,0);
    } 

    if (menu->exit_selected){
        drawBitmapTrans(menu->select_box,420,430,ALIGN_LEFT,0);
    }
}

void delete_scoreMenu(scoreMenu* menu)
{
    free (menu);
}



background * make_background(){
  background* backGround = (background*) malloc(sizeof(background));
  backGround->groundX = 0;
  backGround->backgroundX = 0;
  backGround->ground_speed = GROUND_INITIAL_SPEED;
  backGround->background_speed = BACKGROUND_INITIAL_SPEED;
  backGround->ground = loadBitmap("/home/lcom/labs/proj/res/ground.bmp");
  backGround->background = loadBitmap("/home/lcom/labs/proj/res/sky.bmp");
  backGround->backgroundWidth = backGround->background->bitmapInfoHeader.width;

  return backGround;
}

void delete_background(background* bck){
  deleteBitmap(bck->background);
  free(bck);
}

void update_background(background* backGround){

  move_background(backGround);
  move_ground(backGround);
}

void move_ground(background* backGround){
  backGround->groundX -= backGround->ground_speed;

  if (backGround->groundX < - backGround->backgroundWidth)
    backGround->groundX = 0;
}

void move_background(background* backGround){
  backGround->backgroundX -= backGround->background_speed;

  if (backGround->backgroundX < - backGround->backgroundWidth)
    backGround->backgroundX = 0;
}

void draw_background(background* backGround){
  drawBitmap(backGround->ground, backGround->groundX, FLOOR_LEVEL, ALIGN_LEFT);
  drawBitmap(backGround->ground, backGround->groundX + backGround->backgroundWidth, FLOOR_LEVEL, ALIGN_LEFT);

  drawBitmap(backGround->background, backGround->backgroundX, 0, ALIGN_LEFT);
  drawBitmap(backGround->background, backGround->backgroundX + backGround->backgroundWidth, 0, ALIGN_LEFT);
}

int mouseInsideBox (Box* box, Crosshair* ch){
  int xi = box->xi, xf = box->xf;
  int yi = box->yi, yf = box->yf;

  return xi <= ch->x_pos && xf >= ch->x_pos && yi <= ch->y_pos && yf >= ch->y_pos;
}
