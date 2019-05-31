#include <stdio.h>
#include <process.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "TetrisManager.h"
#include "Util.h"
#include "Constant.h"

/*
SPEED_LEVEL_OFFSET = 속도 레벨이 하나씩 증가할수록 벌어질 offset을 매크로 상수로 정의
LEVELP_UP_CONDITION = 라인을 몇 줄 지워야 속도 레벨이 1 증가하는지 (3 줄 삭제시 1 레벨 증가)
STATUS_POSITION_X_TO_PRINT = 게임의 전반적인 상태를 출력할 위치의 x좌표
STATUS_POSITION_Y_TO_PRINT = 게임의 전반적인 상태를 출력할 위치의 y좌표
LINES_TO_DELETE_HIGHLIGHTING_COUNT = 라인 삭제시 하이라이트 될 횟수
LINES_TO_DELETE_HIGHLIGHTING_MILLISECOND = 라인 삭제시 하이라이트때 지연 시간
*/
#define MAX_MAKE_OBSTACLE_ONE_LINE_COUNT 2
<<<<<<< HEAD
#define MILLI_SECONDS_PER_SECOND 1000   
#define INITIAL_SPEED 300               
#define SPEED_LEVEL_OFFSET 40           
#define LEVELP_UP_CONDITION 3           
#define STATUS_POSITION_X_TO_PRINT 38 
#define STATUS_POSITION_Y_TO_PRINT 1    
#define LINES_TO_DELETE_HIGHLIGHTING_COUNT 3 
#define LINES_TO_DELETE_HIGHLIGHTING_MILLISECOND 100 
#define BOARD_TYPES_TO_PRINT_ROW_SIZE 12 
=======
#define MILLI_SECONDS_PER_SECOND 1000
#define INITIAL_SPEED 300
#define SPEED_LEVEL_OFFSET 30
#define LEVELP_UP_CONDITION 5
#define STATUS_POSITION_X_TO_PRINT 38
#define STATUS_POSITION_Y_TO_PRINT 1

#define LINES_TO_DELETE_HIGHLIGHTING_COUNT 3
#define LINES_TO_DELETE_HIGHLIGHTING_MILLISECOND 100

#define BOARD_TYPES_TO_PRINT_ROW_SIZE 12
>>>>>>> fix
#define BOARD_TYPES_TO_PRINT_COL_SIZE 3

//테트리스판에 출력될 상자 타입
static const char boardTypesToPrint[BOARD_TYPES_TO_PRINT_ROW_SIZE][BOARD_TYPES_TO_PRINT_COL_SIZE] = {
	("  "), ("■"), ("▩"), ("□"), ("┃"), ("┃"), ("━"), ("━"), ("┏"), ("┓"), ("┗"), ("┛")
};

/*
**현재 소스 파일 내부에서만 사용될 static 함수들의 선언부**
_TetrisManager_PrintStatus = 스피드 레벨, 지운 줄 수, 점수 출력
_TetrisManager_PrintKeys = 조작법 출력
_TetrisManager_PrintBlock = 블럭 타입에 따라 출력
_TetrisManager_InitBoard = 초기 테트리스판 설정
_TetrisManager_UpSpeedLevel = 스피드레벨 증가
_TetrisManager_SearchLineIndexesToDelete = 삭제할 라인의 위치 확인
_TetrisManager_DeleteLines = 삭제할 라인을 지우는 역할
_TetrisManager_HighlightLinesToDelete = 라인이 지워질때 깜빡이는 처리
_TetrisManager_GetBlockByType = 블럭 타입 정의
_TetrisManager_MakeShadow = 블럭이 fix될 자리(shadow)
_TetrisManager_CheckValidPosition = 전달받은 방향으로 block 을 임시로 이동 or 회전시켜보고 그 블럭 상태가 유효한지 아닌지를 반환
_TetrisManager_ChangeBoardByDirection = 방향에 해당하는 처리에 대해서 블럭을 이동 혹은 회전시키고, 그에 따른 테트리스 판의 상태도 변경
_TetrisManager_ChangeBoardByStatus =status로 블럭의 위치를 테트리스판에서 변경하는 역할
_TetrisManager_OnTotalTimeThreadStarted = tetrisManager를 매개변수로 받는 thread가 실행되는 시간을 구하는 함수
_TetrisManager_PrintTotalTime = 실행 시간 출력
_TetrisManager_MakeObstacleOneLine = 장애물 생성
*/
static void _TetrisManager_PrintStatus(TetrisManager* tetrisManager, int x, int y);
static void _TetrisManager_PrintKeys( int x, int y);
static void _TetrisManager_PrintBlock(TetrisManager* tetrisManager, int blockType, int status);
static void _TetrisManager_InitBoard(TetrisManager* tetrisManager);
static void _TetrisManager_UpSpeedLevel(TetrisManager* tetrisManager);
static void _TetrisManager_SearchLineIndexesToDelete(TetrisManager* tetrisManager, int* indexes, int* count);
static void _TetrisManager_DeleteLines(TetrisManager* tetrisManager, int* indexes, int count);
static void _TetrisManager_HighlightLinesToDelete( int* indexes, int count);
static Block _TetrisManager_GetBlockByType(TetrisManager* tetrisManager, int blockType);
static void _TetrisManager_MakeShadow(TetrisManager* tetrisManager);
static int _TetrisManager_CheckValidPosition(TetrisManager* tetrisManager, int blockType, int direction);
static void _TetrisManager_ChangeBoardByDirection(TetrisManager* tetrisManager, int blockType, int direction);
static void _TetrisManager_ChangeBoardByStatus(TetrisManager* tetrisManager, int blockType, int status);
static DWORD WINAPI _TetrisManager_OnTotalTimeThreadStarted(void *tetrisManager);
static void _TetrisManager_PrintTotalTime(TetrisManager tetrisManager);
static void _TetrisManager_MakeObstacleOneLine(TetrisManager* tetrisManager);


void TetrisManager_Init(TetrisManager* tetrisManager, int speedLevel) {
	Block block;
	block.current = -1;
	_TetrisManager_InitBoard(tetrisManager);
	tetrisManager->block = Block_Make(True, block); //block_make 호출, 첫 블록이므로 true
	tetrisManager->shadow = tetrisManager->block;
	tetrisManager->isHoldAvailable = True;
	_TetrisManager_MakeShadow(tetrisManager);
	tetrisManager->deletedLineCount = 0;
	tetrisManager->speedLevel = speedLevel; //사용자로부터 입력 받은 speedLevel로 초기화
	tetrisManager->score = 0;
	tetrisManager->totalTimeThread = NULL;
	tetrisManager->totalTime = 0;
	tetrisManager->isTotalTimeAvailable = False; //게임이 실행중 아닐때(stop,pause) False, 실행중이면 true
}

void TetrisManager_ProcessDirection(TetrisManager* tetrisManager, int direction) {
	if (direction != DOWN) {
		_TetrisManager_PrintBlock(tetrisManager, SHADOW_BLOCK, EMPTY);
		_TetrisManager_ChangeBoardByStatus(tetrisManager, SHADOW_BLOCK, EMPTY);
	}
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, EMPTY);
	_TetrisManager_ChangeBoardByDirection(tetrisManager, MOVING_BLOCK, direction);
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, MOVING_BLOCK);
	if (direction != DOWN) {
		_TetrisManager_MakeShadow(tetrisManager);
	}
}

void TetrisManager_ProcessAuto(TetrisManager* tetrisManager) {
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, EMPTY);
	_TetrisManager_ChangeBoardByDirection(tetrisManager, MOVING_BLOCK, DOWN);
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, MOVING_BLOCK);
}

void TetrisManager_ProcessDirectDown(TetrisManager* tetrisManager) {
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, EMPTY);
	while (!TetrisManager_IsReachedToBottom(tetrisManager, MOVING_BLOCK)) {
		_TetrisManager_ChangeBoardByDirection(tetrisManager, MOVING_BLOCK, DOWN);
	}
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, MOVING_BLOCK);
}

void TetrisManager_ProcessDeletingLines(TetrisManager* tetrisManager) {
	int indexes[BOARD_ROW_SIZE]; // 삭제할 라인의 위치
	int count;				     // 삭제 라인의 총 수

	// use temp size (magic number)
	int x = 38;
	int y = 1;

	_TetrisManager_SearchLineIndexesToDelete(tetrisManager, indexes, &count);
	if (count > 0) { //만약 삭제할 라인이 있다면

		//during hightlighting the lines to delete, hide moving block and shadow block
		_TetrisManager_PrintBlock(tetrisManager, SHADOW_BLOCK, EMPTY);
		_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, EMPTY);
		_TetrisManager_HighlightLinesToDelete( indexes, count);
		_TetrisManager_DeleteLines(tetrisManager, indexes, count);
		_TetrisManager_ChangeBoardByStatus(tetrisManager, MOVING_BLOCK, MOVING_BLOCK);
		TetrisManager_PrintBoard(tetrisManager);
		_TetrisManager_PrintStatus(tetrisManager, x, y);
	}
}

int TetrisManager_IsReachedToBottom(TetrisManager* tetrisManager, int blockType) {
	int i;
	Block block = _TetrisManager_GetBlockByType(tetrisManager, blockType);
	for (i = 0; i < POSITIONS_SIZE; i++) { //블록의 네 상자의 x, y좌표 확인
		int x = Block_GetPositions(block)[i].x;
		int y = Block_GetPositions(block)[i].y;

		if (!(tetrisManager->board[x + 1][y] == EMPTY || tetrisManager->board[x + 1][y] == MOVING_BLOCK || tetrisManager->board[x + 1][y] == SHADOW_BLOCK)) {
			return True; //좌표의 아래가 empty, moving block이 아니면 true
		}
	}
	return False; //맞으면 false
}

int TetrisManager_ProcessReachedCase(TetrisManager* tetrisManager) {

	// use temp size (magic number)
	int x = 40;
	int y = 15;

	// if this variable equals to 
	static int makeObstacleOneLineCount = 0;

	_TetrisManager_PrintBlock(tetrisManager, SHADOW_BLOCK, EMPTY);
	_TetrisManager_ChangeBoardByStatus(tetrisManager, SHADOW_BLOCK, EMPTY);
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, EMPTY);
	_TetrisManager_ChangeBoardByStatus(tetrisManager, MOVING_BLOCK, FIXED_BLOCK); //FIXED_BLOCK 매개변수를 넘겨서 현재 블럭 위치를 테트리스판에서 FIXED_BLOCK 으로 변경
	_TetrisManager_PrintBlock(tetrisManager, FIXED_BLOCK, FIXED_BLOCK);
	tetrisManager->block = Block_Make(False, tetrisManager->block); //Block_Make함수 호출, 첫 번째 블록이 아니므로 false, 블록하나 생성
	_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, MOVING_BLOCK);
	_TetrisManager_MakeShadow(tetrisManager);
	if (makeObstacleOneLineCount == MAX_MAKE_OBSTACLE_ONE_LINE_COUNT) {
		if (tetrisManager->speedLevel == MAX_SPEED_LEVEL) {
			_TetrisManager_MakeObstacleOneLine(tetrisManager);
		}
		makeObstacleOneLineCount = 0;
	}
	else {
		makeObstacleOneLineCount++;
	}
	Block_PrintNext(tetrisManager->block, 0, x, y);
	x += 20;
	Block_PrintNext(tetrisManager->block, 1, x, y);
	tetrisManager->isHoldAvailable = True;
	if (TetrisManager_IsReachedToBottom(tetrisManager, MOVING_BLOCK)) { //함수를 호출하여 새로 생성한 블럭이 더 이상 밑으로 갈 수 있는지 체크해서 true면
		Block_Destroy(tetrisManager->block);
		return END; //게임종료
	}
	else {
		return PLAYING; //그대로 진행
	}
}

void TetrisManager_PrintBoard(TetrisManager* tetrisManager) {
	int i;
	int j;
	int x = 0;
	int y = 0;
	for (i = 0; i < BOARD_ROW_SIZE; i++) {
		CursorUtil_GotoXY(x, y++);
		for (j = 0; j < BOARD_COL_SIZE; j++) {
			switch (tetrisManager->board[i][j]) {
			case LEFT_TOP_EDGE: case RIGHT_TOP_EDGE: case LEFT_BOTTOM_EDGE: case RIGHT_BOTTOM_EDGE:
			case LEFT_WALL: case RIGHT_WALL: case TOP_WALL: case BOTTOM_WALL:
			case EMPTY:
				FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);
				break;
			case MOVING_BLOCK:
				FontUtil_ChangeFontColor(tetrisManager->block.color);
				break;
			case FIXED_BLOCK:
				FontUtil_ChangeFontColor(WHITE);
				break;
			case SHADOW_BLOCK:
				FontUtil_ChangeFontColor(GRAY);
				break;
			}
			printf("%s", boardTypesToPrint[tetrisManager->board[i][j]]);
			FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);
		}
	}
}

void TetrisManager_PrintDetailInfomation(TetrisManager* tetrisManager) {
	int x = STATUS_POSITION_X_TO_PRINT;
	int y = STATUS_POSITION_Y_TO_PRINT;
	_TetrisManager_PrintStatus(tetrisManager, x, y);
	x += 6;
	y += 4;
	_TetrisManager_PrintKeys( x, y);
	x -= 4;
	y += 10;
	Block_PrintNext(tetrisManager->block, 0, x, y);
	x += 20;
	Block_PrintNext(tetrisManager->block, 1, x, y);
	y += 5;
	Block_PrintHold(tetrisManager->block, x, y);
	TetrisManager_StartTotalTime(tetrisManager);
	_TetrisManager_PrintTotalTime(*tetrisManager);
}

DWORD TetrisManager_GetDownMilliSecond(TetrisManager* tetrisManager) {
	int i;
	DWORD milliSecond = INITIAL_SPEED;
	for (i = MIN_SPEED_LEVEL; i < tetrisManager->speedLevel; i++) { // 최소 속도 레벨부터 현재 속도 레벨까지 반복
		if (i < MAX_SPEED_LEVEL / 2) {
			milliSecond -= SPEED_LEVEL_OFFSET;
		}
		else { //그 뒤엔
			milliSecond -= (SPEED_LEVEL_OFFSET / 5); //블럭이 내려오는 속도를 줄임
		}
	}
	return milliSecond;
}

void TetrisManager_MakeHold(TetrisManager* tetrisManager) {

	// use temp size (magic number)
	int x = 60;
	int y = 20;

	if (tetrisManager->isHoldAvailable) {
		_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, EMPTY);
		_TetrisManager_ChangeBoardByStatus(tetrisManager, MOVING_BLOCK, EMPTY);
		_TetrisManager_PrintBlock(tetrisManager, SHADOW_BLOCK, EMPTY);
		_TetrisManager_ChangeBoardByStatus(tetrisManager, SHADOW_BLOCK, EMPTY);
		Block_ChangeCurrentForHold(&tetrisManager->block);
		tetrisManager->isHoldAvailable = False;
		Block_PrintHold(tetrisManager->block, x, y);
		_TetrisManager_PrintBlock(tetrisManager, MOVING_BLOCK, MOVING_BLOCK);
		_TetrisManager_MakeShadow(tetrisManager);
	}
}

<<<<<<< HEAD
void TetrisManager_StartTotalTime(TetrisManager* tetrisManager) {
	DWORD totalTimeThreadID;
	tetrisManager->isTotalTimeAvailable = True; //실행중이므로 true
	tetrisManager->totalTimeThread = (HANDLE)_beginthreadex(NULL, 0, _TetrisManager_OnTotalTimeThreadStarted, tetrisManager, 0, (unsigned *)&totalTimeThreadID);
=======
void TetrisManager_StartTotalTime(TetrisManager* tetrisManager){
	//DWORD totalTimeThreadID;
	tetrisManager->isTotalTimeAvailable = True;
	//tetrisManager->totalTimeThread = (HANDLE)_beginthreadex(NULL, 0, _TetrisManager_OnTotalTimeThreadStarted, tetrisManager, 0, (unsigned long *)&totalTimeThreadID);
>>>>>>> fix
}

void TetrisManager_PauseTotalTime(TetrisManager* tetrisManager) {
	tetrisManager->isTotalTimeAvailable = False;
	tetrisManager->totalTime--; // to show not one added time but paused time
}

void TetrisManager_StopTotalTime(TetrisManager* tetrisManager) {
	tetrisManager->isTotalTimeAvailable = False;
	tetrisManager->totalTime = 0; //totalTime이 0이므로 출력되는 시분초 0 0 0
}

static void _TetrisManager_PrintStatus(TetrisManager* tetrisManager, int x, int y) {
	ScreenUtil_ClearRectangle(x + 2, y + 1, 4, 1); // use temp size (magic number)
	ScreenUtil_ClearRectangle(x + 13, y + 1, 6, 1); // use temp size (magic number)
	ScreenUtil_ClearRectangle(x + 26, y + 1, 12, 1); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("┏ Lv ┓   ┏ Line ┓   ┏ TotalScore ┓");
	CursorUtil_GotoXY(x, y++);
	printf("┃%3d ┃   ┃%4d   ┃    ┃  %7d       ┃", tetrisManager->speedLevel, tetrisManager->deletedLineCount, tetrisManager->score);
	CursorUtil_GotoXY(x, y++);
	printf("┗━━┛   ┗━━━┛   ┗━━━━━━┛");
}

<<<<<<< HEAD
static void _TetrisManager_PrintKeys(TetrisManager* tetrisManager, int x, int y) {
=======
static void _TetrisManager_PrintKeys( int x, int y){
>>>>>>> fix
	ScreenUtil_ClearRectangle(x, y, 26, 9); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("┏━━━━ Keys ━━━━┓");
	CursorUtil_GotoXY(x, y++);
	printf("┃←       ┃move left  ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┃→       ┃move right ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┃↓       ┃move down  ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┃↑       ┃rotate     ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┃SpaceBar ┃direct down┃");
	CursorUtil_GotoXY(x, y++);
	printf("┃ESC      ┃pause      ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┃L (l)    ┃hold       ┃");
	CursorUtil_GotoXY(x, y++);
	printf("┗━━━━━━━━━━━┛");
} 

static void _TetrisManager_PrintBlock(TetrisManager* tetrisManager, int blockType, int status) {
	int i;
	Block block = _TetrisManager_GetBlockByType(tetrisManager, blockType);
	switch (blockType) {
	case MOVING_BLOCK:
		FontUtil_ChangeFontColor(tetrisManager->block.color);
		break;
	case FIXED_BLOCK:
		FontUtil_ChangeFontColor(WHITE);
		break;
	case SHADOW_BLOCK:
		FontUtil_ChangeFontColor(GRAY);
		break;
	}
	for (i = 0; i < POSITIONS_SIZE; i++) { //블럭의 네 상자의 위치로
		int x = Block_GetPositions(block)[i].x;
		int y = Block_GetPositions(block)[i].y;
		CursorUtil_GotoXY(2 * y, x); //커서 위치 이동
		printf("%s", boardTypesToPrint[status]); //status에 따라 어느 모양으로 출력할 것인지 
	}
	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);  //DEFAULT_FONT_COLOR으로 색변환
	_TetrisManager_PrintTotalTime(*tetrisManager); // because of multi thread problem, this function covers total time
}

static void _TetrisManager_InitBoard(TetrisManager* tetrisManager) {
	int i;
	memset(tetrisManager->board, EMPTY, sizeof(char)* BOARD_ROW_SIZE * BOARD_COL_SIZE); //memset함수 호출하여 tetrismanager의 board를 empty
	for (i = 0; i < BOARD_ROW_SIZE; i++) { //테트리스판 네 벽 재설정
		tetrisManager->board[i][0] = LEFT_WALL;
		tetrisManager->board[i][BOARD_COL_SIZE - 1] = RIGHT_WALL;
	}
	for (i = 0; i < BOARD_COL_SIZE; i++) {
		tetrisManager->board[0][i] = TOP_WALL;
		tetrisManager->board[BOARD_ROW_SIZE - 1][i] = BOTTOM_WALL;
	}

	//in order to make center hole at top wall, we convert center top wall into empty intentionally
	tetrisManager->board[0][(BOARD_COL_SIZE - 2) / 2 - 1] = EMPTY;
	tetrisManager->board[0][(BOARD_COL_SIZE - 2) / 2] = EMPTY;
	tetrisManager->board[0][(BOARD_COL_SIZE - 2) / 2 + 1] = EMPTY;
	tetrisManager->board[0][(BOARD_COL_SIZE - 2) / 2 + 2] = EMPTY;
	// 테트리스판의 네 모서리값 재설정
	tetrisManager->board[0][0] = LEFT_TOP_EDGE; //
	tetrisManager->board[0][BOARD_COL_SIZE - 1] = RIGHT_TOP_EDGE;
	tetrisManager->board[BOARD_ROW_SIZE - 1][0] = LEFT_BOTTOM_EDGE;
	tetrisManager->board[BOARD_ROW_SIZE - 1][BOARD_COL_SIZE - 1] = RIGHT_BOTTOM_EDGE;
}

static void _TetrisManager_UpSpeedLevel(TetrisManager* tetrisManager) { //매개변수로 tetrisManager을 받아서 speedLevel 1증가
	if (tetrisManager->speedLevel < MAX_SPEED_LEVEL) {
		tetrisManager->speedLevel++;
	}
}
static void _TetrisManager_SearchLineIndexesToDelete(TetrisManager* tetrisManager, int* indexes, int* count) {
	int i;
	int j;
	int toDelete;
	memset(indexes, -1, sizeof(int)* (BOARD_ROW_SIZE - 2));
	*count = 0;
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++) {
		toDelete = True;
		for (j = 1; j < BOARD_COL_SIZE - 1; j++) {
			if (tetrisManager->board[i][j] != FIXED_BLOCK) { //삭제할 라인의 위치를 찾음
				toDelete = False;
				break;
			}
		}
		if (toDelete) {
			indexes[(*count)++] = i; // (삭제할 라인의) 위치를 index에 저장, 그 때마다 count를 1씩 증가
		}
	}
}

static void _TetrisManager_DeleteLines(TetrisManager* tetrisManager, int* indexes, int count) {
	int i;
	int j;
	int k = BOARD_ROW_SIZE - 2;
	int toDelete;
	char temp[BOARD_ROW_SIZE][BOARD_COL_SIZE] = { EMPTY, }; //임시판 선언 (삭제된 테트리스판을 저장할)
	for (i = BOARD_ROW_SIZE - 2; i > 0; i--) {
		toDelete = False;
		for (j = 0; j < BOARD_COL_SIZE; j++) {
			if (i == indexes[j]) { //indexes에 해당하는 줄을 제외
				toDelete = True;
				break;
			}
		}
		if (!toDelete) {
			for (j = 0; j < BOARD_COL_SIZE; j++) {
				temp[k][j] = tetrisManager->board[i][j]; //나머지 줄은 temp에 저장
			}
			k--;
		}
	}
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++) {
		for (j = 1; j < BOARD_COL_SIZE - 1; j++) {
			tetrisManager->board[i][j] = temp[i][j]; //임시판을 기존의 테트리스판에 복사
		}
	}
	for (i = 0; i < count; i++) {
		tetrisManager->shadow = Block_Move(tetrisManager->shadow, DOWN); // lower shadow block by deleted count
		tetrisManager->score += tetrisManager->speedLevel * 100;
		tetrisManager->deletedLineCount++;
		if (tetrisManager->deletedLineCount % LEVELP_UP_CONDITION == 0) {
			_TetrisManager_UpSpeedLevel(tetrisManager);
		}
	}
}

<<<<<<< HEAD
static void _TetrisManager_HighlightLinesToDelete(TetrisManager* tetrisManager, int* indexes, int count) { // 라인이 삭제될때의 처리되는 부분(모양과 색변화)
=======
static void _TetrisManager_HighlightLinesToDelete( int* indexes, int count){
>>>>>>> fix
	int i;
	int j;
	int k;
	for (i = 0; i < LINES_TO_DELETE_HIGHLIGHTING_COUNT; i++) {
		FontUtil_ChangeFontColor(JADE);
		Sleep(LINES_TO_DELETE_HIGHLIGHTING_MILLISECOND);
		for (j = 0; j < count; j++) {
			CursorUtil_GotoXY(2, indexes[j]);
			for (k = 0; k < BOARD_COL_SIZE - 2; k++) {
				printf("▧");
			}
		}
		FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);
		Sleep(LINES_TO_DELETE_HIGHLIGHTING_MILLISECOND);
		for (j = 0; j < count; j++) {
			CursorUtil_GotoXY(2, indexes[j]);
			for (k = 0; k < BOARD_COL_SIZE - 2; k++) {
				printf("  "); //공백을 출력 --> LINES_TO_DELETE_HIGHLIGHTING_COUNT만큼 ▧이 깜빡이는것처럼 보이기 위함
			}
		}
	}
}

static Block _TetrisManager_GetBlockByType(TetrisManager* tetrisManager, int blockType) {
	if (blockType == SHADOW_BLOCK) {
		return tetrisManager->shadow;
	}
	else {
		return tetrisManager->block;
	}
}

static void _TetrisManager_MakeShadow(TetrisManager* tetrisManager) {
	tetrisManager->shadow = tetrisManager->block;
	while (!TetrisManager_IsReachedToBottom(tetrisManager, SHADOW_BLOCK)) {
		_TetrisManager_ChangeBoardByDirection(tetrisManager, SHADOW_BLOCK, DOWN);
	}
	_TetrisManager_PrintBlock(tetrisManager, SHADOW_BLOCK, SHADOW_BLOCK);
}

static int _TetrisManager_CheckValidPosition(TetrisManager* tetrisManager, int blockType, int direction) {
	//Block_Move를 호출하여 얻은 block 값을 임시 temp 에 저장
	Block temp = Block_Move(_TetrisManager_GetBlockByType(tetrisManager, blockType), direction);
	int i;
	for (i = 0; i < POSITIONS_SIZE; i++) { //블럭의 4개의 상자의 x,y 좌표값)
		int x = Block_GetPositions(temp)[i].x;

		//but now, x == 0 is empty
		//originally, x == 0 is top wall
		//because we convert the center top wall into empty intentionally
		if (blockType == MOVING_BLOCK && x == 0) {
			return TOP_WALL;
		}
		int y = Block_GetPositions(temp)[i].y;
		if (!(tetrisManager->board[x][y] == EMPTY || tetrisManager->board[x][y] == MOVING_BLOCK || tetrisManager->board[x][y] == SHADOW_BLOCK)) { //x,y 좌표에 해당하는 tetrisManager->board 가 EMPTY (빈 곳) 가 아니고, MOVING_BLOCK, SHADOW_BLOCK도 아닌 경우
			return tetrisManager->board[x][y]; //유효하지 않으므로 상태 반환
		}
	}
	return EMPTY; //유효하면 Empty 반환
}

static void _TetrisManager_ChangeBoardByDirection(TetrisManager* tetrisManager, int blockType, int direction) {
	// 임시 방향과 임시 유효 상태 체크 결과 변수를 선언 (회전시에 벽에 부딪혀 유효하지 않은 상태가 되었을 때도, 한 칸 밀려 나오면서 회전은 가능해야 하기 때문)
	int tempDirection = DOWN;
	int tempCheckResult = EMPTY;
	int checkResult;
	_TetrisManager_ChangeBoardByStatus(tetrisManager, blockType, EMPTY); // _TetrisManager_ChangeBoardByStatus 함수를 호출하여 테트리스 판의 상태들을 정리
	checkResult = _TetrisManager_CheckValidPosition(tetrisManager, blockType, direction); //TetrisManager_CheckValidPosition 함수를 호출하여 해당 direction 에 해당하는 블럭의 다음 위치가 유효한지를 구하여 checkResult 에 저장

	if (checkResult == EMPTY) { //그 다음 위치가 유효하다면 Block_Move 함수를 호출하여 실제 블럭과 shadow를 direction 으로 이동 혹은 회전
		if (blockType == MOVING_BLOCK) {
			tetrisManager->block = Block_Move(tetrisManager->block, direction);
		}
		else if (blockType == SHADOW_BLOCK) {
			tetrisManager->shadow = Block_Move(tetrisManager->shadow, direction);
		}
	}//
	else {
		if ((direction == UP || direction == LEFT || direction == RIGHT) && checkResult != FIXED_BLOCK) {
			if (checkResult == TOP_WALL) {
				tempDirection = DOWN;
				tempCheckResult = TOP_WALL;
			}
			else if (checkResult == RIGHT_WALL) {
				tempDirection = LEFT;
				tempCheckResult = RIGHT_WALL;
			}
			else if (checkResult == LEFT_WALL) {
				tempDirection = RIGHT;
				tempCheckResult = LEFT_WALL;
			}
			do {
				tetrisManager->block = Block_Move(tetrisManager->block, tempDirection); // Block_Move 함수에 tempDirection 을 넘기면서 block 을 벽의 반대 방향으로 밀려 나오게 
			} while (_TetrisManager_CheckValidPosition(tetrisManager, MOVING_BLOCK, direction) == tempCheckResult);
			tetrisManager->block = Block_Move(tetrisManager->block, direction);
		}
	}
	_TetrisManager_ChangeBoardByStatus(tetrisManager, blockType, blockType); //_TetrisManager_ChangeBoardByStatus 함수를 호출, tetrisManager 의 board 에서 새로 바뀐 블럭의 위치를 재설정
}

static void _TetrisManager_ChangeBoardByStatus(TetrisManager* tetrisManager, int blockType, int status) {
	int i;
	Block block = _TetrisManager_GetBlockByType(tetrisManager, blockType);
	for (i = 0; i < POSITIONS_SIZE; i++) { //block 의 4 좌표를 돌면서 입력받은 status 로 변경
		int x = Block_GetPositions(block)[i].x;
		int y = Block_GetPositions(block)[i].y;

		tetrisManager->board[x][y] = status;
	
	}
}

static DWORD WINAPI _TetrisManager_OnTotalTimeThreadStarted(void *tetrisManager) {
	while (True) {
		if (!((TetrisManager*)tetrisManager)->isTotalTimeAvailable) {
			break;
		}
		Sleep(MILLI_SECONDS_PER_SECOND);
		((TetrisManager*)tetrisManager)->totalTime++;
	}
	return 0;
}

static void _TetrisManager_PrintTotalTime(TetrisManager tetrisManager) {
	int hour = tetrisManager.totalTime / (60 * 60);
	int minute = tetrisManager.totalTime % (60 * 60) / 60;
	int second = tetrisManager.totalTime % 60;

	// use temp size (magic number) 
	int x = 42;
	int y = 20;
	CursorUtil_GotoXY(x, y++);
	printf("┏  time  ┓");
	CursorUtil_GotoXY(x, y++);
	printf("┃%02d:%02d:%02d┃", hour, minute, second);
	CursorUtil_GotoXY(x, y++);
	printf("┗━━━━┛");
}

static void _TetrisManager_MakeObstacleOneLine(TetrisManager* tetrisManager) {
	int i;
	int j;
	int isFixedBlock;
	int fixedBlockCount = 0;
	char temp[BOARD_ROW_SIZE][BOARD_COL_SIZE] = { EMPTY, };
	for (i = 1; i < BOARD_COL_SIZE - 1; i++) {
		if (tetrisManager->board[1][i] == FIXED_BLOCK) {
			return;
		}
	}
	srand((unsigned int)time(NULL));
	for (i = 1; i < BOARD_COL_SIZE - 1; i++) {
		isFixedBlock = rand() % 2;
		fixedBlockCount += isFixedBlock;
		temp[BOARD_ROW_SIZE - 2][i] = isFixedBlock ? FIXED_BLOCK : EMPTY;
	}
	if (fixedBlockCount == BOARD_COL_SIZE - 2) {
		temp[BOARD_ROW_SIZE - 2][rand() % (BOARD_COL_SIZE - 2) + 1] = EMPTY;
	}
	for (i = BOARD_ROW_SIZE - 2; i > 0; i--) {
		for (j = 1; j < BOARD_COL_SIZE - 1; j++) {
			temp[i - 1][j] = tetrisManager->board[i][j];
		}
	}
	for (i = 1; i < BOARD_ROW_SIZE - 1; i++) {
		for (j = 1; j < BOARD_COL_SIZE - 1; j++) {
			tetrisManager->board[i][j] = temp[i][j];
		}
	}
	_TetrisManager_MakeShadow(tetrisManager);
	TetrisManager_PrintBoard(tetrisManager);
}

