#ifndef _TETRIS_VIEW_H
#define _TETRIS_VIEW_H

#include "TetrisManager.h"
#include "RankingManager.h"

typedef struct _tetrisView {
	/****구조체 변수 설명****
		TetrisManager: 테트리스 기능을 구현하기 위해 사용
		RankingManager : ranking기능 구현을 위해 사용
		mainMenu, pauseMenu, endMenu : 각각의 메뉴에서 사용자가 선택한 메뉴번호를 저장. 각 기능을 사용하지 않을 경우에는 0으로 설정
	************************/
	TetrisManager tetrisManager;
	RankingManager rankingManager;
	int level;
	int mainMenu;
	int pauseMenu;
	int endMenu;
}TetrisView; //

/*****함수 설명*****
	TetrisView_StartGame : 게임이 시작될 때 화면을 설정 ,배경음악 재생, 초기화
	TestrisView_ProcessGame : 게임이 진행될 때 게임화면 출력
	TestrisView_PauseGame : 일시정지했을 때 화면, 배경음악 설정
	TestrisView_EndGame : 게임이 끝났을때 기본 설정
	TestrisView_ShowSetting : Rank를 보여주기위한 과정
	TestrisView_AddRanking : 게임이 종료된 뒤 현재 rank입력, 전체 ranking출력
	TestrisView_ShowSetting : 설정메뉴를 출력하고, 레벨의 범위 설정
	TestrisView_ProcessMainMenu : 메인메뉴를 출력하고 사용자가 메뉴를 선택하는 기능
	TestrisView_ProcessPauseMenu : 게임을 일시정지했을 때 일시정지 메뉴 화면 출력, 메뉴를 선택하는 기능
	TestrisView_ProcessEndMenu : 게임이 종료되었을 때 화면출력, 메뉴 선택 기능
*******************/
void TetrisView_StartGame(TetrisView* tetrisView);
void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction);
void TetrisView_PauseGame(TetrisView* tetrisView);
void TetrisView_EndGame(TetrisView* tetrisView);
void TetrisView_ShowRanking(TetrisView* tetrisView);
void TetrisView_AddRanking(TetrisView* tetrisView);
void TetrisView_ShowSetting(TetrisView* tetrisView);
void TetrisView_ProcessMainMenu(TetrisView* tetrisView);
void TetrisView_PrintPauseMenu(TetrisView* tetrisView);
void TetrisView_ProcessEndMenu(TetrisView* tetrisView);

//make temporarily for calling from main function
/*****함수 기능*****
TetrisView_GetDownMilliSecond : 블럭이 내려오는 속도(milliSecond)반환
TetrisView_MakeHold : Hold기능을 구현한 함수 호출
********************/
DWORD TetrisView_GetDownMilliSecond(TetrisView* tetrisView);
void TetrisView_MakeHold(TetrisView* tetrisView);

#endif	//Header를 중복하여 include하지 않기 위해