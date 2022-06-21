#include "block.h"
#include "draw.h"
#include "button.h"
#include <reg52.h>
#include "delay.h"
#include "eeprom.h"
#include <stdlib.h>

extern unsigned int bestScore[];
char base[MAX_X][MAX_Y] = {0}; 	//x*y	//0Ϊ�� 1Ϊ������� 2Ϊ�������� 3Ϊ��ת���ĵ�
char mask[MAX_X][MAX_Y] = {0}; 		//��ת�����
extern unsigned int score;
extern enum OPR opr;
enum GS blockGameStatus = __idle;

void blockInit()
{
  score = 0;
  srand((unsigned)TL0);
  blockGameStatus = start;
}

void blockDestroy()
{
  //д�����а�
  if(addBest(score))
    {
      IapEraseSector(IAP_ADDRESS);
      IapProgramByte(IAP_ADDRESS + 0, bestScore[0] >> 8);
      IapProgramByte(IAP_ADDRESS + 1, bestScore[0] % 256);
      IapProgramByte(IAP_ADDRESS + 2, bestScore[1] >> 8);
      IapProgramByte(IAP_ADDRESS + 3, bestScore[1] % 256);
      IapProgramByte(IAP_ADDRESS + 4, bestScore[2] >> 8);
      IapProgramByte(IAP_ADDRESS + 5, bestScore[2] % 256);
      IapProgramByte(IAP_ADDRESS + 6, bestScore[3] >> 8);
      IapProgramByte(IAP_ADDRESS + 7, bestScore[3] % 256);
      IapProgramByte(IAP_ADDRESS + 8, bestScore[4] >> 8);
      IapProgramByte(IAP_ADDRESS + 9, bestScore[4] % 256);
      IapProgramByte(IAP_ADDRESS + 10, bestScore[5] >> 8);
      IapProgramByte(IAP_ADDRESS + 11, bestScore[5] % 256);
      IapProgramByte(IAP_ADDRESS + 12, bestScore[6] >> 8);
      IapProgramByte(IAP_ADDRESS + 13, bestScore[6] % 256);
      IapProgramByte(IAP_ADDRESS + 14, bestScore[7] >> 8);
      IapProgramByte(IAP_ADDRESS + 15, bestScore[7] % 256);
      IapProgramByte(IAP_ADDRESS + 16, bestScore[8] >> 8);
      IapProgramByte(IAP_ADDRESS + 17, bestScore[8] % 256);
      IapProgramByte(IAP_ADDRESS + 18, bestScore[9] >> 8);
      IapProgramByte(IAP_ADDRESS + 19, bestScore[9] % 256);
    }
  memset(base, 0, sizeof(base));
  blockGameStatus = over;
}

void genPiece()
{

  char i, j;
  //��Ҫһ�������
  int random = rand() % 100;


  if(blockGameStatus == over)
    return;


  //��������δ������ɵķ�������
  for(i = 0; i < MAX_X; i++)
    for(j = 0; j < MAX_Y; j++)
      if(base[i][j] >= 2)
        return;

  //��������������
  if(random >= 0 && random <= 6)
    {
      base[4][9] = 2;
    }

  //��������������
  if(random >= 7 && random <= 15)
    {
      base[4][9] = 3;							//
      base[4][10] = 2;						//
    }
  else if(random >= 16 && random <= 22)
    {
      base[3][9] = 3;						////
      base[4][9] = 2;
    }

  //��������������
  else if(random >= 23 && random <= 32)
    {

      base[4][11] = 2;
      base[4][10] = 3;
      base[4][9] = 2;
    }
  else if(random >= 32 && random <= 38)
    {
      base[3][9] = 2;
      base[4][9] = 3;
      base[5][9] = 2;
    }
  else if(random >= 39 && random <= 47)
    {
      base[3][10] = 2;
      base[3][9] = 3;
      base[4][9] = 2;
    }
  else if (random >= 48 && random <= 55)
    {
      base[4][10] = 2;
      base[3][9] = 2;
      base[4][9] = 3;
    }
  else if (random >= 56 && random <= 63)
    {
      base[4][9] = 2;
      base[3][10] = 2;
      base[4][10] = 3;
    }
  else if (random >= 64 && random <= 69)
    {
      base[3][9] = 2;
      base[3][10] = 3;
      base[4][10] = 2;
    }

//���ĸ���������
  else if(random >= 70 && random <= 79)
    {
      base[3][10] = 2;
      base[4][10] = 2;
      base[3][9] = 2;
      base[4][9] = 2;
    }
  else if(random >= 80 && random <= 84)
    {
      base[3][10] = 2;
      base[4][10] = 3;
      base[3][9] = 2;
      base[4][11] = 2;
    }
  else if(random >= 85 && random <= 89)
    {
      base[3][10] = 2;
      base[4][10] = 3;
      base[3][11] = 2;
      base[4][9] = 2;
    }
  else if(random >= 90 && random <= 94)
    {
      base[3][10] = 2;
      base[4][10] = 3;
      base[4][9] = 2;
      base[5][9] = 2;
    }
  else if(random >= 95 && random <= 99)
    {
      base[3][10] = 2;
      base[4][10] = 3;
      base[3][9] = 2;
      base[4][9] = 2;
    }
}

//�����Ƿ�����Ļ��
unsigned char isIegal(char i, char j)
{
  if(i >= 0 && i < MAX_X && j >= 0 && j < MAX_Y && base[i][j] != 1)
    return 1;
  return 0;
}

//�������
void drawBlock()
{
  char i, j;

  for(i = 0; i < 8; i++)
    for(j = 0; j < 8; j++)
      if(base[i][j] != 0)
        placeIMG_BLOCK(i, j);
      else
        placeVOID_BLOCK(i, j);
}

//����������ϲ�
void dropPiece()
{
  char i, j, flag = 0;

  //����Ƿ񴥵�
  for(i = 0; i < MAX_X; i++)
    {
      for(j = 0; j < MAX_Y; j++)
        {
          if(flag == 1)
            break;
          if(base[i][j] >= 2)
            {
              //������,�����
              if(flag == 0 && (base[i][j - 1] == 1 || j == 0))
                {
                  flag = 1;
                  break;
                }

            }
        }
      if(flag == 1)
        break;
    }
  //ת��Ƕ��
  for(i = 0; i < MAX_X; i++)
    {
      for(j = 0; j < MAX_Y; j++)
        {
          if(base[i][j] >= 2)
            {
              //�ޱ��������
              if(flag == 0 && isIegal(i, j - 1))
                {
                  base[i][j - 1] = base[i][j];
                  base[i][j] = 0;
                }
              //�б����ת��
              else if(flag == 1)
                {
                  score++;
                  base[i][j] = 1;
                }
            }
        }
    }

}

//����ƽ��
void moveLeftPiece()
{

  char i, j;
  if(blockGameStatus == over)
    return;
  for(i = 0; i < MAX_X; i++)
    for(j = 0; j < MAX_Y; j++)
      if(base[i][j] == 2 && !isIegal(i - 1, j))
        {
          return;
        }
  for(i = 0; i < MAX_X; i++)
    for(j = 0; j < MAX_Y; j++)
      if(base[i][j] >= 2)
        {
          base[i - 1][j] = base[i][j];
          base[i][j] = 0;
        }
}
void moveRightPiece()
{
  char i, j;
  if(blockGameStatus == over)
    return;
  for(i = MAX_X - 1; i >= 0; i--)
    for(j = MAX_Y - 1; j >= 0; j--)
      if(base[i][j] == 2 && !isIegal(i + 1, j))
        {
          return;
        }
  for(i = MAX_X - 1; i >= 0; i--)
    for(j = MAX_Y - 1; j >= 0; j--)
      if(base[i][j] >= 2 )
        {
          base[i + 1][j] = base[i][j];
          base[i][j] = 0;
        }
}

//�ж���������
void judgeBlock()
{
  char i, j, k, h, count;
  if(blockGameStatus == over)
    return;
  //����ɨ��
  for(j = 0; j < MAX_Y; j++)
    {
      count = 0;
      for(i = 0; i < MAX_X; i++)
        {
          //�������
          if(base[i][j] == 1)
            {
              count++;
            }
        }
      //����������������
      if(count == MAX_X)
        {
          score = score + 10;
          blockGameStatus = good;
          for(k = 0; k < MAX_X; k++)
            for(h = j ; h < MAX_Y - 1; h++)
              base[k][h] = base[k][h + 1];
        }

    }
}

//�ж��Ƿ���Ϸ����
void blockGameOver()
{
  char j, i;
  if(blockGameStatus == over)
    return;
  for(j = 0; j < MAX_X; j++)
    {
      //�������������Ϸ����
      if(base[j][7] == 1)
        {
          blockGameStatus = over;
          break;
        }
    }
  //����������
  if(blockGameStatus == over)
    {
      for(j = 0; j < MAX_Y; j++)
        for(i = 0; i < MAX_X; i++)
          {
            //�˴����ƽ���Ч�ʣ�����ʵ��Ϊ������ϲ��Ч��
            delayms(10);
            base[i][j] = 1;
            placeIMG_BLOCK(i, j);
          }
    }
}

//�ж���߷�
unsigned int addBest(unsigned int s)
{
  int i, j;
  for(i = 0; i < 10; i++)
    if(bestScore[i] < s)
      {
        for(j = 9; j > i; j--)
          bestScore[j] = bestScore[j - 1];

        bestScore[i] = s;
        return 1;
      }
  return 0;
}

unsigned int rotateBlock(enum OPR opr)
{
  char i, j, cy, cx, flag = 0;
  //Ѱ����ת���ĵ�
  for(i = 0; i < MAX_X; i++)
    for(j = 0; j < MAX_Y; j++)
      {
        if(base[i][j] == 3)
          {
            flag = 1;
            cx = i;
            cy = j;
            break;
          }
      }

  if(!flag)
    return 0; //û�ҵ���ֱ�ӷ���
  if(opr == right)
    {
      //�ж��Ƿ�Ϸ� ���Ϸ�����ת
      for(i = - 1; i <= 1; i++)
        for(j = - 1; j <=  1; j++)
          if(base[cx + i][cy + j] == 2)
            if(isIegal(j + cx, -i + cy))
              {
                //���ɻ����
                mask[j + cx][-i + cy] = base[cx + i][cy + j];
              }
            else
              {
                memset(mask, 0, sizeof(mask));
                return 0;
              }
      //������в�
      for(i = 0; i < MAX_X; i++)
        for(j = 0; j <  MAX_Y; j++)
          if(base[i][j] == 2)
            base[i][j] = 0;

      //����㸲��
      for(i = -1; i <= 1; i++)
        for(j = -1; j <= 1; j++)
          if(mask[cx + i][cy + j] == 2)
            base[cx + i][cy + j] = mask[cx + i][cy + j];
    }

  if(opr == left)
    {
      //�ж��Ƿ�Ϸ� ���Ϸ�����ת
      for(i = - 1; i <= 1; i++)
        for(j = - 1; j <=  1; j++)
          if(base[cx + i][cy + j] == 2)
            if(isIegal(-j + cx, i + cy))
              {
                //���ɻ����
                mask[-j + cx][i + cy] = base[cx + i][cy + j];
              }
            else
              {
                memset(mask, 0, sizeof(mask));
                return 0;
              }
      //������в�
      for(i = 0; i < MAX_X; i++)
        for(j = 0; j <  MAX_Y; j++)
          if(base[i][j] == 2)
            base[i][j] = 0;

      //����㸲��
      for(i = -1; i <= 1; i++)
        for(j = -1; j <= 1; j++)
          if(mask[cx + i][cy + j] == 2)
            base[cx + i][cy + j] = mask[cx + i][cy + j];
    }
  memset(mask, 0, sizeof(mask));
  return 1;
}

