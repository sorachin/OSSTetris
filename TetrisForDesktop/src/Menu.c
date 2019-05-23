#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "Menu.h"
#include "Util.h"
#include "Constant.h"

void Menu_Create(Menu* menu, char(*items)[MENU_ITEMS_CONTENT_SIZE], int count, int startPositionXToPrint, int startPositionYToPrint, int menuColor) {
	int i;

	for (i = 0; i < count; i++) {
		strcpy(menu->items[i], items[i]);
	}

	menu->count = count;
	menu->selectedIndex = 0;
	menu->startPositionToPrint.x = startPositionXToPrint;
	menu->startPositionToPrint.y = startPositionYToPrint;
	menu->menuColor = menuColor;

	return ;
}

void Menu_Print(Menu* menu) {
	int i;
	int startPositionXToPrint = menu->startPositionToPrint.x;
	int startPositionYToPrint = menu->startPositionToPrint.y;//메인메뉴에서 선택한 메뉴의 위치

	CursorUtil_Hide();

	/*메인 메뉴 화면에서 현재 커서가 위치하고있는 메뉴의 색상은 기본 색상과 다르게 표시하고(if) 다른메뉴는 기본 색상으로 표시(else)*/
	for (i = 0; i < menu->count; i++) {
		CursorUtil_GotoXY(startPositionXToPrint, startPositionYToPrint++);
		if (i == menu->selectedIndex) {
			FontUtil_ChangeFontColor(WHITE_INVERSION);
		}
		else {
			FontUtil_ChangeFontColor(menu->menuColor);
		}
		printf("%s", menu->items[i]);
	}//for

	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);//다시 폰트의 색을 기본색으로 초기화

	return ;
}

int Menu_ProcessKey(Menu* menu) {
	int key = -1;//입력한 키를 구분하기 위한 변수

	do {
			key = _getch();

			switch (key) {
			case ENTER_KEY_CODE: break;

			case UP_KEY_CODE:
				menu->selectedIndex = (menu->selectedIndex - 1 + menu->count) % menu->count;//가장 위의 메뉴에서 ↑버튼을 누르면 가장 아래의 메뉴로 이동하도록 +menu_count를 함
				Menu_Print(menu);
				break;
			case DOWN_KEY_CODE:
				menu->selectedIndex = (menu->selectedIndex + 1) % menu->count;
				Menu_Print(menu);
				break;

			default: break;
			}
		
	} while (key != ENTER_KEY_CODE);

	return menu->selectedIndex; //선택한 메뉴의 인덱스값 반환
}

const char* Menu_GetSelectedContent(Menu* menu) {
	return menu->items[menu->selectedIndex];
}
