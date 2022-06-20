#include <stdlib.h>
#include "mine.h"
#include "button.h"
#include <reg52.h>
#include "draw.h"
#include <string.h>

enum MGS mineGameStatus = _idle;
extern unsigned int mBestScore[];
char mbase[MAX_MX][MAX_MY] = {0}; //x*y	//0为空 -1为有雷 整数为雷的个数
extern unsigned int score;
extern enum OPR opr;

void mineInit()
{
  char i, j;
  mineGameStatus = start;
  score = 0;
  genMine();
	  for(i = 0; i < 8; i++)
    for(j = 0; j < 8; j++)
			if(mbase[i][j]==-1)
        placeIMG_BLOCK(i, j);
}

void mineDestroy()
{
  score = 0;
  memset(mbase,0, sizeof(mbase));
  mineGameStatus = over;
}

unsigned mIsIegal(int i, int j)
{
  if(i >= 0 && i < MAX_MX && j >= 0 && j < MAX_MY)
    return 1;
  return 0;
}

unsigned int findSum(int i, int j)
{
  int re = 0;
  if(mbase[i][j] == -1)
    return -1;
  else
    {
      mIsIegal(i, j + 1) ? re += mbase[i][j + 1] : 0;
      mIsIegal(i, j - 1) ? re += mbase[i][j - 1] : 0;
      mIsIegal(i + 1, j) ? re += mbase[i + 1][j] : 0;
      mIsIegal(i - 1, j) ? re += mbase[i - 1][j] : 0;
      mIsIegal(i - 1, j + 1) ? re += mbase[i - 1][j + 1] : 0;
      mIsIegal(i + 1, j + 1) ? re += mbase[i + 1][j + 1] : 0;
      mIsIegal(i - 1, j - 1) ? re += mbase[i - 1][j - 1] : 0;
      mIsIegal(i + 1, j - 1) ? re += mbase[i + 1][j - 1] : 0;
    }
  return re;
}

void genMine()
{
  int i, j;
  score = 0;
  srand((unsigned)TL0);
  for(i = 0; i < MAX_MY; i++)
    mbase[rand() % 10][i] = -1;
  for(i = 0; i < MAX_MX; i++)
    for(j = 0; j < MAX_MX; j++)
      mbase[j][i] = findSum(j, i);
  mineGameStatus = start;
}