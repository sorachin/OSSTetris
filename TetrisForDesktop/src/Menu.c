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
	int startPositionYToPrint = menu->startPositionToPrint.y;//���θ޴����� ������ �޴��� ��ġ

	CursorUtil_Hide();

	/*���� �޴� ȭ�鿡�� ���� Ŀ���� ��ġ�ϰ��ִ� �޴��� ������ �⺻ ����� �ٸ��� ǥ���ϰ�(if) �ٸ��޴��� �⺻ �������� ǥ��(else)*/
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

	FontUtil_ChangeFontColor(DEFAULT_FONT_COLOR);//�ٽ� ��Ʈ�� ���� �⺻������ �ʱ�ȭ

	return ;
}

int Menu_ProcessKey(Menu* menu) {
	int key = -1;//�Է��� Ű�� �����ϱ� ���� ����

	do {
			key = _getch();

			switch (key) {
			case ENTER_KEY_CODE: break;

			case UP_KEY_CODE:
				menu->selectedIndex = (menu->selectedIndex - 1 + menu->count) % menu->count;//���� ���� �޴����� ���ư�� ������ ���� �Ʒ��� �޴��� �̵��ϵ��� +menu_count�� ��
				Menu_Print(menu);
				break;
			case DOWN_KEY_CODE:
				menu->selectedIndex = (menu->selectedIndex + 1) % menu->count;
				Menu_Print(menu);
				break;

			default: break;
			}
		
	} while (key != ENTER_KEY_CODE);

	return menu->selectedIndex; //������ �޴��� �ε����� ��ȯ
}

const char* Menu_GetSelectedContent(Menu* menu) {
	return menu->items[menu->selectedIndex];
}
