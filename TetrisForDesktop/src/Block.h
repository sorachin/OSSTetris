#ifndef _BLOCK_H
#define _BLOCK_H

#include "Point.h"
#include "Queue.h"

#define POSITIONS_SIZE 4 //블록의 positions배열의 크기

typedef struct _block {
	/*****구조체 변수 설명*****
	positions : 현재 블럭의 위치 좌표
	current : 현재 블럭모양이 무엇인지를 저장
	next : 다음에 출력될 블럭들의 정보를 큐에 저장
	hold : hold된 블럭의 모양이 무엇인지 저장(hold된 블럭이 없을경우 == -1)
	direction : 블럭이 회전하여 현재 어떤 모양인지 알기위해 사용
	***************************/
	Point positions[POSITIONS_SIZE][POSITIONS_SIZE];
	int current;
	Queue next;
	int hold;
	int direction;
	int color;
}Block;

/*****함수 설명*****
Block_Make : 현재 출력될 블럭, next1, next2 블럭 생성
Block_Destroy : 다음에 출력될 블록의 정보 삭제
Block_Move : 입력한 방향에따라 블럭이 움직이도록함. 아무것도 누르지 않으면 자동으로 아래로 내려옴
Block_GetPositions : 블록의 위치를 (x,y)좌표로 반환
Block_ChangeCurrentForHold : 블럭을 hold했을때의 기능
Block_PrintNext : block변수에 들어있는 next(index+1)번째 블럭을 보여준다. (x,y) : 커서 시작위치
Block_PrintHold : hold되어있는 블럭의 모양 출력 (x,y) : 커서 시작위치
********************/
Block Block_Make(int isFirst, Block block);
void Block_Destroy(Block block);
Block Block_Move(Block block, int direction);
Point* Block_GetPositions(Block block);
void Block_ChangeCurrentForHold(Block* block);
void Block_PrintNext(Block block, int index, int x, int y);
void Block_PrintHold(Block block, int x, int y);

#endif