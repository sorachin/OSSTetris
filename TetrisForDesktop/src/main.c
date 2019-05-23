#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "Constant.h"
#include "TetrisView.h"

int main(void) {
	/*****���� ����*****
	TetrisView : ȭ����°� ������ ����Ǵµ� �ʿ��� ����
	processType : ���� �ڵ����� ����������, ����Ű �Է¿����� �����̴����� �����ϱ����� ���
	direction : ���� �̵��ϴ� ������ ����
	tickCount : ���������߿� tickCount���̸� ����Ͽ� ���� �ܰ躰�� �����س��� �ð��� ������ �ڵ����� ����������
	isSetMainMenu : ���θ޴������� ��ȯ
	********************/
	TetrisView tetrisView;
	int processType;
	int direction;
	DWORD tickCount;
	int isSetMainMenu = False;
	int isGaming = False;
	int isOverTime = True;
	int key = -1;
	do {
		TetrisView_ProcessMainMenu(&tetrisView);//������ �����ϸ� ���θ޴����� �����
		switch (tetrisView.mainMenu) {//���õ� �޴��� ���� ��� ����
		case START_MAIN_MENU: //������ ����������
			isGaming = True;

			TetrisView_StartGame(&tetrisView);
			tickCount = GetTickCount();
			while (isGaming == True) {
				processType = AUTO;
				direction = DOWN; //����Ű�� �Է����� ������ ����� �ڵ����� �Ʒ��� �̵�
				isSetMainMenu = False;
				isOverTime = True;

					while (GetTickCount() - tickCount < TetrisView_GetDownMilliSecond(&tetrisView)){
						if (_kbhit()) {//Ű���忡�� �Է��� �������
							key = _getch();
							if (key == 0xE0 && _kbhit()) {//ȭ��ǥŰ�� �Է�������
								key = _getch();
								if (key == UP_KEY_CODE) {
									processType = DIRECTION;
									direction = UP;
									isOverTime = False;
									break;
								}
								if (key == DOWN_KEY_CODE) {
									processType = DIRECTION;
									direction = DOWN;
									tickCount = GetTickCount();
									isOverTime = False;
									break;
								}
								if (key == LEFT_KEY_CODE) {
									processType = DIRECTION;
									direction = LEFT;
									isOverTime = False;
									break;
								}
								if (key == RIGHT_KEY_CODE) {
									processType = DIRECTION;
									direction = RIGHT;
									isOverTime = False;
									break;
								}
							}
							else {//ȭ��ǥŰ�� �ƴ� �ٸ� Ű�� �Է�������
								if (key == SPACE_BAR_KEY_CODE) {
									processType = DIRECT_DOWN; //����� �� ���� ���� �Ʒ����� �����´�.
									tickCount = GetTickCount();
									isOverTime = False;
									break;
								}
								if (key == ESC_KEY_CODE) {
									TetrisView_PauseGame(&tetrisView);
									if (tetrisView.pauseMenu == MAIN_MENU_PAUSE_MENU) {//������ �Ͻ������� �� ���θ޴��� �̵��� ���
										isSetMainMenu = True;
										break;
									}
									processType = AUTO; //RESUME�� �����Ͽ� ������ �̾�ϴ� ���
									tickCount = GetTickCount();
									break;
								}
								if (key == L_KEY_CODE) {//L == HoldŰ
									TetrisView_MakeHold(&tetrisView);
								}//if
							}//if-else
						}//if
					}//while
					if (isOverTime == True) {
						processType = AUTO;
						direction = DOWN;
						tickCount = GetTickCount();
					}
				//for selecting main menu at pause menu
				if (isSetMainMenu) {
					isGaming = False;
				}
				TetrisView_ProcessGame(&tetrisView, processType, direction);//��������ȭ�� ���
				/*������ ����� �� ������ endMenu������ ����*/
				if (tetrisView.endMenu == RANKING_END_MENU) {
					isGaming = False;

					TetrisView_AddRanking(&tetrisView);
					TetrisView_ShowRanking(&tetrisView);
				}
				else if (tetrisView.endMenu == MAIN_MENU_END_MENU) {
					isGaming = False;
				}
				else if (tetrisView.endMenu == EXIT_END_MENU) {
					isGaming = False;

					tetrisView.mainMenu = EXIT_MAIN_MENU;//������ ����
				}
			}
			break;
		case RANKING_MAIN_MENU:
			TetrisView_ShowRanking(&tetrisView);
			break;
		case SETTING_MAIN_MENU:
			TetrisView_ShowSetting(&tetrisView);
			break;
		}
	} while (tetrisView.mainMenu != EXIT_MAIN_MENU);
	return 0;
}