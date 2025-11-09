#include "MAR_GAME_MARIO.h"
#include "app.h"
#include "task_display.h"
#include "task_list.h"
#include "MAR_GAME_MAP.h"

#define Mario_maximum_jump_y    (15)
#define Mario_gravity_decrease_y    (1)
#define Mario_move_x_speed   (8)
/*****************************************************************************/
/*           GLOBAL             */
/*****************************************************************************/
mar_game_mario_coordinate_t mar_game_mario;

/*****************************************************************************/
static inline void mar_game_mario_reset() {
    mar_game_mario.visible = false;
    mar_game_mario.x = 5;
    mar_game_mario.y = 10;
    mar_game_mario.action_image = 1;
}

static void mar_game_mario_setup() {
    mar_game_mario.visible = true;
    mar_game_mario.x = 5;
    mar_game_mario.y = 10;
    mar_game_mario.action_image = 1;
}

// Mario decrease y due to gravity
static void mar_game_mario_fall() {
    if (mar_game_mario.y < 50) {        // if Mario is above ground
        mar_game_mario.y += Mario_gravity_decrease_y;
    }
    // if (mar_game_mario.y > 50) {
    //     mar_game_mario.y += Mario_gravity_decrease_y;
    // }
}

static void mar_game_mario_jump() {
        mar_game_mario.y -= Mario_maximum_jump_y;

}
static void mar_game_mario_move_left() {
    if (mar_game_mario.x > 0) {        // if Mario is not at the left edge
        mar_game_mario.x -= 1;
    }
    if(mar_game_mario.x < 0) {
        mar_game_mario.x = 0;
    }
}

static void mar_game_mario_move_right() {
    //if (mar_game_mario.x < (MAX_LCD_WDITH - SIZE_BITMAP_MARIO_X)) {  // if Mario is not at the right edge
        mar_game_mario.x += Mario_move_x_speed;
    // }
    // if(mar_game_mario.x > (MAX_LCD_WDITH - SIZE_BITMAP_MARIO_X)) {
    //     mar_game_mario.x = MAX_LCD_WDITH - SIZE_BITMAP_MARIO_X;
    // }
}
/*****************************************************************************/
/*           TASK HANDLER       */
/*****************************************************************************/
void task_mar_game_mario_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        case MAR_GAME_MARIO_SETUP_SIG:
            APP_DBG_SIG("MAR_GAME_MARIO_SETUP\n");
            mar_game_mario_setup();
            break;
        case MAR_GAME_MARIO_RESET_SIG:
            APP_DBG_SIG("MAR_GAME_MARIO_RESET\n");
            mar_game_mario_reset();
            break;

        case MAR_GAME_MARIO_FALLING_SIG:
            APP_DBG_SIG("MAR_GAME_MARIO_FALLING\n");
            mar_game_mario_fall();
            break;

        case MAR_GAME_MARIO_JUMP_SIG:
            APP_DBG_SIG("MAR_GAME_MARIO_JUMP\n");
            mar_game_mario_jump();
            break;

        case MAR_GAME_MARIO_MOVE_LEFT_SIG:
            APP_DBG_SIG("MAR_GAME_MARIO_MOVE_LEFT\n");
            mar_game_mario_move_left();
            break;

        case MAR_GAME_MARIO_MOVE_RIGHT_SIG:
            APP_DBG_SIG("MAR_GAME_MARIO_MOVE_RIGHT\n");
            mar_game_mario_move_right();
            break;

        default:
            break;
    }
}