#ifndef _TETRIS_VIEW_H
#define _TETRIS_VIEW_H

#include "TetrisManager.h"
#include "RankingManager.h"

typedef struct _tetrisView {
	/****����ü ���� ����****
		TetrisManager: ��Ʈ���� ����� �����ϱ� ���� ���
		RankingManager : ranking��� ������ ���� ���
		mainMenu, pauseMenu, endMenu : ������ �޴����� ����ڰ� ������ �޴���ȣ�� ����. �� ����� ������� ���� ��쿡�� 0���� ����
	************************/
	TetrisManager tetrisManager;
	RankingManager rankingManager;
	int level;
	int mainMenu;
	int pauseMenu;
	int endMenu;
}TetrisView; //

/*****�Լ� ����*****
	TetrisView_StartGame : ������ ���۵� �� ȭ���� ���� ,������� ���, �ʱ�ȭ
	TestrisView_ProcessGame : ������ ����� �� ����ȭ�� ���
	TestrisView_PauseGame : �Ͻ��������� �� ȭ��, ������� ����
	TestrisView_EndGame : ������ �������� �⺻ ����
	TestrisView_ShowSetting : Rank�� �����ֱ����� ����
	TestrisView_AddRanking : ������ ����� �� ���� rank�Է�, ��ü ranking���
	TestrisView_ShowSetting : �����޴��� ����ϰ�, ������ ���� ����
	TestrisView_ProcessMainMenu : ���θ޴��� ����ϰ� ����ڰ� �޴��� �����ϴ� ���
	TestrisView_ProcessPauseMenu : ������ �Ͻ��������� �� �Ͻ����� �޴� ȭ�� ���, �޴��� �����ϴ� ���
	TestrisView_ProcessEndMenu : ������ ����Ǿ��� �� ȭ�����, �޴� ���� ���
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
/*****�Լ� ���*****
TetrisView_GetDownMilliSecond : ���� �������� �ӵ�(milliSecond)��ȯ
TetrisView_MakeHold : Hold����� ������ �Լ� ȣ��
********************/
DWORD TetrisView_GetDownMilliSecond(TetrisView* tetrisView);
void TetrisView_MakeHold(TetrisView* tetrisView);

#endif	//Header�� �ߺ��Ͽ� include���� �ʱ� ����