#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "Block.h"
#include "Util.h"
#include "Constant.h"

/*****상수 설명*****
NEXT_QUEUE_SIZE : 사용자는 NEXT_QUEUE_SIZE 개수만큼 다음에 무슨 블럭이 나올지 알 수 있음
BLOCK_EXAMPLES_SIZE : 블럭 모양의 종류 수
********************/
#define NEXT_QUEUE_SIZE 2
#define BLOCK_EXAMPLES_SIZE 7

const static Point blockExamples[BLOCK_EXAMPLES_SIZE][POSITIONS_SIZE][POSITIONS_SIZE] = {//[블록 종류의 수][한 블록에 필요한 ㅁ의 수][처음 블럭이 나올때의 x,y좌표]
	//블럭의 모양결정
	//ㅁㅁㅁㅁ
	{
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } },
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } }
	},

	//    ㅁ
	//ㅁㅁㅁ
	{
		{ { 0, 8 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 6 } },
		{ { -1, 6 }, { -1, 7 }, { 0, 7 }, { 1, 7 } }
	},

	//  ㅁㅁ
	//ㅁㅁ
	{
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},

	//ㅁㅁ
	//  ㅁㅁ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } }
	},

	//  ㅁ
	//ㅁㅁㅁ
	{
		{ { 0, 7 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { -1, 7 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},

	//ㅁ
	//ㅁㅁㅁ
	{
		{ { 0, 6 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { -1, 7 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 6 } }
	},

	//ㅁㅁ
	//ㅁㅁ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } }
	}
};
/*****static 함수 설명*****
Block.c내부에서만 사용될 함수

_Block_MoveToDown, _Block_MoveToLeft, _Block_MoveToRight : 각각 블럭이 아래, 왼쪽, 오른쪽으로 한칸씩 움직임
_Block_RotateRight : 블럭을 오른쪽으로 90도 회전시킨 모양 반환 (블럭모양을 정의할 때 오른쪽으로 90도씩 회전시킨 순서대로 정의해놓음(blockExamples))
_Block_PrintDefaultBlock : next1, next2, hold에서 블럭 모양을 출력하는 기능
***************************/
static Block _Block_MoveToDown(Block block);
static Block _Block_MoveToLeft(Block block);
static Block _Block_MoveToRight(Block block);
static Block _Block_RotateRight(Block block);
static void _Block_PrintDefaultBlock(int blockNumber, int x, int* y);

Block Block_Make(int isFirst, Block block) {
	int i;
	int j;
	int next1;
	int next2;

	srand((unsigned int)time(NULL));//블럭의 종류는 무작위로 선택되어야 하므로

	if (isFirst) {//가장 처음에 만들이지는 블록일 경우
		block.current = rand() % BLOCK_EXAMPLES_SIZE;//block.current에 무작위의 블록 모양 번호 저장
		block.hold = -1;
	}
	else {
		Queue_Get(&block.next, &block.current, sizeof(int));//block.current에 block.next블록을 복사
	}

	for (i = 0; i < POSITIONS_SIZE; i++) {
		for (j = 0; j < POSITIONS_SIZE; j++) {
			block.positions[i][j] = blockExamples[block.current][i][j];//block.positions에 현재 출력될 블럭의 좌표 저장
		}
	}

	if (isFirst) {
		Queue_Create(&block.next, NEXT_QUEUE_SIZE, sizeof(int));//다음 블록 정보가 저장될 큐를 만듦

		do {
			next1 = rand() % BLOCK_EXAMPLES_SIZE;
		} while (block.current == next1); //다음에 나올 블럭이 현재 블럭 모양과 같지 않을때까지 다음에 출력될 블럭을 결정
		Queue_Put(&block.next, &next1, sizeof(int));//위에서 생성된 큐에 다음에 출력될 next1블럭 정보 저장
	}

	Queue_At(&block.next, &next1, 0, sizeof(int));// next1에 block.next정보 저장
	do {
		next2 = rand() % BLOCK_EXAMPLES_SIZE;
	} while (block.current == next2 || next1 == next2);//next2는 현재블럭과 next1블럭과 같지 않아야함
	Queue_Put(&block.next, &next2, sizeof(int));//큐가 비어있지 않다면 next2에 block.next정보 저장

	block.direction = UP;
	block.color = rand() % (FONT_COLOR_SIZE - 1) + 1;

	return block;
}

void Block_Destroy(Block block) {
	Queue_Destroy(&block.next);

	return ;
}

Block Block_Move(Block block, int direction) {
	switch (direction) {
	case LEFT:
		return _Block_MoveToLeft(block);

	case RIGHT:
		return _Block_MoveToRight(block);

	case DOWN:
		return _Block_MoveToDown(block);

	case UP:
		return _Block_RotateRight(block);

	default: exit(1);
			 break;
	}

	return _Block_MoveToDown(block);//방향키를 누르지 않으면 자동으로 아래로 내려옴
}

Point* Block_GetPositions(Block block) {
	return block.positions[block.direction];//같은 모양의 블록에서도 회전하여 현재 어떤 모양인지 알아야하므로 block.direction 사용
}

void Block_ChangeCurrentForHold(Block* block) {
	int i;
	int j;
	int temp;

	/*현재 block과 hold된 블럭을 바꿈*/
	temp = block->current;
	block->current = block->hold;
	block->hold = temp;

	if (block->current != -1) {//바꾸기전 hold된 블럭이 있었던 경우
		for (i = 0; i < POSITIONS_SIZE; i++) {
			for (j = 0; j < POSITIONS_SIZE; j++) {
				block->positions[i][j] = blockExamples[block->current][i][j];//현재 출력될 block의 좌표 저장
			}
		}
		block->direction = UP;
	}
	else {//hold된 블럭이 없던 경우
		*block = Block_Make(False, *block);//다음에 출력되기로했던 블록을 가져와서 출력한 후 next1, next2정보 수정 (False : 처음으로 만들어지는 블럭이 아님)
	}

	return ;
}

void Block_PrintNext(Block block, int index, int x, int y) {
	int next;

	ScreenUtil_ClearRectangle(x + 2, y + 1, 12, 2); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("┏━ Next %d ━┓", index + 1);
	CursorUtil_GotoXY(x, y++);
	Queue_At(&block.next, &next, index, sizeof(int));
	_Block_PrintDefaultBlock(next, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("┗━━━━━━┛");

	return ;
}

void Block_PrintHold(Block block, int x, int y) {
	ScreenUtil_ClearRectangle(x + 2, y + 1, 12, 2); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("┏━  Hold  ━┓");
	CursorUtil_GotoXY(x, y++);
	_Block_PrintDefaultBlock(block.hold, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("┗━━━━━━┛");

	return ;
}

static Block _Block_MoveToDown(Block block) {
	int i;
	int j;

	for (i = 0; i < POSITIONS_SIZE; i++) {//블럭을 구성하는 각 ■의 위치를 한 칸 아래로 이동
		for (j = 0; j < POSITIONS_SIZE; j++) {
			block.positions[i][j].x++;
		}
	}

	return block;
}

static Block _Block_MoveToLeft(Block block) {
	int i;
	int j;

	for (i = 0; i < POSITIONS_SIZE; i++) {
		for (j = 0; j < POSITIONS_SIZE; j++) {
			block.positions[i][j].y--;
		}
	}

	return block;
}

static Block _Block_MoveToRight(Block block) {
	int i;
	int j;

	for (i = 0; i < POSITIONS_SIZE; i++) {
		for (j = 0; j < POSITIONS_SIZE; j++) {
			block.positions[i][j].y++;
		}
	}

	return block;
}

static Block _Block_RotateRight(Block block) {
	block.direction = (block.direction + 1) % POSITIONS_SIZE;

	return block;
}

static void _Block_PrintDefaultBlock(int blockNumber, int x, int* y) {
	switch (blockNumber) {
	case -1:
		printf("┃            ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃            ┃");
		break;

	case 0:
		printf("┃  ■■■■  ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃            ┃");
		break;

	case 1:
		printf("┃       ■   ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃   ■■■   ┃");
		break;

	case 2:
		printf("┃     ■■   ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃   ■■     ┃");
		break;

	case 3:
		printf("┃   ■■     ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃     ■■   ┃");
		break;

	case 4:
		printf("┃     ■     ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃   ■■■   ┃");
		break;

	case 5:
		printf("┃   ■       ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃   ■■■   ┃");
		break;

	case 6:
		printf("┃    ■■    ┃");
		CursorUtil_GotoXY(x, (*y)++);
		printf("┃    ■■    ┃");
		break;

	default: printf("PRINT BLOCK ERROR!!");
			 exit(1);
			 break;
	}

	return ;
}