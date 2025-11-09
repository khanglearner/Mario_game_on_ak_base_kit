#include <queue>

#include "app.h"
#include "app_dbg.h"
#include "task_display.h"
#include "task_list.h"
#include "timer.h"

#include "MAR_GAME_MAP.h"
#include "game_config.h"

#include "scr_mario_game.h"
#include "screens.h"


#include "game_bitmap.h"

/*****************************************************************************/
/* Variable Declaration - Mario game screen */
/*****************************************************************************/
uint8_t mar_game_state = GAME_OFF;
/*****************************************************************************/
/* View - Mario game screen*/
/*****************************************************************************/


/***********************************************************
* VARIABLE AND ARRAY BITMAP VIEW GAME ON
***********************************************************/
#define MAR_NUM_WALL_ICON        (7)


static const unsigned char *mar_arr_map_icon[MAR_NUM_WALL_ICON] = {
	MAP_I, MAP_II, MAP_III, MAP_IV, MAP_V, MAP_VI, MAP_VII
};



/***********************************************************
* VIEW - GAME ON
***********************************************************/

static void view_scr_mar_game_on();

view_dynamic_t dyn_view_item_mario_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_mar_game_on
};

view_screen_t scr_mario_game = {
	&dyn_view_item_mario_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};


// show all map
void mar_game_view_map() {
	if (!mar_vec_map.empty()) {
		for (uint8_t i = 0; i < NUM_MAP; i++) {
			view_render.drawBitmap(mar_vec_map[i].x, mar_vec_map[i].y, \
									mar_arr_map_icon[mar_vec_map[i].ver], \
									MAP_WIDTH, MAP_HEIGHT, \
									WHITE);
		}
	}
}

void mar_game_view_mario() {
	if(mar_game_mario.visible == WHITE) {
		if(mar_game_mario.action_image == 1) {				//Walking right
			view_render.drawBitmap(	mar_game_mario.x, \
									mar_game_mario.y, \
									mario_stand_right_10x10, \
									SIZE_BITMAP_MARIO_X, \
									SIZE_BITMAP_MARIO_Y, \
									WHITE);
		}
		else {
			view_render.drawBitmap(	mar_game_mario.x, \
									mar_game_mario.y, \
									mario_stand_left_10x10, \
									SIZE_BITMAP_MARIO_X, \
									SIZE_BITMAP_MARIO_Y, \
									WHITE);
		}
	}
}


// 	view_render.print("SCORE:");
// 	view_render.print(fs_game_score);

// 	view_render.setCursor(55, 57);
// 	view_render.print(" TRIGGERS:");
// 	view_render.print(FS_MAX_MISSLE - fs_vec_missile.size());
// }

void view_scr_mar_game_on() {
	mar_game_view_map();
	// fs_game_view_obstacle();
	// fs_game_view_explosion();
	
	// fs_game_view_plane();
	// fs_game_view_missle();
	// fs_game_view_infor_fly();
}

/***********************************************************
* SCREEN GAME ON HANDLE
***********************************************************/
void mar_game_time_tick_setup() {
	timer_set(	AC_TASK_DISPLAY_ID, \
				MAR_GAME_DISPLAY_ON_TICK, \
				MAR_GAME_TIME_TICK_INTERVAL, \
				TIMER_PERIODIC);
}	
void task_scr_mar_game_on_handle(ak_msg_t *msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY\n");
			mar_game_time_tick_setup();

			// Setup map and game state
			task_post_pure_msg(MAR_GAME_TASK_MAP_ID, MAR_GAME_MAP_SETUP_SIG);
			//task_post_pure_msg(MAR_GAME_TASK_MARIO_ID, MAR_GAME_MARIO_SETUP_SIG);
			mar_game_state = GAME_PLAY;

			// Start display refresh timer
			timer_set(AC_TASK_DISPLAY_ID,		\
					  MAR_GAME_DISPLAY_ON_TICK,\
					  MAR_GAME_TIME_TICK_INTERVAL,\
					  TIMER_PERIODIC);

			// //set timer for map update
			// timer_set(MAR_GAME_TASK_MAP_ID,	\
			// 		  MAR_GAME_MAP_UPDATE_ON_TICK_SIG,\
			// 		  MAR_GAME_TIME_TICK_INTERVAL,\
			// 		  TIMER_PERIODIC);
			
			break;
		}

		case MAR_GAME_DISPLAY_ON_TICK: {  
			APP_DBG_SIG("MAR_GAME_DISPLAY_ON_TICK\n");
			// Periodic update tick
			task_post_pure_msg(MAR_GAME_TASK_MAP_ID, MAR_GAME_MAP_MOVE_LEFT_SIG);
			//task_post_pure_msg(MAR_GAME_TASK_MARIO_ID, MAR_GAME_MARIO_MOVE_RIGHT_SIG);
			break;
		}

		// case MAR_GAME_MAP_UPDATE_ON_TICK_SIG: {
		// 	APP_DBG_SIG("MAR_GAME_MAP_UPDATE_ON_TICK_SIG\n");
		// 	// Refresh view
		// 	view_render.initialize();
		// 	view_render_display_on();
		// 	break;
		// }
		case MAR_GAME_RESET: {
			APP_DBG_SIG("MAR_GAME_RESET\n");
			mar_game_state = GAME_OFF;
			break;
		}

		default:
			break;
	}
}
