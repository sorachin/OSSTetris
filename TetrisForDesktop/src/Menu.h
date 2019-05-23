#ifndef _MENU_H
#define _MENU_H

#include "Point.h"

#define MENU_ITEMS_COUNT 20
#define MENU_ITEMS_CONTENT_SIZE 128

typedef struct _menu {
	char items[MENU_ITEMS_COUNT][MENU_ITEMS_CONTENT_SIZE];
	int count;
	int selectedIndex;
	Point startPositionToPrint;
	int menuColor;
}Menu;

/*****함수 설명*****
Menu_Create : 현재 메뉴 정보를 Menu형 변수에 저장
Menu_Print : 메인메뉴의 메뉴부분 출력
Menu_ProcessKey : 사용자가 어떤 메뉴를 선택했는지를 반환
Menu_GetSelectedContent : 선택된 메뉴의 이름을 반환
********************/
void Menu_Create(Menu* menu, char(*items)[MENU_ITEMS_CONTENT_SIZE], int count, int startPositionXToPrint, int startPositionYToPrint, int menuColor);
void Menu_Print(Menu* menu);
int Menu_ProcessKey(Menu* menu);
#endif