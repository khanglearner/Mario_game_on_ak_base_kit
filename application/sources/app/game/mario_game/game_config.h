#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include "stdio.h"
#include <vector>

#include "ak.h"

using namespace std;

#define MAX_LCD_WDITH       (128)
#define MAX_LCD_HEIGHT      (64)

#define MAP_HEIGHT  54
#define MAP_WIDTH   128

#define FS_MAX_HISTORY      (12)

typedef enum 
{
    MAR_GAME_ON,
    MAR_GAME_OFF,
    MAR_GAME_OVER
} mar_game_state_t;

typedef struct {
    int8_t x;
    int8_t y;
} mar_game_coordinate_t;

// typedef struct {
//     bool fs_setting_sound = false;
//     uint8_t fs_setting_missle = 5;
//     fs_game_mode_t fs_setting_game_mode = FS_GAME_MODE_EASY;
// } fs_game_setting_t;

// extern fs_game_state_t fs_state_game;
// extern fs_game_setting_t fs_game_setting;
// extern uint8_t fs_game_score;
// extern uint16_t fs_game_score_history[FS_MAX_HISTORY];

extern mar_game_state_t mar_state_game;

#endif  //FS_CONFIG_H