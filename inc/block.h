#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdlib.h>
#include <string.h>
#include "button.h"

#define MAX_X 8
#define MAX_Y 12


void blockInit();
void blockDestroy();

void genPiece();
void dropPiece();
void drawBlock();
void moveLeftPiece();
void moveRightPiece();
void judgeBlock();
void blockGameOver();

void saveBest();
void readBest();
unsigned int addBest(unsigned int s);
unsigned int rotateBlock(enum OPR opr);
#endif