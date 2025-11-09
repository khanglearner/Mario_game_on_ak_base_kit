#ifndef MAR_GAME_MARIO_H
#define MAR_GAME_MARIO_H

#include <vector>
#include <stdint-gcc.h> //for int16_t, int8 t
#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

using namespace std;

#define SIZE_BITMAP_MARIO_X    (10)
#define SIZE_BITMAP_MARIO_Y    (10)

typedef struct {
    bool visible;
    int8_t x;
    int8_t y;
    uint8_t action_image;
} mar_game_mario_coordinate_t;

extern mar_game_mario_coordinate_t mar_game_mario;


#endif // MAR_GAME_MARIO_H