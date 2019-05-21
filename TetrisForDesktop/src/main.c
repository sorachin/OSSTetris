#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "Constant.h"
#include "TetrisView.h"

int main(int argc, char* argv[]) {
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
	do {
		TetrisView_ProcessMainMenu(&tetrisView);//������ �����ϸ� ���θ޴����� �����
		switch (tetrisView.mainMenu) {//���õ� �޴��� ���� ��� ����
		case START_MAIN_MENU: //������ ����������
			TetrisView_StartGame(&tetrisView);
			tickCount = GetTickCount();
			while (True) {
				processType = AUTO;
				direction = DOWN; //����Ű�� �Է����� ������ ����� �ڵ����� �Ʒ��� �̵�
				isSetMainMenu = False;
				while (True) {
					if (GetTickCount() - tickCount > TetrisView_GetDownMilliSecond(&tetrisView)) {//����Ű �Է��� ��� �ð��� ������ �ڵ����� ���� �����;��ϴ°��
						processType = AUTO;
						direction = DOWN;
						tickCount = GetTickCount();
						break;
					}//if
					if (_kbhit()) {//Ű���忡�� �Է��� �������
						int key = _getch();
						if (key == 0xE0 && _kbhit()) {//ȭ��ǥŰ�� �Է�������
							key = _getch();
							if (key == UP_KEY_CODE) {
								processType = DIRECTION;
								direction = UP;
								break;
							}
							if (key == DOWN_KEY_CODE) {
								processType = DIRECTION;
								direction = DOWN;
								tickCount = GetTickCount();
								break;
							}
							if (key == LEFT_KEY_CODE) {
								processType = DIRECTION;
								direction = LEFT;
								break;
							}
							if (key == RIGHT_KEY_CODE) {
								processType = DIRECTION;
								direction = RIGHT;
								break;
							}
						}
						else {//ȭ��ǥŰ�� �ƴ� �ٸ� Ű�� �Է�������
							if (key == SPACE_BAR_KEY_CODE) {
								processType = DIRECT_DOWN; //����� �� ���� ���� �Ʒ����� �����´�.
								tickCount = GetTickCount();
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
							}
						}
					}
				}

				//for selecting main menu at pause menu
				if (isSetMainMenu) {
					break;
				}
				TetrisView_ProcessGame(&tetrisView, processType, direction);//��������ȭ�� ���
				/*������ ����� �� ������ endMenu������ ����*/
				if (tetrisView.endMenu == RANKING_END_MENU) {
					TetrisView_AddRanking(&tetrisView);
					TetrisView_ShowRanking(&tetrisView);
					break;
				}
				else if (tetrisView.endMenu == MAIN_MENU_END_MENU) {
					break;
				}
				else if (tetrisView.endMenu == EXIT_END_MENU) {
					tetrisView.mainMenu = EXIT_MAIN_MENU;//������ ����
					break;
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