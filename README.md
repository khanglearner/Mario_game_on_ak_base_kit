# Mario map game

## I. Giới thiệu
Trò chơi bắt đầu bằng màn hình **Menu game** với các lựa chọn sau: 
- **Archery Game:** Chọn vào để bắt đầu chơi game.
- **Setting:** Chọn vào để cài đặt các thông số của game.
- **Charts:** Chọn vào để xem top 3 điểm cao nhất đạt được.
- **Exit:** Thoát menu vào màn hình chờ.
- **Mario:** Vào game mario

### Đối tượng (Object) trong game:

|**Map**|Bản đồ|Di chuyển sang trái, phải theo timer|


### Ghi chú:
**SCREEN_ENTRY:** Cài đặt các thiết lập ban đầu cho đối tượng trong game.
- **MAR_GAME_MAP_SETUP_SIG:** Thiết lập map thứ nhất
- **Setup timer - Time tick:** Khởi tạo Timer - Time tick cho game.
- **STATE (GAME_ON):** Cập nhật trạng thái game -> GAME_ON

**GAME PLAY:** Quá trình hoạt động của game.

**GAME PLAY - Normal:** Game hoạt động ở trạng thái bình thường.
- **MAR_GAME_DISPLAY_ON_TICK:** Signal do Timer - Time tick gửi đến.
- **MAR_GAME_MAP_MOVE_LEFT_SIG:** Gửi tính hiệu đến map handle để cập nhật bản đồ kế tiếp (dịch chuyển sang phải).
**Screen**.

**GAME PLAY - Action:** Game hoạt động ở trạng thái có tác động của các nút nhấn.
- **MAR_GAME_MAP_MOVE_LEFT_TIMER_SIG:** Player nhấn nút **[Up]** để dịch chuyển map sang trái theo timer
- **MAR_GAME_MAP_MOVE_LEFT_TIMER_SIG:** Player nhấn nút **[Down]** để dịch chuyển map sang trái theo timer.



#### 2.2.1 Thuộc tính đối tượng MAP
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


**Các biến quan trọng:**
- **mar_game_state:** Trạng thái quả trò chơi.
  - GAME_OFF: Tắt .
  - GAME_ON: Bật.
  - GAME_OVER: Đã thua.

#### 2.2.2 Task

	{MAR_GAME_TASK_DISPLAY_GAME_ON_ID	,	TASK_PRI_LEVEL_4	,	task_scr_mar_game_on_handle		},
	{MAR_GAME_TASK_MAP_ID				,	TASK_PRI_LEVEL_4	,	task_mar_map_handle				},
	{MAR_GAME_TASK_GAME_OVER_ID			,	TASK_PRI_LEVEL_4	,	task_scr_mar_game_over_handle	},
	{MAR_GAME_TASK_MARIO_ID				,	TASK_PRI_LEVEL_4	,	task_mar_game_mario_handle		},

#### 2.2.3 Message & Signal
**Message** được chia làm 2 loại chính, Message chỉ chứa Signal và Message vừa chứa Signal và Data. **Message** tương đương với **Signal**


- **Task: MAR_GAME_TASK_DISPLAY_GAME_ON_ID**
        **Signal**
		MAR_GAME_INITIAL_SETUP = AK_USER_DEFINE_SIG,
		MAR_GAME_DISPLAY_ON_TICK = AK_USER_DEFINE_SIG,
		MAR_GAME_RESET,

- **Task: MAR_GAME_TASK_MAP_ID**
		MAR_GAME_MAP_SETUP_SIG = AK_USER_DEFINE_SIG,
		MAR_GAME_MAP_RESET_SIG,
		MAR_GAME_MAP_UPDATE_ON_TICK_SIG,
		
		MAR_GAME_MAP_MOVE_LEFT_TIMER_SIG,
		MAR_GAME_MAP_MOVE_LEFT_SIG,
		MAR_GAME_MAP_MOVE_RIGHT_TIMER_SIG,
		MAR_GAME_MAP_MOVE_RIGHT_SIG


## III. Hướng dẫn chi tiết code trong đối tượng
### **Đối tượng Map**

**Code**

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

// dịch chuyển map sang trái
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

// Dịch chuyển map sang phải
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
