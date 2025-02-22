#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "Block.h"
#include "Util.h"
#include "Constant.h"

/*****雌呪 竺誤*****
NEXT_QUEUE_SIZE : 紫遂切澗 NEXT_QUEUE_SIZE 鯵呪幻鏑 陥製拭 巷充 鷺薫戚 蟹臣走 硝 呪 赤製
BLOCK_EXAMPLES_SIZE : 鷺薫 乞丞税 曽嫌 呪
********************/
#define NEXT_QUEUE_SIZE 2
#define BLOCK_EXAMPLES_SIZE 7

const static Point blockExamples[BLOCK_EXAMPLES_SIZE][POSITIONS_SIZE][POSITIONS_SIZE] = {//[鷺系 曽嫌税 呪][廃 鷺系拭 琶推廃 け税 呪][坦製 鷺薫戚 蟹臣凶税 x,y疎妊]
	//鷺薫税 乞丞衣舛
	//けけけけ
	{
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } },
		{ { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 } },
		{ { -1, 6 }, { 0, 6 }, { 1, 6 }, { 2, 6 } }
	},

	//    け
	//けけけ
	{
		{ { 0, 8 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 6 } },
		{ { -1, 6 }, { -1, 7 }, { 0, 7 }, { 1, 7 } }
	},

	//  けけ
	//けけ
	{
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 7 }, { 0, 8 }, { 1, 6 }, { 1, 7 } },
		{ { -1, 6 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},

	//けけ
	//  けけ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { 0, 8 }, { 0, 7 }, { 1, 7 } }
	},

	//  け
	//けけけ
	{
		{ { 0, 7 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 7 } },
		{ { -1, 7 }, { 0, 6 }, { 0, 7 }, { 1, 7 } }
	},

	//け
	//けけけ
	{
		{ { 0, 6 }, { 1, 6 }, { 1, 7 }, { 1, 8 } },
		{ { -1, 8 }, { -1, 7 }, { 0, 7 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 8 } },
		{ { -1, 7 }, { 0, 7 }, { 1, 7 }, { 1, 6 } }
	},

	//けけ
	//けけ
	{
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } },
		{ { 0, 6 }, { 0, 7 }, { 1, 6 }, { 1, 7 } }
	}
};
/*****static 敗呪 竺誤*****
Block.c鎧採拭辞幻 紫遂吃 敗呪

_Block_MoveToDown, _Block_MoveToLeft, _Block_MoveToRight : 唖唖 鷺薫戚 焼掘, 図楕, 神献楕生稽 廃牒梢 崇送績
_Block_RotateRight : 鷺薫聖 神献楕生稽 90亀 噺穿獣轍 乞丞 鋼発 (鷺薫乞丞聖 舛税拝 凶 神献楕生稽 90亀梢 噺穿獣轍 授辞企稽 舛税背兜製(blockExamples))
_Block_PrintDefaultBlock : next1, next2, hold拭辞 鷺薫 乞丞聖 窒径馬澗 奄管
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

	srand((unsigned int)time(NULL));//鷺薫税 曽嫌澗 巷拙是稽 識澱鞠嬢醤 馬糠稽

	if (isFirst) {//亜舌 坦製拭 幻級戚走澗 鷺系析 井酔
		block.current = rand() % BLOCK_EXAMPLES_SIZE;//block.current拭 巷拙是税 鷺系 乞丞 腰硲 煽舌
		block.hold = -1;
	}
	else {
		Queue_Get(&block.next, &block.current, sizeof(int));//block.current拭 block.next鷺系聖 差紫
	}

	for (i = 0; i < POSITIONS_SIZE; i++) {
		for (j = 0; j < POSITIONS_SIZE; j++) {
			block.positions[i][j] = blockExamples[block.current][i][j];//block.positions拭 薄仙 窒径吃 鷺薫税 疎妊 煽舌
		}
	}

	if (isFirst) {
		Queue_Create(&block.next, NEXT_QUEUE_SIZE, sizeof(int));//陥製 鷺系 舛左亜 煽舌吃 泥研 幻糾

		do {
			next1 = rand() % BLOCK_EXAMPLES_SIZE;
		} while (block.current == next1); //陥製拭 蟹臣 鷺薫戚 薄仙 鷺薫 乞丞引 旭走 省聖凶猿走 陥製拭 窒径吃 鷺薫聖 衣舛
		Queue_Put(&block.next, &next1, sizeof(int));//是拭辞 持失吉 泥拭 陥製拭 窒径吃 next1鷺薫 舛左 煽舌
	}

	Queue_At(&block.next, &next1, 0, sizeof(int));// next1拭 block.next舛左 煽舌
	do {
		next2 = rand() % BLOCK_EXAMPLES_SIZE;
	} while (block.current == next2 || next1 == next2);//next2澗 薄仙鷺薫引 next1鷺薫引 旭走 省焼醤敗
	Queue_Put(&block.next, &next2, sizeof(int));//泥亜 搾嬢赤走 省陥檎 next2拭 block.next舛左 煽舌

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

	return _Block_MoveToDown(block);//号狽徹研 刊牽走 省生檎 切疑生稽 焼掘稽 鎧形身
}

Point* Block_GetPositions(Block block) {
	return block.positions[block.direction];//旭精 乞丞税 鷺系拭辞亀 噺穿馬食 薄仙 嬢恐 乞丞昔走 硝焼醤馬糠稽 block.direction 紫遂
}

void Block_ChangeCurrentForHold(Block* block) {
	int i;
	int j;
	int temp;

	/*薄仙 block引 hold吉 鷺薫聖 郊嘩*/
	temp = block->current;
	block->current = block->hold;
	block->hold = temp;

	if (block->current != -1) {//郊荷奄穿 hold吉 鷺薫戚 赤醸揮 井酔
		for (i = 0; i < POSITIONS_SIZE; i++) {
			for (j = 0; j < POSITIONS_SIZE; j++) {
				block->positions[i][j] = blockExamples[block->current][i][j];//薄仙 窒径吃 block税 疎妊 煽舌
			}
		}
		block->direction = UP;
	}
	else {//hold吉 鷺薫戚 蒸揮 井酔
		*block = Block_Make(False, *block);//陥製拭 窒径鞠奄稽梅揮 鷺系聖 亜閃人辞 窒径廃 板 next1, next2舛左 呪舛 (False : 坦製生稽 幻級嬢走澗 鷺薫戚 焼還)
	}

	return ;
}

void Block_PrintNext(Block block, int index, int x, int y) {
	int next;

	ScreenUtil_ClearRectangle(x + 2, y + 1, 12, 2); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("ΞΜ Next %d ΜΟ", index + 1);
	CursorUtil_GotoXY(x, y++);
	Queue_At(&block.next, &next, index, sizeof(int));
	_Block_PrintDefaultBlock(next, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("ΡΜΜΜΜΜΜΠ");

	return ;
}

void Block_PrintHold(Block block, int x, int y) {
	ScreenUtil_ClearRectangle(x + 2, y + 1, 12, 2); // use temp size (magic number)
	CursorUtil_GotoXY(x, y++);
	printf("ΞΜ  Hold  ΜΟ");
	CursorUtil_GotoXY(x, y++);
	_Block_PrintDefaultBlock(block.hold, x, &y);
	CursorUtil_GotoXY(x, y++);
	printf("ΡΜΜΜΜΜΜΠ");

	return ;
}

static Block _Block_MoveToDown(Block block) {
	int i;
	int j;

	for (i = 0; i < POSITIONS_SIZE; i++) {//鷺薫聖 姥失馬澗 唖 ＝税 是帖研 廃 牒 焼掘稽 戚疑
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
		printf("Ν            Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν            Ν");
		break;

	case 0:
		printf("Ν  ＝＝＝＝  Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν            Ν");
		break;

	case 1:
		printf("Ν       ＝   Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν   ＝＝＝   Ν");
		break;

	case 2:
		printf("Ν     ＝＝   Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν   ＝＝     Ν");
		break;

	case 3:
		printf("Ν   ＝＝     Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν     ＝＝   Ν");
		break;

	case 4:
		printf("Ν     ＝     Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν   ＝＝＝   Ν");
		break;

	case 5:
		printf("Ν   ＝       Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν   ＝＝＝   Ν");
		break;

	case 6:
		printf("Ν    ＝＝    Ν");
		CursorUtil_GotoXY(x, (*y)++);
		printf("Ν    ＝＝    Ν");
		break;

	default: printf("PRINT BLOCK ERROR!!");
			 exit(1);
			 break;
	}

	return ;
}