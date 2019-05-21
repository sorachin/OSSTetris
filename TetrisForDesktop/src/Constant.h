#ifndef _CONSTANT_H
#define _CONSTANT_H

enum Boolean {
	False, True
};

enum Direction { //블럭에 대해 들어온 입력(방향키)
	UP, RIGHT, DOWN, LEFT
};

enum ProcessType { //블럭이 이동하는 방법(DIRECTION:좌,우 , DIRECT_DOWN:한 번에 아래로 이동, AUTO:자동으로 아래로 이동)
	DIRECTION, DIRECT_DOWN, AUTO
};

enum KeyCode {//각 키에 대한 코드값
	UP_KEY_CODE = 72, LEFT_KEY_CODE = 75,
	RIGHT_KEY_CODE = 77, DOWN_KEY_CODE = 80,
	SPACE_BAR_KEY_CODE = 32, ESC_KEY_CODE = 27,
	L_KEY_CODE = 108, ENTER_KEY_CODE = 13
};

enum FontColor {
	BLACK, BLUE, GREEN, JADE, RED, PURPLE, YELLOW, WHITE, GRAY,
	LIGHT_BLUE, LIGHT_GREEN, LIGHT_JADE, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, LIGHT_WHITE, WHITE_INVERSION = 240
};

enum BoardType { //게임 board의 각 위치에 대한 정보
	EMPTY, //비어있음
	MOVING_BLOCK, FIXED_BLOCK, SHADOW_BLOCK, //움직이는 블럭, 아래에서 굳은 블럭, moving block이 떨어질 위치(미리보기)
	LEFT_WALL, RIGHT_WALL, BOTTOM_WALL, TOP_WALL,
	LEFT_TOP_EDGE, RIGHT_TOP_EDGE, LEFT_BOTTOM_EDGE, RIGHT_BOTTOM_EDGE
};

enum MainMenu {//메인메뉴의 각 메뉴에대한 상수값 정의
	START_MAIN_MENU = 1, RANKING_MAIN_MENU, SETTING_MAIN_MENU, EXIT_MAIN_MENU
};

enum PauseMenu {//일시정지메뉴의 각 메뉴에대한 상수값 정의
	RESUME_PAUSE_MENU = 1, MAIN_MENU_PAUSE_MENU
};

enum EndMenu {//게임이 종료되었을 때 나타나는 메뉴에대한 상수값 정의
	RANKING_END_MENU = 1, MAIN_MENU_END_MENU, EXIT_END_MENU
};

//속도에 대한 상수값 정의
#define MAX_SPEED_LEVEL 10
#define MIN_SPEED_LEVEL 1

#define DEFAULT_FONT_COLOR LIGHT_WHITE

#endif