//Header
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "TetrisView.h"
#include "TetrisManager.h"
#include "Constant.h"
#include "Util.h"
#include "Menu.h"

#pragma comment(lib, "winmm.lib")//PlaySound()를 사용하기 위해서 winmm.lib를 읽어온다.

#define TETRIS_BACKGROUND_MUSIC_FILE_NAME "..\\res\\tetris_background_music.wav"

/********Constant********
PROCESS_REACHED_CASE_COUNT : 바닥에 블럭이 닿았을 때 블럭이 굳지않고 유지되는 시간을 결정
ADD_ID_POSITION_X_TO_PRINT, ADD_ID_POSITION_X_TO_PRINT : 게임 종료 후 Rank를 추가하는 화면에서 처음으로 커서가 위치하는 x,y좌표
SETTING_POSITION_X_TO_PRINT,SETTING_POSITION_Y_TO_PRINT : 설정메뉴에서 처음으로 커서가 위치하는 x,y좌표
*************************/
#define PROCESS_REACHED_CASE_COUNT 2
#define ADD_ID_POSITION_X_TO_PRINT 25
#define ADD_ID_POSITION_Y_TO_PRINT 10
#define SETTING_POSITION_X_TO_PRINT 30
#define SETTING_POSITION_Y_TO_PRINT 10

enum MenuStartPosition {
	//변수 : 각 메뉴에서 처음으로 커서가 위치되는 X,Y좌표 위치
	MAIN_MENU_X = 18, MAIN_MENU_Y = 7, PAUSE_MENU_X = 5, PAUSE_MENU_Y = 12, END_MENU_X = 14, END_MENU_Y = 8
};

void TetrisView_StartGame(TetrisView* tetrisView) {
	PlaySound(TEXT(TETRIS_BACKGROUND_MUSIC_FILE_NAME), NULL, SND_ASYNC | SND_LOOP);//배경음악을 반복재생
	if (!(tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL)) { //게임을 시작하기 전에 게임을 실행할 레벨을 입력할 수 있는데, 이때의 레벨 범위가 유효한지(1~10)를 검사
		tetrisView->level = MIN_SPEED_LEVEL; //레벨의 범위가 유효하지 않으면 가장 낮은레벨(1)로 설정
	}//if
	TetrisManager_Init(&tetrisView->tetrisManager, tetrisView->level);//변수 tetrisView->tetrisManager 초기화

	/*화면을 지운 뒤 게임화면에 출력되는 font, block등의 색을 지정하고, 위치에 맞춰서 화면을 출력*/
	system("cls");
	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);
	TetrisManager_PrintBoard(&tetrisView->tetrisManager);
	TetrisManager_PrintDetailInfomation(&tetrisView->tetrisManager);
}

void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction) {

	//it is used to move left or right at bottom in case of space which you want to move is available
	static int processReachedCaseCount = 0;
	if (processType == DIRECTION) {//방향키를 누른경우
		TetrisManager_ProcessDirection(&tetrisView->tetrisManager, direction);
	}
	else if (processType == DIRECT_DOWN) {//스페이스바를 누른 경우
		TetrisManager_ProcessDirectDown(&tetrisView->tetrisManager);
	}
	else if (processType == AUTO) {//자동으로 내려오는 경우
		TetrisManager_ProcessAuto(&tetrisView->tetrisManager);
	}
	if (TetrisManager_IsReachedToBottom(&tetrisView->tetrisManager, MOVING_BLOCK)) {//현재 이동중인 블럭이바닥에 닿았을 때
		if (processType == DIRECT_DOWN) {//스페이스바를 누른 경우
			processReachedCaseCount = 0;
			if (TetrisManager_ProcessReachedCase(&tetrisView->tetrisManager) == END) {
				TetrisView_EndGame(tetrisView);
				return;
			}
		}
		else {//스페이스바를 누르지 않고 블럭이 바닥에 도달했을 때

			/*processReachedCaseCount가 PROCESS_REACHED_CASE_COUNT만큼 증가할때까지는 바닥에서 바로 굳지않고(else) 블럭 제어 가능
			  processReachedCaseCount가 PROCESS_REACHED_CASE_COUNT와 값이 같아지면(if) 블럭이 굳음*/
			if (processReachedCaseCount == PROCESS_REACHED_CASE_COUNT) {
				if (TetrisManager_ProcessReachedCase(&tetrisView->tetrisManager) == END) {//게임이 끝난경우
					TetrisView_EndGame(tetrisView);
					return;
				}

				processReachedCaseCount = 0;//다음 블럭을 위해 processReachedCaseCount 초기화
			}
			else {
				processReachedCaseCount++;
			}
		}
	}
	TetrisManager_ProcessDeletingLines(&tetrisView->tetrisManager);//삭제될 라인을 찾아서 테트리스판 갱신
}

void TetrisView_PauseGame(TetrisView* tetrisView) {
	/*사용자가 일시정지했을때 게임화면 설정*/
	TetrisManager_PauseTotalTime(&tetrisView->tetrisManager);
	PlaySound(NULL, 0, 0);//일시정지하면 배경음악이 일시정지됨
	FontUtil_ChangeFontColor(LIGHT_YELLOW);//Pause됬을때 보여지는 메뉴의 폰트 색상
	TetrisView_ProcessPauseMenu(tetrisView);
	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);

	switch (tetrisView->pauseMenu) {//사용자가 선택한 일시정지 메뉴
	case RESUME_PAUSE_MENU:
		TetrisManager_StartTotalTime(&tetrisView->tetrisManager);
		PlaySound(TEXT(TETRIS_BACKGROUND_MUSIC_FILE_NAME), NULL, SND_ASYNC | SND_LOOP);
		break;
	case MAIN_MENU_PAUSE_MENU:
		TetrisManager_StopTotalTime(&tetrisView->tetrisManager);
		break;
	}
}

void TetrisView_EndGame(TetrisView* tetrisView) {
	TetrisManager_PauseTotalTime(&tetrisView->tetrisManager);//Rank에 기록을 등록할 수 있으므로 시간을 초기화하지 않고 일시정지함
	PlaySound(NULL, 0, 0);
	TetrisView_ProcessEndMenu(tetrisView);//끝났을 때 화면 출력
}

void TetrisView_ShowRanking(TetrisView* tetrisView) {
	/*Rank를 보여주기위한 과정
	Create로 rank정보 저장 공간 생성 -> 파일에서 rank정보를 load하여 저장 -> 출력 -> 출력 후 메모리 공간을 확보해야하므로 정보 저장 공간 Destroy*/

	RankingManager_Create(&tetrisView->rankingManager);
	RankingManager_Load(&tetrisView->rankingManager);
	RankingManager_Print(&tetrisView->rankingManager);
	RankingManager_Destroy(&tetrisView->rankingManager);
}

void TetrisView_AddRanking(TetrisView* tetrisView) {
	Ranking ranking;
	int x = ADD_ID_POSITION_X_TO_PRINT;
	int y = ADD_ID_POSITION_Y_TO_PRINT;//x,y : 커서의 위치
	int i;
	char id[ID_SIZE + 1] = { '\0', };

	/*화면을 초기화한 후 Rank를 등록할 아이디를 입력하는 화면 출력*/
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("┏━━━━━━━━━━━━━┓");
	CursorUtil_GotoXY(x, y++);
	printf("┃Input ID (%d chars limit!)┃", ID_SIZE);
	CursorUtil_GotoXY(x, y++);
	printf("┃:                         ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┗━━━━━━━━━━━━━┛");
	x += 4;
	y -= 2;
	CursorUtil_GotoXY(x, y++);
	CursorUtil_Show();//아이디가 입력될 위치에 커서를 보여줌
	fgets(id, ID_SIZE + 1, stdin);//아이디를 입력받음
	CursorUtil_Hide();//커서를 보이지않게 함
	for (i = ID_SIZE; i >= 0; i--) {//입력한 아이디를 배열에 저장
		if (id[i] == '\n') {//아이디가 입력할 수 있는 아이디 길이보다 짧은 경우
			id[i] = '\0';
			break;
		}//if
	}//for

	/*id길이를 초과한 부분 제거*/
	if (i == -1) {
		while (getchar() != '\n');
	}

	/*현재 Rank를 ranking변수에 저장*/
	strcpy(ranking.id, id);
	ranking.score = tetrisView->tetrisManager.score;
	ranking.level = tetrisView->tetrisManager.speedLevel;
	ranking.deletedLineCount = tetrisView->tetrisManager.deletedLineCount;
	ranking.totalTime = tetrisView->tetrisManager.totalTime;
	ranking.timestamp = time(NULL);

	/*전체 Ranking을 보여줌*/
	RankingManager_Create(&tetrisView->rankingManager);
	RankingManager_Load(&tetrisView->rankingManager);
	RankingManager_Add(&tetrisView->rankingManager, ranking);
	RankingManager_Save(&tetrisView->rankingManager);
	RankingManager_Destroy(&tetrisView->rankingManager);
}

void TetrisView_ShowSetting(TetrisView* tetrisView) {
	int x = SETTING_POSITION_X_TO_PRINT;
	int y = SETTING_POSITION_Y_TO_PRINT;//x,y : 커서의 위치
	if (!(tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL)) {
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("┏━━━━━━━━━┓");
	CursorUtil_GotoXY(x, y++);
	printf("┃Current Level : %2d┃", tetrisView->level);
	CursorUtil_GotoXY(x, y++);
	printf("┃New Level (%d ~ %d)┃", MIN_SPEED_LEVEL, MAX_SPEED_LEVEL);
	CursorUtil_GotoXY(x, y++);
	printf("┃:                 ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┗━━━━━━━━━┛");
	x += 4;
	y -= 2;
	CursorUtil_GotoXY(x, y++);
	CursorUtil_Show();
	scanf("%d", &tetrisView->level);
	CursorUtil_Hide();
	if (tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL) {//레벨 범위가 유효할 경우

	}
	else if (tetrisView->level < MIN_SPEED_LEVEL) {//입력한 레벨의 범위가 최소레벨(1)보다 작은 경우
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	else if (tetrisView->level > MAX_SPEED_LEVEL) {//입력한 레벨의 범위가 최대레벨(10)보다 큰 경우
		tetrisView->level = MAX_SPEED_LEVEL;
	}
	else {
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	while (getchar() != '\n');//버퍼에 남아있는 값을 비움
}

void TetrisView_ProcessMainMenu(TetrisView* tetrisView) {
	/****변수 설명****
	items = [가능한 메뉴의 최대 개수][가능한 메뉴의 제목 길이]인 배열, 메뉴 목록 저장
	menuCount : 메뉴 개수
	x,y : 커서의 위치
	******************/
	Menu menu = { 0, };
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE] = {
		"[1] S T A R T", "[2] R A N K I N G", "[3] S E T T I N G", "[4] E X I T"
	};
	int menuCount = 4;
	int x = MAIN_MENU_X;
	int y = MAIN_MENU_Y;
	WindowUtil_ChangeWindowSize(WINDOW_LINE_SIZE, WINDOW_COL_SIZE);//출력화면크기 설정

	//메인메뉴 화면 출력
	system("cls");
	FontUtil_ChangeFontColor(RED);
	CursorUtil_GotoXY(x, y++);
	printf("■■■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_RED);
	CursorUtil_GotoXY(x, y++);
	printf("■■■");
	CursorUtil_GotoXY(x, y++);
	printf("■");
	CursorUtil_GotoXY(x, y++);
	printf("■■■");
	CursorUtil_GotoXY(x, y++);
	printf("■");
	CursorUtil_GotoXY(x, y++);
	printf("■■■");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_YELLOW);
	CursorUtil_GotoXY(x, y++);
	printf("■■■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(GREEN);
	CursorUtil_GotoXY(x, y++);
	printf("■■");
	CursorUtil_GotoXY(x, y++);
	printf("■ ■");
	CursorUtil_GotoXY(x, y++);
	printf("■■");
	CursorUtil_GotoXY(x, y++);
	printf("■ ■");
	CursorUtil_GotoXY(x, y++);
	printf("■  ■");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_BLUE);
	CursorUtil_GotoXY(x, y++);
	printf("■");
	CursorUtil_GotoXY(x, y++);
	printf("■");
	CursorUtil_GotoXY(x, y++);
	printf("■");
	CursorUtil_GotoXY(x, y++);
	printf("■");
	CursorUtil_GotoXY(x, y++);
	printf("■");
	x += 4;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_PURPLE);
	CursorUtil_GotoXY(x, y++);
	printf(" ■■");
	CursorUtil_GotoXY(x, y++);
	printf("■");
	CursorUtil_GotoXY(x, y++);
	printf("■■■");
	CursorUtil_GotoXY(x, y++);
	printf("    ■");
	CursorUtil_GotoXY(x, y++);
	printf(" ■■");
	x -= 22;
	y += 2;

	/*메인메뉴의 메뉴부분을 출력, 사용자가 메뉴 선택*/
	Menu_Create(&menu, items, menuCount, x, y, DEFAULT_FONT_COLOR);
	Menu_Print(&menu);
	tetrisView->mainMenu = Menu_ProcessKey(&menu) + 1;

	/*게임을 pause하거나 end할 일이 없으므로 0으로 설정*/
	tetrisView->pauseMenu = 0;
	tetrisView->endMenu = 0;
}

void TetrisView_ProcessPauseMenu(TetrisView* tetrisView) {
	Menu menu = { 0, };
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE] = {
		"[1] R E S U M E", "[2] M A I N M E N U"
	};
	int menuCount = 2;
	int x = PAUSE_MENU_X;
	int y = PAUSE_MENU_Y;

	/*Pause를 했을때 메뉴화면 출력, 메뉴선택기능*/
	Menu_Create(&menu, items, menuCount, x, y, LIGHT_YELLOW);
	Menu_Print(&menu);
	tetrisView->pauseMenu = Menu_ProcessKey(&menu) + 1;
	tetrisView->mainMenu = 0;
	tetrisView->endMenu = 0;

	if (tetrisView->pauseMenu == RESUME_PAUSE_MENU) {
		TetrisManager_PrintBoard(&tetrisView->tetrisManager);
	}
}

void TetrisView_ProcessEndMenu(TetrisView* tetrisView) {
	Menu menu = { 0, };
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE] = {
		"[1] R A N K I N G", "[2] M A I N M E N U", "[3] E X I T"
	};
	int menuCount = 3;
	int x = END_MENU_X;
	int y = END_MENU_Y;
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("■■■  ■  ■  ■■■          ■■■  ■  ■  ■■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■    ■  ■  ■              ■       ■ ■  ■ ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■    ■■■  ■■■  ■■■  ■■■  ■■■  ■  ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■    ■  ■  ■              ■      ■ ■   ■ ■");
	CursorUtil_GotoXY(x, y++);
	printf("  ■    ■  ■  ■■■          ■■■  ■  ■  ■■");
	x += 19;
	y += 2;
	/*게임이 끝났을때 메뉴화면 출력, 메뉴선택기능*/
	Menu_Create(&menu, items, menuCount, x, y, DEFAULT_FONT_COLOR);
	Menu_Print(&menu);
	tetrisView->endMenu = Menu_ProcessKey(&menu) + 1;
	tetrisView->mainMenu = 0;
	tetrisView->pauseMenu = 0;
}

DWORD TetrisView_GetDownMilliSecond(TetrisView* tetrisView) {
	return TetrisManager_GetDownMilliSecond(&tetrisView->tetrisManager);
}

void TetrisView_MakeHold(TetrisView* tetrisView) {
	TetrisManager_MakeHold(&tetrisView->tetrisManager);
}