#ifndef _BLOCK_H
#define _BLOCK_H

#include "Point.h"
#include "Queue.h"

#define POSITIONS_SIZE 4 //����� positions�迭�� ũ��

typedef struct _block {
	/*****����ü ���� ����*****
	positions : ���� ���� ��ġ ��ǥ
	current : ���� ������� ���������� ����
	next : ������ ��µ� ������ ������ ť�� ����
	hold : hold�� ���� ����� �������� ����(hold�� ���� ������� == -1)
	direction : ���� ȸ���Ͽ� ���� � ������� �˱����� ���
	***************************/
	Point positions[POSITIONS_SIZE][POSITIONS_SIZE];
	int current;
	Queue next;
	int hold;
	int direction;
	int color;
}Block;

/*****�Լ� ����*****
Block_Make : ���� ��µ� ��, next1, next2 �� ����
Block_Destroy : ������ ��µ� ����� ���� ����
Block_Move : �Է��� ���⿡���� ���� �����̵�����. �ƹ��͵� ������ ������ �ڵ����� �Ʒ��� ������
Block_GetPositions : ����� ��ġ�� (x,y)��ǥ�� ��ȯ
Block_ChangeCurrentForHold : ���� hold�������� ���
Block_PrintNext : block������ ����ִ� next(index+1)��° ���� �����ش�. (x,y) : Ŀ�� ������ġ
Block_PrintHold : hold�Ǿ��ִ� ���� ��� ��� (x,y) : Ŀ�� ������ġ
********************/
Block Block_Make(int isFirst, Block block);
void Block_Destroy(Block block);
Block Block_Move(Block block, int direction);
Point* Block_GetPositions(Block block);
void Block_ChangeCurrentForHold(Block* block);
void Block_PrintNext(Block block, int index, int x, int y);
void Block_PrintHold(Block block, int x, int y);

#endif