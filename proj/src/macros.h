#pragma once

#define TRANSPARENCY                0xf81f
#define NUM_OBS                     10
#define GFX_MODE                    0x117

// Chicken
#define CHICKEN_X_POS               20
#define CHICKEN_HEIGHT              144
#define CHICKEN_DUCKING_HEIGHT      100


#define OBSTACLE_X_DIM              66
#define OBSTACLE_Y_DIM              66
#define CONE_HEIGHT                 62
#define BEES_LEVEL                  110
#define BEES_HEIGHT                 90

// Background
#define FLOOR_LEVEL                 488
#define BACKGROUND_WIDTH            1024
#define BACKGROUND_HEIGHT           768


// Keyboard
#define UP_KEY_MAKE                 0x48
#define DOWN_KEY_MAKE               0x50
#define CTRL_KEY_MAKE               0x1D
#define SPACE_KEY_MAKE              0x39
#define ESC_BREAK                   0x81
#define ESC_MAKE                    0x01
#define W_KEY_MAKE                  0x11
#define S_KEY_MAKE                  0x1F
#define S_KEY_BREAK                 0x9F
#define DOWN_KEY_BREAK              0xD0
#define CTRL_KEY_BREAK              0x9D


// Speeds
#define GRAVITY                     1
#define BACKGROUND_INITIAL_SPEED    1  
#define GROUND_INITIAL_SPEED        6      
#define JUMP_INICIAL_SPEED          16
#define MAX_SPEED                   14 //13

// RTC
#define RTC_ADDR_REG                0x70
#define RTC_DATA_REG                0x71
#define REG_A                       10
#define SECONDS                     0
#define MINUTES                     2
#define HOURS                       4
#define DAY                         7
#define MONTH                       8
#define YEAR                        9
#define DISPLAY_GAP_HOR             24
#define DISPLAY_GAP_VER             5
#define DISPLAY_GAP_VER_DATE        42
#define SCORE_HOR_BEGIN             776
#define SCORE_BMP_SIZE              144

#define POWER_UPS_HOR               380
#define POWER_UPS_VER               15

#define CROSSHAIR_DIM               34
