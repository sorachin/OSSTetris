#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "Block.h"
#include "Util.h"
#include "Constant.h"

/*****��� ����*****
NEXT_QUEUE_SIZE : ����ڴ� NEXT_QUEUE_SIZE ������ŭ ������ ���� ���� ������ �� �� ����
BLOCK_EXAMPLES_SIZE : �� ����� ���� ��
********************/
#define NEXT_QUEUE_SIZE 2
#define BLOCK_EXAMPLES_SIZE 7

const static Point blockExamples[BLOCK_EXAMPLES_SIZE][POSITIONS_SIZE][POSITIONS_SIZE] = {//[��� ������ ��][�� ��Ͽ� �ʿ��� ���� ��][ó�� ���� ���ö��� x,y��ǥ]
	//���� ������
	//��������
	{
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } },
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } }
	},

	//    ��
	//������
	{
		{ { 0, 8 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 6 } },
		{ { -1, 6 }, { -1, 7 }, { 0, 7 }, { 1, 7 } }
	},

	//  ����
	//����
	{
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},

	//����
	//  ����
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } }
	},

	//  ��
	//������
	{
		{ { 0, 7 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { -1, 7 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},

	//��
	//������
	{
		{ { 0, 6 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { -1, 7 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 6 } }
	},

	//����
	//����
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } }
	}
};
/*****static �Լ� ����*****
Block.c���ο����� ���� �Լ�

_Block_MoveToDown, _Block_MoveToLeft, _Block_MoveToRight : ���� ���� �Ʒ�, ����, ���������� ��ĭ�� ������
_Block_RotateRight : ���� ���������� 90�� ȸ����Ų ��� ��ȯ (������� ������ �� ���������� 90���� ȸ����Ų ������� �����س���(blockExamples))
_Block_PrintDefaultBlock : next1, next2, hold���� �� ����� ����ϴ� ���
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

	srand((unsigned int)time(NULL));//���� ������ �������� ���õǾ�� �ϹǷ�

	if (isFirst) {//���� ó���� ���������� ����� ���
		block.current = rand() % BLOCK_EXAMPLES_SIZE;//block.current�� �������� ��� ��� ��ȣ ����
		block.hold = -1;
	}
	else {
		Queue_Get(&block.next, &block.current, sizeof(int));//block.current�� block.next����� ����
	}

	for (i = 0; i < POSITIONS_SIZE; i++) {
		for (j = 0; j < POSITIONS_SIZE; j++) {
			block.positions[i][j] = blockExamples[block.current][i][j];//block.positions�� ���� ��µ� ���� ��ǥ ����
		}
	}

	if (isFirst) {
		Queue_Create(&block.next, NEXT_QUEUE_SIZE, sizeof(int));//���� ��� ������ ����� ť�� ����

		do {
			next1 = rand() % BLOCK_EXAMPLES_SIZE;
		} while (block.current == next1); //������ ���� ���� ���� �� ���� ���� ���������� ������ ��µ� ���� ����
		Queue_Put(&block.next, &next1, sizeof(int));//������ ������ ť�� ������ ��µ� next1�� ���� ����
	}

	Queue_At(&block.next, &next1, 0, sizeof(int));// next1�� block.next���� ����
	do {
		next2 = rand() % BLOCK_EXAMPLES_SIZE;
	} while (block.current == next2 || next1 == next2);//next2�� ������� next1���� ���� �ʾƾ���
	Queue_Put(&block.next, &next2, sizeof(int));//ť�� ������� �ʴٸ� next2�� block.next���� ����

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

	return _Block_MoveToDown(block);//����Ű�� ������ ������ �ڵ����� �Ʒ��� ������
}

Point* Block_GetPositions(Block block) {
	return block.positions[block.direction];//���� ����� ��Ͽ����� ȸ���Ͽ� ���� � ������� �˾ƾ��ϹǷ� block.direction ���
}

void Block_ChangeCurrentForHold(Block* block) {
	int i;
	int j;
	int temp;

	/*���� block�� hold�� ���� �ٲ�*/
	temp = block->current;
	block->current = block->hold;
	block->hold = temp;

	if (block->current != -1) {//�ٲٱ��� hold�� ���� �־��� ���
		for (i = 0; i < POSITIONS_SIZE; i++) {
			for (j = 0; j < POSITIONS_SIZE; j++) {
				block->positions[i][j] = blockExamples[block->current][i][j];//���� ��µ� block�� ��ǥ ����
			}
		}
		block->direction = UP;
	}
	else {//hold�� ���� ���� ���
		*block = Block_Make(False, *block);//������ ��µǱ���ߴ� ����� �����ͼ� ����� �� next1, next2���� ���� (False : ó������ ��������� ���� �ƴ�)
	}

	return ;
}

void Block_PrintNext(Block block, int index, int x, int y) {
	int next;

	ScreenUtil_ClearRectangle(x + 2, y + 1, 12, 2); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("���� Next %d ����", index + 1);
	CursorUtil_GotoXY(x, y++);
	Queue_At(&block.next, &next, index, sizeof(int));
	_Block_PrintDefaultBlock(next, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("����������������");

	return ;
}

void Block_PrintHold(Block block, int x, int y) {
	ScreenUtil_ClearRectangle(x + 2, y + 1, 12, 2); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("����  Hold  ����");
	CursorUtil_GotoXY(x, y++);
	_Block_PrintDefaultBlock(block.hold, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("����������������");

	return ;
}

static Block _Block_MoveToDown(Block block) {
	int i;
	int j;

	for (i = 0; i < POSITIONS_SIZE; i++) {//���� �����ϴ� �� ���� ��ġ�� �� ĭ �Ʒ��� �̵�
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
		printf("��            ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��            ��");
		break;

	case 0:
		printf("��  �����  ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��            ��");
		break;

	case 1:
		printf("��       ��   ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��   ����   ��");
		break;

	case 2:
		printf("��     ���   ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��   ���     ��");
		break;

	case 3:
		printf("��   ���     ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��     ���   ��");
		break;

	case 4:
		printf("��     ��     ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��   ����   ��");
		break;

	case 5:
		printf("��   ��       ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��   ����   ��");
		break;

	case 6:
		printf("��    ���    ��");
		CursorUtil_GotoXY(x, (*y)++);
		printf("��    ���    ��");
		break;

	default: printf("PRINT BLOCK ERROR!!");
			 exit(1);
			 break;
	}

	return ;
}