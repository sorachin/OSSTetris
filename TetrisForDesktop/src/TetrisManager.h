#ifndef _BOARD_H
#define _BOARD_H

#include <windows.h>
#include "Block.h"
/* GameStatus 열거형 상수를 정의, 게임 상태에 대한 내용들
   PLAYING (게임을 끝내지 않고, 계속 진행함), END (게임 종료)*/
enum GameStatus {
	PLAYING, END
};
//테트리스판의 가로 행의 개수와, 세로 열의 개수를 매크로 상수로 정의
#define BOARD_ROW_SIZE 24 
#define BOARD_COL_SIZE 14


typedef struct _tetrisManager {
	/*
	TetrisManager 구조체를 정의
	board[BOARD_ROW_SIZE][BOARD_COL_SIZE] = 2 차원 배열인 board (테트리스 판)
	block = 블록(현재, 다음 블럭)에 대한 처리
	isHoldAvailable = if hold block is empty or hold turn is passed, this variable is true
	deletedLineCount 삭제한 줄 수
	speedLevel = 블럭이 떨어지는 속도 레벨 (사용자 입력값)
	isTotalTimeAvailable = if totalTimeThread is alive, this variable is true
	*/
	char board[BOARD_ROW_SIZE][BOARD_COL_SIZE];
	Block block;
	int isHoldAvailable;
	Block shadow;
	int deletedLineCount;
	int speedLevel;
	int score;
	HANDLE totalTimeThread;
	long totalTime;
	int isTotalTimeAvailable;
}TetrisManager;

/*
**TetrisManager.c 에서 정의될 함수의 선언부**
TetrisManager_Init = tetrisManager의 멤버 변수들을 초기화
TetrisManager_ProcessDirection = 이동중인 블럭 방향설정
TetrisManager_ProcessAuto = 자동으로 블럭이 내려가게함
TetrisManager_ProcessDirectDown = 이동중인 블럭을 바로 밑으로 이동
TetrisManager_ProcessDeletingLines =  삭제할 라인을 찾아서 삭제하고 테트리스 판을 갱신
TetrisManager_IsReachedToBottom = 현재 이동중인 블럭이 밑으로 이동할 수 있는지 체크, 가능할 때 true으로 return
TetrisManager_ProcessReachedCase = 블럭이 밑에 닿았을 때 현재블럭을 fix되게 하고, 새 블럭을 생성하는 처리
TetrisManager_PrintBoard = 테트리스판 출력함수
TetrisManager_PrintDetailInfomation = 조작법,상태 등 출력 위치 지정
TetrisManager_GetDownMilliSecond = 속도 레벨에 해당하는 만큼 프로그램을 지연
TetrisManager_MakeHold = Hold (실행중지)
TetrisManager_StartTotalTime = 실행중일때  시간 처리
TetrisManager_PauseTotalTime = 일시정지 되었을때 시간 처리
TetrisManager_StopTotalTime = 정지되었을때 시간 처리
*/
void TetrisManager_Init(TetrisManager* tetrisManager, int speedLevel);
void TetrisManager_ProcessDirection(TetrisManager* tetrisManager, int direction);
void TetrisManager_ProcessAuto(TetrisManager* tetrisManager);
void TetrisManager_ProcessDirectDown(TetrisManager* tetrisManager);
void TetrisManager_ProcessDeletingLines(TetrisManager* tetrisManager);
int TetrisManager_IsReachedToBottom(TetrisManager* tetrisManager, int blockType);
int TetrisManager_ProcessReachedCase(TetrisManager* tetrisManager);
//void TetrisManager_Print(TetrisManager* tetrisManager); 
void TetrisManager_PrintBoard(TetrisManager* tetrisManager);
void TetrisManager_PrintDetailInfomation(TetrisManager* tetrisManager);
DWORD TetrisManager_GetDownMilliSecond(TetrisManager* tetrisManager);
void TetrisManager_MakeHold(TetrisManager* tetrisManager);
void TetrisManager_StartTotalTime(TetrisManager* tetrisManager);
void TetrisManager_PauseTotalTime(TetrisManager* tetrisManager);
void TetrisManager_StopTotalTime(TetrisManager* tetrisManager);

#endif