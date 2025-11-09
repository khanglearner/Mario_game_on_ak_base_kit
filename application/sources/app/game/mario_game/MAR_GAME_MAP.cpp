#include "MAR_GAME_MAP.h"
#include "app.h"
#include "task_display.h"
#include "task_list.h"
#include "screens_bitmap.h"
#include <vector>

using namespace std;

/*==============================*/
/*           DEFINE             */
/*==============================*/
#define MAR_MAP_MOVE_X       (3)
#define MAR_MAP_WIDTH        (128)
#define MAX_MAP              (7)  
#define NUM_MAP              (7)   

/*==============================*/
/*           GLOBAL             */
/*==============================*/
static int a = 1;   // current map version index (1..7)
vector<mar_map_info_t> mar_vec_map;

vector<vector<uint8_t>> mar_vec_limit_wall_y = {
	{2, 2, 1, 3, 2, 2, 2, 4, 4, 3, 3, 3},
	{1, 3, 3, 1, 1, 2, 3, 4, 2, 2, 2, 1},
	{2, 2, 1, 3, 2, 2, 2, 4, 4, 3, 3, 3},
};

/*==============================*/
/*           FUNCTIONS          */
/*==============================*/

// clear all walls
static inline void mar_game_map_reset() {
	if (!mar_vec_map.empty()) {
		mar_vec_map.clear();
		mar_vec_map.shrink_to_fit();
	}
}

static void update_wall_ver(uint8_t i, bool direction) {
	// direction == true  -> MOVE LEFT
	// direction == false -> MOVE RIGHT

	switch (a) {
		case 1: mar_vec_map[i].ver = MAR_MAP_I;   break;
		case 2: mar_vec_map[i].ver = MAR_MAP_II;  break;
		case 3: mar_vec_map[i].ver = MAR_MAP_III; break;
		case 4: mar_vec_map[i].ver = MAR_MAP_IV;  break;
		case 5: mar_vec_map[i].ver = MAR_MAP_V;   break;
		case 6: mar_vec_map[i].ver = MAR_MAP_VI;  break;
		case 7: mar_vec_map[i].ver = MAR_MAP_VII; break;
		default: a = 1; mar_vec_map[i].ver = MAR_MAP_I; break;
	}

	if (direction)
		a++;
	else
		a--;

	if (a < 1) a = MAX_MAP;
	if (a > MAX_MAP) a = 1;
}

static void mar_game_map_setup() {
	for (uint8_t i = 0; i < NUM_MAP; i++) {
		mar_map_info_t temp_wall;
		temp_wall.x = i * MAR_MAP_WIDTH;
		temp_wall.y = 0;
		temp_wall.ver = MAR_MAP_I;
		mar_vec_map.push_back(temp_wall);
	}
}

/*==============================*/
/*         MAP MOVEMENT         */
/*==============================*/

// move map to the left
static void mar_game_map_move_left() {
	if (mar_vec_map.empty()) return;

	for (uint8_t i = 0; i < NUM_MAP; i++) {
		mar_vec_map[i].x -= MAR_MAP_MOVE_X;

		// if wall goes completely off left edge
		if (mar_vec_map[i].x + MAR_MAP_WIDTH <= 0) {
			update_wall_ver(i, true);
			// reposition behind last wall
			mar_vec_map[i].x += NUM_MAP * MAR_MAP_WIDTH;
		}
	}

	// rotate wall Y pattern
	for (auto &row : mar_vec_limit_wall_y) {
		row.push_back(row.front());
		row.erase(row.begin());
	}
}

// move map to the right
static void mar_game_map_move_right() {
	if (mar_vec_map.empty()) return;

	for (uint8_t i = 0; i < NUM_MAP; i++) {
		mar_vec_map[i].x += MAR_MAP_MOVE_X;

		// if wall goes completely off right edge
		if (mar_vec_map[i].x >= LCD_WIDTH) {
			update_wall_ver(i, false);
			// reposition before first wall
			mar_vec_map[i].x -= NUM_MAP * MAR_MAP_WIDTH;
		}
	}

	for (auto &row : mar_vec_limit_wall_y) {
		row.insert(row.begin(), row.back());
		row.pop_back();
	}
}

/*==============================*/
/*           TASK HANDLER       */
/*==============================*/

void task_mar_map_handle(ak_msg_t *msg) {
	switch (msg->sig) {
		case MAR_GAME_MAP_RESET_SIG:
			APP_DBG_SIG("MAR_GAME_MAP_RESET\n");
			mar_game_map_reset();
			break;

		case MAR_GAME_MAP_SETUP_SIG:
			APP_DBG_SIG("MAR_GAME_MAP_SETUP\n");
			mar_game_map_setup();
			mar_game_map_move_right();
			break;

		case MAR_GAME_MAP_MOVE_LEFT_SIG:
			mar_game_map_move_left();
			break;

		case MAR_GAME_MAP_MOVE_RIGHT_SIG:
			mar_game_map_move_right();
			break;

		case MAR_GAME_MAP_MOVE_LEFT_TIMER_SIG:
			mar_game_map_move_left();
			break;

		case MAR_GAME_MAP_MOVE_RIGHT_TIMER_SIG:
			mar_game_map_move_right();
			break;

		default:
			break;
	}
}
