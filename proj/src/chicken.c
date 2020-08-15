#include <lcom/lcf.h>
#include "bitmap.h"
#include "video_gr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "macros.h"
#include "chicken.h"

extern int counter;

Chicken* make_chick() {
    Chicken * chick = (Chicken *) malloc (sizeof(Chicken));
    chick->step_rightLeg_bmp = loadBitmap("/home/lcom/labs/proj/res/chicken_step_rightLeg.bmp");
    chick->step_midStage_bmp = loadBitmap("/home/lcom/labs/proj/res/chicken_step_midStage.bmp");
    chick->step_leftLeg_bmp = loadBitmap("/home/lcom/labs/proj/res/chicken_step_leftLeg.bmp");
    chick->ducking_bmp = loadBitmap("/home/lcom/labs/proj/res/ducking_chicken.bmp");
    chick->x_pos = CHICKEN_X_POS;
    chick->y_pos = FLOOR_LEVEL - CHICKEN_HEIGHT - 2;
    chick->jumping = false;
    chick->vertical_velocity = JUMP_INICIAL_SPEED;
    chick->ducking = false;
    chick->walkingStage = 0;
    
    return chick;
}


void delete_chick(Chicken * chicken){
    deleteBitmap(chicken->step_rightLeg_bmp);
    deleteBitmap(chicken->step_midStage_bmp);
    deleteBitmap(chicken->step_leftLeg_bmp);
    deleteBitmap(chicken->ducking_bmp);
    free(chicken);
}

void draw_Chicken(Chicken * chicken){
    if (chicken->ducking){
        drawBitmapTrans(chicken->ducking_bmp, chicken->x_pos, chicken->y_pos + 44, ALIGN_LEFT, 1);
    } else
    {
        if (chicken->walkingStage == 0) drawBitmapTrans(chicken->step_rightLeg_bmp, chicken->x_pos, chicken->y_pos, ALIGN_LEFT, 1);
        else if (chicken->walkingStage == 1) drawBitmapTrans(chicken->step_midStage_bmp, chicken->x_pos, chicken->y_pos, ALIGN_LEFT, 1);
        else drawBitmapTrans(chicken->step_leftLeg_bmp, chicken->x_pos, chicken->y_pos, ALIGN_LEFT, 1);
    }
}

void jump(Chicken * chicken)
{
    chicken->vertical_velocity -= GRAVITY;

    chicken->y_pos -= chicken->vertical_velocity;
    
    if (chicken->y_pos >= FLOOR_LEVEL - CHICKEN_HEIGHT - 2)
    {
        chicken->y_pos = FLOOR_LEVEL - CHICKEN_HEIGHT - 2;
        chicken->jumping = false;
        chicken->vertical_velocity = 0;
    }
}

