#include "menu.h"
#include "display.h"
#include "delay.h"
#include "draw.h"
#include "eeprom.h"
#include <stdio.h>
#include <stdlib.h>
#include "block.h"
#include <absacc.h>
#include "mine.h"

//���ڵ��Ե�Ԥ��������
//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define M_DEBUG M_BLOCK
#endif

Menu M_MAINMENU;//���˵�
Menu M_BLOCK; 	//����˹����
Menu M_BEST;		//����
Menu M_ABOUT;		//����
Menu M_MINE;		//ɨ��

Menu* NOW; //��ǰ�˵�ָ��
Menu* LAST = NULL; //��һ��״̬�Ĳ˵�

code unsigned int ver _at_ 0x7ffe;
enum OPR opr = idle;
static unsigned int local[5] = {0}; //���˵�ʹ��0,1�����๫��
unsigned int score;
unsigned int bestScore[10];
unsigned int mBestScore[10];
extern enum GS blockGameStatus;
extern enum GS mineGameStatus;

//���˵�
void m_mainmenu(struct _menu* this)
{
  if(LAST != &M_MAINMENU)
    {
      LAST = &M_MAINMENU;
      drawMAINMENU(local[0]);

      if(!local[1])
        {
          local[1] = 1;
          drawVER(90, 4);
          drawSUPNUM(80, 6, ver / 10000);		//���ư汾
          drawSUPNUM(89, 6, (ver % 10000) / 1000);
          drawSUPNUM(98, 6, (ver % 1000) / 100);
          drawSUPNUM(107, 6, (ver % 100) / 10);
          drawSUPNUM(116, 6, ver % 10);
          delayms(1000);
          drawVOID8(90, 4, 32);
          drawVOID8(80, 6, 46);
        }
    }

  if(opr == right)
    {
      local[0]++;
      if(local[0] > 3)
        local[0] = 3;

      opr = idle;
      drawMAINMENU(local[0]);
    }
  if(opr == left)
    {
      if(local[0] > 1)
        local[0]--;
      if(local[0] == 0)
        local[0] = 3;

      opr = idle;
      drawMAINMENU(local[0]);
    }
  if(opr == confirm)
    {

      switch(local[0])
        {
        case 0:
          return;
        case 1:
          NOW = &M_BLOCK;				//ת����һ�˵�
          break;
        case 2:
          NOW = &M_BEST;				//ת����һ�˵�
          break;
        case 3:
          NOW = &M_ABOUT;			//ת����һ�˵�
          break;
        }

      clear();
      opr = idle;
    }
}


//������Ϸ
void m_block(struct _menu* this)
{
  if(LAST != &M_BLOCK)
    {
      LAST = &M_BLOCK;
      blockInit();
      local[2] = 0; //Ŀǰ״̬ 0ƽ�� 1��ת
    }
  //����
  if(opr == confirm)
    {
      opr = idle;
      if(blockGameStatus == over)
        {
          clear();
          local[2] = 0;
          blockDestroy();
          NOW = &M_MAINMENU;
          return;
        }
      local[2] = ~local[2];
      return;
    }
  if(opr == left)
    {
      if(local[2])
        {
          rotateBlock(opr);
          local[2] = 0;
        }
      else
        moveLeftPiece();
      opr = idle;
      drawBlock();
      return;
    }
  if(opr == right)
    {
      if(local[2])
        {
          rotateBlock(opr);
          local[2] = 0;
        }
      else
        moveRightPiece();
      opr = idle;
      drawBlock();
      return;
    }
  switch(blockGameStatus)
    {
    case good:
      drawGOOD(70, 4);
      blockGameStatus = start;
      break;
    case over:
      drawGAME(70, 4);
      drawOVER(70, 6);
      break;
    default:
      drawVOID8(70, 4, 40);
      drawVOID8(70, 6, 32);
    }
  drawVerticalDottedLine(64);				//������
  drawBLOCKSCORE();									//������
  drawSUPNUM(70, 2, score / 10000);	//���Ʒ���
  drawSUPNUM(79, 2, (score % 10000) / 1000);
  drawSUPNUM(88, 2, (score % 1000) / 100);
  drawSUPNUM(97, 2, (score % 100) / 10);
  drawSUPNUM(106, 2, score % 10);
  if(local[2])
    placeIMG_ROTATE(118, 1);				//������ʾ��
  else
    placeRIMG_BLOCK(118, 1);


  blockGameOver();									//����Ƿ���Ϸ����
  genPiece();  											//������Ҫ�������¿�
  dropPiece();											//������Ҫ��������
  judgeBlock();											//������Ҫ�����һ��
  drawBlock();											//���ƽ���
  delayms(1500);

}

//����ҳ
void m_about(struct _menu* this)
{


  if(LAST != &M_ABOUT)
    {
      LAST = &M_ABOUT;
      local[2] = 0;
      local[3] = 0;
      delayms(2000);
      drawNAME(); 		 		//��������
      delayms(2000);
      drawNUMBER(); 			//����ѧ��
      delayms(3000);

      drawVER(5, 4);
      drawSUPNUM(2, 6, ver / 10000);		//���ư汾
      drawSUPNUM(11, 6, (ver % 10000) / 1000);
      drawSUPNUM(20, 6, (ver % 1000) / 100);
      drawSUPNUM(29, 6, (ver % 100) / 10);
      drawSUPNUM(38, 6, ver % 10);
    }
  if(opr == confirm)
    {
      opr = idle;
      NOW = &M_MAINMENU;				//ת����һ�˵�
      clear();
      //����������������������а�
      if(local[2] >= 30 && local[3] >= 30)
        {
          IapProgramByte(IAP_ADDRESS + 0, 0);
          IapProgramByte(IAP_ADDRESS + 1, 0);
          IapProgramByte(IAP_ADDRESS + 2, 0);
          IapProgramByte(IAP_ADDRESS + 3, 0);
          IapProgramByte(IAP_ADDRESS + 4, 0);
          IapProgramByte(IAP_ADDRESS + 5, 0);
          IapProgramByte(IAP_ADDRESS + 6, 0);
          IapProgramByte(IAP_ADDRESS + 7, 0);
          IapProgramByte(IAP_ADDRESS + 8, 0);
          IapProgramByte(IAP_ADDRESS + 9, 0);
          IapProgramByte(IAP_ADDRESS + 10, 0);
          IapProgramByte(IAP_ADDRESS + 11, 0);
          IapProgramByte(IAP_ADDRESS + 12, 0);
          IapProgramByte(IAP_ADDRESS + 13, 0);
          IapProgramByte(IAP_ADDRESS + 14, 0);
          IapProgramByte(IAP_ADDRESS + 15, 0);
          IapProgramByte(IAP_ADDRESS + 16, 0);
          IapProgramByte(IAP_ADDRESS + 17, 0);
          IapProgramByte(IAP_ADDRESS + 18, 0);
          IapProgramByte(IAP_ADDRESS + 19, 0);
        }
      local[2] = 0;
      local[3] = 0;
    }
  if(opr == left)
    {
      local[2]++;
      opr = idle;
      delayms(20);
    }
  if(opr == right)
    {
      local[3]++;
      opr = idle;
      delayms(20);
    }
}

//��¼ҳ
void m_best(struct _menu* this)
{


  if(LAST != &M_BEST)
    {
      LAST = &M_BEST;
      local[4] = 0;
      drawBEST(2, 0);														//������
      drawSUPNUM(2, 2, bestScore[0] / 10000);		//���Ʒ���
      drawSUPNUM(11, 2, (bestScore[0] % 10000) / 1000);
      drawSUPNUM(20, 2, (bestScore[0] % 1000) / 100);
      drawSUPNUM(29, 2, (bestScore[0] % 100) / 10);
      drawSUPNUM(38, 2, bestScore[0] % 10);

      drawSUPNUM(2, 4, bestScore[1] / 10000);		//���Ʒ���
      drawSUPNUM(11, 4, (bestScore[1] % 10000) / 1000);
      drawSUPNUM(20, 4, (bestScore[1] % 1000) / 100);
      drawSUPNUM(29, 4, (bestScore[1] % 100) / 10);
      drawSUPNUM(38, 4, bestScore[1] % 10);

      drawSUPNUM(2, 6, bestScore[2] / 10000);		//���Ʒ���
      drawSUPNUM(11, 6, (bestScore[2] % 10000) / 1000);
      drawSUPNUM(20, 6, (bestScore[2] % 1000) / 100);
      drawSUPNUM(29, 6, (bestScore[2] % 100) / 10);
      drawSUPNUM(38, 6, bestScore[2] % 10);

      drawSUPNUM(72, 0, bestScore[3] / 10000);		//���Ʒ���
      drawSUPNUM(81, 0, (bestScore[3] % 10000) / 1000);
      drawSUPNUM(90, 0, (bestScore[3] % 1000) / 100);
      drawSUPNUM(99, 0, (bestScore[3] % 100) / 10);
      drawSUPNUM(108, 0, bestScore[3] % 10);

      drawSUPNUM(72, 2, bestScore[4] / 10000);		//���Ʒ���
      drawSUPNUM(81, 2, (bestScore[4] % 10000) / 1000);
      drawSUPNUM(90, 2, (bestScore[4] % 1000) / 100);
      drawSUPNUM(99, 2, (bestScore[4] % 100) / 10);
      drawSUPNUM(108, 2, bestScore[4] % 10);

      drawSUPNUM(72, 4, bestScore[5] / 10000);		//���Ʒ���
      drawSUPNUM(81, 4, (bestScore[5] % 10000) / 1000);
      drawSUPNUM(90, 4, (bestScore[5] % 1000) / 100);
      drawSUPNUM(99, 4, (bestScore[5] % 100) / 10);
      drawSUPNUM(108, 4, bestScore[5] % 10);

      drawSUPNUM(72, 6, bestScore[6] / 10000);		//���Ʒ���
      drawSUPNUM(81, 6, (bestScore[6] % 10000) / 1000);
      drawSUPNUM(90, 6, (bestScore[6] % 1000) / 100);
      drawSUPNUM(99, 6, (bestScore[6] % 100) / 10);
      drawSUPNUM(108, 6, bestScore[6] % 10);
    }

  if(opr == confirm)
    {
      opr = idle;
      NOW = &M_MAINMENU;				//ת����һ�˵�
      clear();
    }
  if(opr == left)
    {
      opr = idle;
      local[4]++;
    }
  if(local[4] > 5)
    {
      local[4] = 0;
      NOW = &M_MINE;						//ɨ�׵������
      clear();
    }
}

//ɨ����Ϸ
void m_mine(struct _menu* this)
{
  if(LAST != &M_MINE)
    {
      LAST = &M_MINE;
      drawVerticalDottedLine(64);
      drawBLOCKSCORE();
      mineInit();
    }
  //����
  if(opr == confirm)
    {
      opr = idle;
      clear();
      mineDestroy();
      NOW = &M_MAINMENU;
      return;
    }
}

void menuInit()
{
  M_MAINMENU.f = m_mainmenu;
  M_BLOCK.f = m_block;
  M_ABOUT.f = m_about;
  M_BEST.f	=	m_best;
  M_MINE.f	=	m_mine;

#ifdef DEBUG_MODE
  NOW = &M_DEBUG;
#else
  NOW = &M_MAINMENU;
#endif
}
