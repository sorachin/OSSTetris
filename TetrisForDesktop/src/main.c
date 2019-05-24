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
	key : �Է¹��� Ű������ ������ ����
	isSetMainMenu : ���θ޴������� ����
	isGaming : ������ ���������� ����
	isOverTime : ���� �� ĭ �������� �ð��� ���������� ����
	********************/
	TetrisView tetrisView;
	int processType;
	int direction;
	DWORD tickCount;
	int key = -1;
	int isSetMainMenu = False;
	int isGaming = False;
	int isOverTime = True;

	do {
		TetrisView_PrintMainMenu(&tetrisView);//������ �����ϸ� ���θ޴����� �����

		switch (tetrisView.mainMenu) {//���õ� �޴��� ���� ��� ����
	
		case START_MAIN_MENU: //������ ����������
			isGaming = True;
			TetrisView_StartGame(&tetrisView);
			tickCount = GetTickCount();

			while (isGaming == True) {
				isSetMainMenu = False;
				isOverTime = True;

				processType = AUTO;
				direction = DOWN; //����Ű�� �Է����� ������ ����� �ڵ����� �Ʒ��� �̵�
				
					while (GetTickCount() - tickCount < TetrisView_GetDownMilliSecond(&tetrisView)){
						if (_kbhit()) {//Ű���忡�� �Է��� �������
							key = _getch();
							if (key == 0xE0 && _kbhit()) {//ȭ��ǥŰ�� �Է�������
								key = _getch();
								
								if (key == UP_KEY_CODE) {
									isOverTime = False;
									processType = DIRECTION;
									direction = UP;
									break;
								}
								else if (key == DOWN_KEY_CODE) {
									isOverTime = False;
									processType = DIRECTION;
									direction = DOWN;
									tickCount = GetTickCount();
									break;
								}
								else if (key == LEFT_KEY_CODE) {
									isOverTime = False;
									processType = DIRECTION;
									direction = LEFT;
									break;
								}
								else if (key == RIGHT_KEY_CODE) {
									isOverTime = False;
									processType = DIRECTION;
									direction = RIGHT;
									break;
								}
								else {
									exit(1);
								}
							}//if
							else {//ȭ��ǥŰ�� �ƴ� �ٸ� Ű�� �Է�������
								if (key == SPACE_BAR_KEY_CODE) {
									isOverTime = False;
									processType = DIRECT_DOWN; //����� �� ���� ���� �Ʒ����� �����´�.
									tickCount = GetTickCount();
									break;
								}
								else if (key == ESC_KEY_CODE) {
									TetrisView_PauseGame(&tetrisView);

									if (tetrisView.pauseMenu == MAIN_MENU_PAUSE_MENU) {//������ �Ͻ������� �� ���θ޴��� �̵��� ���
										isSetMainMenu = True;
										isGaming = False;
										tetrisView.endMenu = MAIN_MENU_END_MENU;
										break;
									}
									else {
										processType = AUTO; //RESUME�� �����Ͽ� ������ �̾�ϴ� ���
										tickCount = GetTickCount();
										break;
									}
								}
								else if (key == L_KEY_CODE) {//L == HoldŰ
									TetrisView_MakeHold(&tetrisView);
								}
							}
						}//if
					}//while
					if (isOverTime == True) {//���� �ڵ����� �� ĭ �����;��ϴ� ���
						processType = AUTO;
						direction = DOWN;
						tickCount = GetTickCount();
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
					else {
						if (isGaming == False)
							exit(1); //tetrisView.endMenu�� ���� ��� ���ǿ��� �ش������ʴµ� ������ �������� �� �����Ƿ� ������ �߻��� ��.
					}
				}
				break;//START_MAIN_MENU

			case RANKING_MAIN_MENU:
				TetrisView_ShowRanking(&tetrisView);
				break;

			case SETTING_MAIN_MENU:
				TetrisView_ShowSetting(&tetrisView);
				break;

			default: break;
			}//switch
		} while (tetrisView.mainMenu != EXIT_MAIN_MENU);

	return 0;
}