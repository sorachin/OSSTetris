//Header
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "TetrisView.h"
#include "TetrisManager.h"
#include "Constant.h"
#include "Util.h"
#include "Menu.h"

#pragma comment(lib, "winmm.lib")//PlaySound()�� ����ϱ� ���ؼ� winmm.lib�� �о�´�.

#define TETRIS_BACKGROUND_MUSIC_FILE_NAME "..\\res\\tetris_background_music.wav"

/********Constant********
PROCESS_REACHED_CASE_COUNT : �ٴڿ� ���� ����� �� ���� �����ʰ� �����Ǵ� �ð��� ����
ADD_ID_POSITION_X_TO_PRINT, ADD_ID_POSITION_X_TO_PRINT : ���� ���� �� Rank�� �߰��ϴ� ȭ�鿡�� ó������ Ŀ���� ��ġ�ϴ� x,y��ǥ
SETTING_POSITION_X_TO_PRINT,SETTING_POSITION_Y_TO_PRINT : �����޴����� ó������ Ŀ���� ��ġ�ϴ� x,y��ǥ
*************************/
#define PROCESS_REACHED_CASE_COUNT 2
#define ADD_ID_POSITION_X_TO_PRINT 25
#define ADD_ID_POSITION_Y_TO_PRINT 10
#define SETTING_POSITION_X_TO_PRINT 30
#define SETTING_POSITION_Y_TO_PRINT 10

enum MenuStartPosition {
	//���� : �� �޴����� ó������ Ŀ���� ��ġ�Ǵ� X,Y��ǥ ��ġ
	MAIN_MENU_X = 18, MAIN_MENU_Y = 7, PAUSE_MENU_X = 5, PAUSE_MENU_Y = 12, END_MENU_X = 14, END_MENU_Y = 8
};

void TetrisView_StartGame(TetrisView* tetrisView) {
	PlaySound(TEXT(TETRIS_BACKGROUND_MUSIC_FILE_NAME), NULL, SND_ASYNC | SND_LOOP);//��������� �ݺ����
	if (!(tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL)) { //������ �����ϱ� ���� ������ ������ ������ �Է��� �� �ִµ�, �̶��� ���� ������ ��ȿ����(1~10)�� �˻�
		tetrisView->level = MIN_SPEED_LEVEL; //������ ������ ��ȿ���� ������ ���� ��������(1)�� ����
	}//if
	TetrisManager_Init(&tetrisView->tetrisManager, tetrisView->level);//���� tetrisView->tetrisManager �ʱ�ȭ

	/*ȭ���� ���� �� ����ȭ�鿡 ��µǴ� font, block���� ���� �����ϰ�, ��ġ�� ���缭 ȭ���� ���*/
	system("cls");
	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);
	TetrisManager_PrintBoard(&tetrisView->tetrisManager);
	TetrisManager_PrintDetailInfomation(&tetrisView->tetrisManager);
}

void TetrisView_ProcessGame(TetrisView* tetrisView, int processType, int direction) {

	//it is used to move left or right at bottom in case of space which you want to move is available
	static int processReachedCaseCount = 0;
	if (processType == DIRECTION) {//����Ű�� �������
		TetrisManager_ProcessDirection(&tetrisView->tetrisManager, direction);
	}
	else if (processType == DIRECT_DOWN) {//�����̽��ٸ� ���� ���
		TetrisManager_ProcessDirectDown(&tetrisView->tetrisManager);
	}
	else if (processType == AUTO) {//�ڵ����� �������� ���
		TetrisManager_ProcessAuto(&tetrisView->tetrisManager);
	}
	if (TetrisManager_IsReachedToBottom(&tetrisView->tetrisManager, MOVING_BLOCK)) {//���� �̵����� ���̹ٴڿ� ����� ��
		if (processType == DIRECT_DOWN) {//�����̽��ٸ� ���� ���
			processReachedCaseCount = 0;
			if (TetrisManager_ProcessReachedCase(&tetrisView->tetrisManager) == END) {
				TetrisView_EndGame(tetrisView);
				return;
			}
		}
		else {//�����̽��ٸ� ������ �ʰ� ���� �ٴڿ� �������� ��

			/*processReachedCaseCount�� PROCESS_REACHED_CASE_COUNT��ŭ �����Ҷ������� �ٴڿ��� �ٷ� �����ʰ�(else) �� ���� ����
			  processReachedCaseCount�� PROCESS_REACHED_CASE_COUNT�� ���� ��������(if) ���� ����*/
			if (processReachedCaseCount == PROCESS_REACHED_CASE_COUNT) {
				if (TetrisManager_ProcessReachedCase(&tetrisView->tetrisManager) == END) {//������ �������
					TetrisView_EndGame(tetrisView);
					return;
				}

				processReachedCaseCount = 0;//���� ���� ���� processReachedCaseCount �ʱ�ȭ
			}
			else {
				processReachedCaseCount++;
			}
		}
	}
	TetrisManager_ProcessDeletingLines(&tetrisView->tetrisManager);//������ ������ ã�Ƽ� ��Ʈ������ ����
}

void TetrisView_PauseGame(TetrisView* tetrisView) {
	/*����ڰ� �Ͻ����������� ����ȭ�� ����*/
	TetrisManager_PauseTotalTime(&tetrisView->tetrisManager);
	PlaySound(NULL, 0, 0);//�Ͻ������ϸ� ��������� �Ͻ�������
	FontUtil_ChangeFontColor(LIGHT_YELLOW);//Pause������ �������� �޴��� ��Ʈ ����
	TetrisView_ProcessPauseMenu(tetrisView);
	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);

	switch (tetrisView->pauseMenu) {//����ڰ� ������ �Ͻ����� �޴�
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
	TetrisManager_PauseTotalTime(&tetrisView->tetrisManager);//Rank�� ����� ����� �� �����Ƿ� �ð��� �ʱ�ȭ���� �ʰ� �Ͻ�������
	PlaySound(NULL, 0, 0);
	TetrisView_ProcessEndMenu(tetrisView);//������ �� ȭ�� ���
}

void TetrisView_ShowRanking(TetrisView* tetrisView) {
	/*Rank�� �����ֱ����� ����
	Create�� rank���� ���� ���� ���� -> ���Ͽ��� rank������ load�Ͽ� ���� -> ��� -> ��� �� �޸� ������ Ȯ���ؾ��ϹǷ� ���� ���� ���� Destroy*/

	RankingManager_Create(&tetrisView->rankingManager);
	RankingManager_Load(&tetrisView->rankingManager);
	RankingManager_Print(&tetrisView->rankingManager);
	RankingManager_Destroy(&tetrisView->rankingManager);
}

void TetrisView_AddRanking(TetrisView* tetrisView) {
	Ranking ranking;
	int x = ADD_ID_POSITION_X_TO_PRINT;
	int y = ADD_ID_POSITION_Y_TO_PRINT;//x,y : Ŀ���� ��ġ
	int i;
	char id[ID_SIZE + 1] = { '\0', };

	/*ȭ���� �ʱ�ȭ�� �� Rank�� ����� ���̵� �Է��ϴ� ȭ�� ���*/
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("������������������������������");
	CursorUtil_GotoXY(x, y++);
	printf("��Input ID (%d chars limit!)��", ID_SIZE);
	CursorUtil_GotoXY(x, y++);
	printf("��:                         ��");
	CursorUtil_GotoXY(x, y++);
	printf("������������������������������");
	x += 4;
	y -= 2;
	CursorUtil_GotoXY(x, y++);
	CursorUtil_Show();//���̵� �Էµ� ��ġ�� Ŀ���� ������
	fgets(id, ID_SIZE + 1, stdin);//���̵� �Է¹���
	CursorUtil_Hide();//Ŀ���� �������ʰ� ��
	for (i = ID_SIZE; i >= 0; i--) {//�Է��� ���̵� �迭�� ����
		if (id[i] == '\n') {//���̵� �Է��� �� �ִ� ���̵� ���̺��� ª�� ���
			id[i] = '\0';
			break;
		}//if
	}//for

	/*id���̸� �ʰ��� �κ� ����*/
	if (i == -1) {
		while (getchar() != '\n');
	}

	/*���� Rank�� ranking������ ����*/
	strcpy(ranking.id, id);
	ranking.score = tetrisView->tetrisManager.score;
	ranking.level = tetrisView->tetrisManager.speedLevel;
	ranking.deletedLineCount = tetrisView->tetrisManager.deletedLineCount;
	ranking.totalTime = tetrisView->tetrisManager.totalTime;
	ranking.timestamp = time(NULL);

	/*��ü Ranking�� ������*/
	RankingManager_Create(&tetrisView->rankingManager);
	RankingManager_Load(&tetrisView->rankingManager);
	RankingManager_Add(&tetrisView->rankingManager, ranking);
	RankingManager_Save(&tetrisView->rankingManager);
	RankingManager_Destroy(&tetrisView->rankingManager);
}

void TetrisView_ShowSetting(TetrisView* tetrisView) {
	int x = SETTING_POSITION_X_TO_PRINT;
	int y = SETTING_POSITION_Y_TO_PRINT;//x,y : Ŀ���� ��ġ
	if (!(tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL)) {
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	system("cls");
	CursorUtil_GotoXY(x, y++);
	printf("����������������������");
	CursorUtil_GotoXY(x, y++);
	printf("��Current Level : %2d��", tetrisView->level);
	CursorUtil_GotoXY(x, y++);
	printf("��New Level (%d ~ %d)��", MIN_SPEED_LEVEL, MAX_SPEED_LEVEL);
	CursorUtil_GotoXY(x, y++);
	printf("��:                 ��");
	CursorUtil_GotoXY(x, y++);
	printf("����������������������");
	x += 4;
	y -= 2;
	CursorUtil_GotoXY(x, y++);
	CursorUtil_Show();
	scanf("%d", &tetrisView->level);
	CursorUtil_Hide();
	if (tetrisView->level >= MIN_SPEED_LEVEL && tetrisView->level <= MAX_SPEED_LEVEL) {//���� ������ ��ȿ�� ���

	}
	else if (tetrisView->level < MIN_SPEED_LEVEL) {//�Է��� ������ ������ �ּҷ���(1)���� ���� ���
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	else if (tetrisView->level > MAX_SPEED_LEVEL) {//�Է��� ������ ������ �ִ뷹��(10)���� ū ���
		tetrisView->level = MAX_SPEED_LEVEL;
	}
	else {
		tetrisView->level = MIN_SPEED_LEVEL;
	}
	while (getchar() != '\n');//���ۿ� �����ִ� ���� ���
}

void TetrisView_ProcessMainMenu(TetrisView* tetrisView) {
	/****���� ����****
	items = [������ �޴��� �ִ� ����][������ �޴��� ���� ����]�� �迭, �޴� ��� ����
	menuCount : �޴� ����
	x,y : Ŀ���� ��ġ
	******************/
	Menu menu = { 0, };
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE] = {
		"[1] S T A R T", "[2] R A N K I N G", "[3] S E T T I N G", "[4] E X I T"
	};
	int menuCount = 4;
	int x = MAIN_MENU_X;
	int y = MAIN_MENU_Y;
	WindowUtil_ChangeWindowSize(WINDOW_LINE_SIZE, WINDOW_COL_SIZE);//���ȭ��ũ�� ����

	//���θ޴� ȭ�� ���
	system("cls");
	FontUtil_ChangeFontColor(RED);
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_RED);
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("����");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_YELLOW);
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(GREEN);
	CursorUtil_GotoXY(x, y++);
	printf("���");
	CursorUtil_GotoXY(x, y++);
	printf("�� ��");
	CursorUtil_GotoXY(x, y++);
	printf("���");
	CursorUtil_GotoXY(x, y++);
	printf("�� ��");
	CursorUtil_GotoXY(x, y++);
	printf("��  ��");
	x += 8;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_BLUE);
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	x += 4;
	y -= 5;
	FontUtil_ChangeFontColor(LIGHT_PURPLE);
	CursorUtil_GotoXY(x, y++);
	printf(" ���");
	CursorUtil_GotoXY(x, y++);
	printf("��");
	CursorUtil_GotoXY(x, y++);
	printf("����");
	CursorUtil_GotoXY(x, y++);
	printf("    ��");
	CursorUtil_GotoXY(x, y++);
	printf(" ���");
	x -= 22;
	y += 2;

	/*���θ޴��� �޴��κ��� ���, ����ڰ� �޴� ����*/
	Menu_Create(&menu, items, menuCount, x, y, DEFAULT_FONT_COLOR);
	Menu_Print(&menu);
	tetrisView->mainMenu = Menu_ProcessKey(&menu) + 1;

	/*������ pause�ϰų� end�� ���� �����Ƿ� 0���� ����*/
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

	/*Pause�� ������ �޴�ȭ�� ���, �޴����ñ��*/
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
	printf("����  ��  ��  ����          ����  ��  ��  ���");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ��  ��  ��              ��       �� ��  �� ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ����  ����  ����  ����  ����  ��  ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ��  ��  ��              ��      �� ��   �� ��");
	CursorUtil_GotoXY(x, y++);
	printf("  ��    ��  ��  ����          ����  ��  ��  ���");
	x += 19;
	y += 2;
	/*������ �������� �޴�ȭ�� ���, �޴����ñ��*/
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