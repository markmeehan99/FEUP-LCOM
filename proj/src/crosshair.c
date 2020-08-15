#include <lcom/lcf.h>
#include "bitmap.h"
#include "video_gr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "macros.h"
#include "chicken.h"
#include "crosshair.h"

Crosshair* make_ch() {
    Crosshair * ch = (Crosshair *) malloc (sizeof(Crosshair));
    ch->x_pos = 0;
    ch->y_pos = 0;
    ch->pressed = false;
    ch->bmp = loadBitmap("/home/lcom/labs/proj/res/crosshair.bmp");

    return ch;
}

void update_ch(Crosshair *ch, struct packet *pp)
{
  ch->pressed = pp->lb;
  ch->x_pos += pp->delta_x;
  ch->y_pos -= pp->delta_y;

  if (ch->x_pos < 0) ch->x_pos = 0;

  if (ch->y_pos < 0) ch->y_pos = 0;

  if (ch->x_pos > BACKGROUND_WIDTH - CROSSHAIR_DIM)
    ch->x_pos = BACKGROUND_WIDTH - CROSSHAIR_DIM;

  if (ch->y_pos > BACKGROUND_HEIGHT - CROSSHAIR_DIM)
    ch->y_pos = BACKGROUND_HEIGHT - CROSSHAIR_DIM;
}

void draw_ch(Crosshair *ch){
  drawBitmapTrans(ch->bmp, ch->x_pos, ch->y_pos, ALIGN_LEFT, 0);
}
