#ifndef GAME_BITMAP_H
#define GAME_BITMAP_H

#include "pgmspace.h"
#include "view_render.h"

#define MAP_HEIGHT  54
#define MAP_WIDTH   128
//Map bitmaps
extern const unsigned char MAP_I    [] PROGMEM;
extern const unsigned char MAP_II   [] PROGMEM;
extern const unsigned char MAP_III  [] PROGMEM;
extern const unsigned char MAP_IV   [] PROGMEM;
extern const unsigned char MAP_V    [] PROGMEM;
extern const unsigned char MAP_VI   [] PROGMEM;
extern const unsigned char MAP_VII  [] PROGMEM;

//Mario bitmaps
//stand
extern const unsigned char mario_stand_right_10x10 [] PROGMEM;
extern const unsigned char mario_stand_left_10x10 [] PROGMEM;


#endif  //FS_BITMAP_H