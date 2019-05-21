#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "Constant.h"
#include "TetrisView.h"

int main(int argc, char* argv[]) {
	/*****변수 설명*****
	TetrisView : 화면출력과 게임이 진행되는데 필요한 변수
	processType : 블럭이 자동으로 떨어지는지, 방향키 입력에의해 움직이는지를 구분하기위해 사용
	direction : 블럭이 이동하는 방향을 저장
	tickCount : 게임진행중에 tickCount차이를 계산하여 블럭이 단계별로 지정해놓은 시간이 지나면 자동으로 내려오게함
	isSetMainMenu : 메인메뉴인지를 반환
	********************/
	TetrisView tetrisView;
	int processType;
	int direction;
	DWORD tickCount;
	int isSetMainMenu = False;
	do {
		TetrisView_ProcessMainMenu(&tetrisView);//게임을 실행하면 메인메뉴부터 실행됨
		switch (tetrisView.mainMenu) {//선택된 메뉴에 따라 기능 실행
		case START_MAIN_MENU: //게임을 실행했을때
			TetrisView_StartGame(&tetrisView);
			tickCount = GetTickCount();
			while (True) {
				processType = AUTO;
				direction = DOWN; //방향키를 입력하지 않으면 블록은 자동으로 아래로 이동
				isSetMainMenu = False;
				while (True) {
					if (GetTickCount() - tickCount > TetrisView_GetDownMilliSecond(&tetrisView)) {//방향키 입력이 없어도 시간이 지나서 자동으로 블럭이 내려와야하는경우
						processType = AUTO;
						direction = DOWN;
						tickCount = GetTickCount();
						break;
					}//if
					if (_kbhit()) {//키보드에서 입력이 있을경우
						int key = _getch();
						if (key == 0xE0 && _kbhit()) {//화살표키를 입력했을때
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
						else {//화살표키를 아닌 다른 키를 입력했을때
							if (key == SPACE_BAR_KEY_CODE) {
								processType = DIRECT_DOWN; //블록이 한 번에 제일 아래까지 내려온다.
								tickCount = GetTickCount();
								break;
							}
							if (key == ESC_KEY_CODE) {
								TetrisView_PauseGame(&tetrisView);
								if (tetrisView.pauseMenu == MAIN_MENU_PAUSE_MENU) {//게임을 일시정지한 후 메인메뉴로 이동한 경우
									isSetMainMenu = True;
									break;
								}
								processType = AUTO; //RESUME을 선택하여 게임을 이어서하는 경우
								tickCount = GetTickCount();
								break;
							}
							if (key == L_KEY_CODE) {//L == Hold키
								TetrisView_MakeHold(&tetrisView);
							}
						}
					}
				}

				//for selecting main menu at pause menu
				if (isSetMainMenu) {
					break;
				}
				TetrisView_ProcessGame(&tetrisView, processType, direction);//게임진행화면 출력
				/*게임이 종료된 뒤 선택한 endMenu에따라 진행*/
				if (tetrisView.endMenu == RANKING_END_MENU) {
					TetrisView_AddRanking(&tetrisView);
					TetrisView_ShowRanking(&tetrisView);
					break;
				}
				else if (tetrisView.endMenu == MAIN_MENU_END_MENU) {
					break;
				}
				else if (tetrisView.endMenu == EXIT_END_MENU) {
					tetrisView.mainMenu = EXIT_MAIN_MENU;//게임을 종료
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