#ifndef MAR_GAME_MAP_H
#define MAR_GAME_MAP_H

#include <vector>
#include <stdint-gcc.h> //for int16_t, int8 t

using namespace std;

#define NUM_MAP         (7)

typedef enum {
    MAR_MAP_I   = 0,
    MAR_MAP_II  = 1,
    MAR_MAP_III = 2,
    MAR_MAP_IV  = 3,
    MAR_MAP_V   = 4,
    MAR_MAP_VI  = 5,
    MAR_MAP_VII = 6
} mar_ver_map_t;

typedef struct {
    int16_t x;
    int8_t y;
    mar_ver_map_t ver;
} mar_map_info_t;

extern vector<vector<uint8_t>> mar_ver_limit_map_y;
extern vector<mar_map_info_t> mar_vec_map;

#endif 