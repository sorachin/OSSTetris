#ifndef _BOARD_H
#define _BOARD_H

#include <windows.h>
#include "Block.h"
/* GameStatus ������ ����� ����, ���� ���¿� ���� �����
   PLAYING (������ ������ �ʰ�, ��� ������), END (���� ����)*/
enum GameStatus {
	PLAYING, END
};
//��Ʈ�������� ���� ���� ������, ���� ���� ������ ��ũ�� ����� ����
#define BOARD_ROW_SIZE 24 
#define BOARD_COL_SIZE 14


typedef struct _tetrisManager {
	/*
	TetrisManager ����ü�� ����
	board[BOARD_ROW_SIZE][BOARD_COL_SIZE] = 2 ���� �迭�� board (��Ʈ���� ��)
	block = ���(����, ���� ��)�� ���� ó��
	isHoldAvailable = if hold block is empty or hold turn is passed, this variable is true
	deletedLineCount ������ �� ��
	speedLevel = ���� �������� �ӵ� ���� (����� �Է°�)
	isTotalTimeAvailable = if totalTimeThread is alive, this variable is true
	*/
	char board[BOARD_ROW_SIZE][BOARD_COL_SIZE];
	Block block;
	int isHoldAvailable;
	Block shadow;
	int deletedLineCount;
	int speedLevel;
	int score;
	HANDLE totalTimeThread;
	long totalTime;
	int isTotalTimeAvailable;
}TetrisManager;

/*
**TetrisManager.c ���� ���ǵ� �Լ��� �����**
TetrisManager_Init = tetrisManager�� ��� �������� �ʱ�ȭ
TetrisManager_ProcessDirection = �̵����� �� ���⼳��
TetrisManager_ProcessAuto = �ڵ����� ���� ����������
TetrisManager_ProcessDirectDown = �̵����� ���� �ٷ� ������ �̵�
TetrisManager_ProcessDeletingLines =  ������ ������ ã�Ƽ� �����ϰ� ��Ʈ���� ���� ����
TetrisManager_IsReachedToBottom = ���� �̵����� ���� ������ �̵��� �� �ִ��� üũ, ������ �� true���� return
TetrisManager_ProcessReachedCase = ���� �ؿ� ����� �� ������� fix�ǰ� �ϰ�, �� ���� �����ϴ� ó��
TetrisManager_PrintBoard = ��Ʈ������ ����Լ�
TetrisManager_PrintDetailInfomation = ���۹�,���� �� ��� ��ġ ����
TetrisManager_GetDownMilliSecond = �ӵ� ������ �ش��ϴ� ��ŭ ���α׷��� ����
TetrisManager_MakeHold = Hold (��������)
TetrisManager_StartTotalTime = �������϶�  �ð� ó��
TetrisManager_PauseTotalTime = �Ͻ����� �Ǿ����� �ð� ó��
TetrisManager_StopTotalTime = �����Ǿ����� �ð� ó��
*/
void TetrisManager_Init(TetrisManager* tetrisManager, int speedLevel);
void TetrisManager_ProcessDirection(TetrisManager* tetrisManager, int direction);
void TetrisManager_ProcessAuto(TetrisManager* tetrisManager);
void TetrisManager_ProcessDirectDown(TetrisManager* tetrisManager);
void TetrisManager_ProcessDeletingLines(TetrisManager* tetrisManager);
int TetrisManager_IsReachedToBottom(TetrisManager* tetrisManager, int blockType);
int TetrisManager_ProcessReachedCase(TetrisManager* tetrisManager);
//void TetrisManager_Print(TetrisManager* tetrisManager); 
void TetrisManager_PrintBoard(TetrisManager* tetrisManager);
void TetrisManager_PrintDetailInfomation(TetrisManager* tetrisManager);
DWORD TetrisManager_GetDownMilliSecond(TetrisManager* tetrisManager);
void TetrisManager_MakeHold(TetrisManager* tetrisManager);
void TetrisManager_StartTotalTime(TetrisManager* tetrisManager);
void TetrisManager_PauseTotalTime(TetrisManager* tetrisManager);
void TetrisManager_StopTotalTime(TetrisManager* tetrisManager);

#endif