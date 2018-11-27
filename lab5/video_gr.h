#ifndef _VIDEO_GR_
#define _VIDEO_GR_

#include <stdint.h>

int vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);
int (vg_display_xpm)(const char *xpm[],  uint16_t x, uint16_t y);
int color_set(uint16_t x, uint16_t y, uint32_t color);

#endif
