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

//便于调试的预编译命令
//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define M_DEBUG M_BLOCK
#endif

Menu M_MAINMENU;//主菜单
Menu M_BLOCK; 	//俄罗斯方块
Menu M_BEST;		//弹球
Menu M_ABOUT;		//关于
Menu M_MINE;		//扫雷

Menu* NOW; //当前菜单指针
Menu* LAST = NULL; //上一个状态的菜单

code unsigned int ver _at_ 0x7ffe;
enum OPR opr = idle;
static unsigned int local[5] = {0}; //主菜单使用0,1，其余公用
unsigned int score;
unsigned int bestScore[10];
unsigned int mBestScore[10];
extern enum GS blockGameStatus;
extern enum GS mineGameStatus;

//主菜单
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
          drawSUPNUM(80, 6, ver / 10000);		//绘制版本
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
          NOW = &M_BLOCK;				//转向下一菜单
          break;
        case 2:
          NOW = &M_BEST;				//转向下一菜单
          break;
        case 3:
          NOW = &M_ABOUT;			//转向下一菜单
          break;
        }

      clear();
      opr = idle;
    }
}


//方块游戏
void m_block(struct _menu* this)
{
  if(LAST != &M_BLOCK)
    {
      LAST = &M_BLOCK;
      blockInit();
      local[2] = 0; //目前状态 0平移 1旋转
    }
  //返回
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
  drawVerticalDottedLine(64);				//绘制线
  drawBLOCKSCORE();									//绘制字
  drawSUPNUM(70, 2, score / 10000);	//绘制分数
  drawSUPNUM(79, 2, (score % 10000) / 1000);
  drawSUPNUM(88, 2, (score % 1000) / 100);
  drawSUPNUM(97, 2, (score % 100) / 10);
  drawSUPNUM(106, 2, score % 10);
  if(local[2])
    placeIMG_ROTATE(118, 1);				//绘制提示符
  else
    placeRIMG_BLOCK(118, 1);


  blockGameOver();									//检查是否游戏结束
  genPiece();  											//若有需要，生成新块
  dropPiece();											//若有需要，块下落
  judgeBlock();											//若有需要，清除一行
  drawBlock();											//绘制界面
  delayms(1500);

}

//关于页
void m_about(struct _menu* this)
{


  if(LAST != &M_ABOUT)
    {
      LAST = &M_ABOUT;
      local[2] = 0;
      local[3] = 0;
      delayms(2000);
      drawNAME(); 		 		//绘制名字
      delayms(2000);
      drawNUMBER(); 			//绘制学号
      delayms(3000);

      drawVER(5, 4);
      drawSUPNUM(2, 6, ver / 10000);		//绘制版本
      drawSUPNUM(11, 6, (ver % 10000) / 1000);
      drawSUPNUM(20, 6, (ver % 1000) / 100);
      drawSUPNUM(29, 6, (ver % 100) / 10);
      drawSUPNUM(38, 6, ver % 10);
    }
  if(opr == confirm)
    {
      opr = idle;
      NOW = &M_MAINMENU;				//转向下一菜单
      clear();
      //若满足隐藏条件，清空排行榜
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

//记录页
void m_best(struct _menu* this)
{


  if(LAST != &M_BEST)
    {
      LAST = &M_BEST;
      local[4] = 0;
      drawBEST(2, 0);														//绘制字
      drawSUPNUM(2, 2, bestScore[0] / 10000);		//绘制分数
      drawSUPNUM(11, 2, (bestScore[0] % 10000) / 1000);
      drawSUPNUM(20, 2, (bestScore[0] % 1000) / 100);
      drawSUPNUM(29, 2, (bestScore[0] % 100) / 10);
      drawSUPNUM(38, 2, bestScore[0] % 10);

      drawSUPNUM(2, 4, bestScore[1] / 10000);		//绘制分数
      drawSUPNUM(11, 4, (bestScore[1] % 10000) / 1000);
      drawSUPNUM(20, 4, (bestScore[1] % 1000) / 100);
      drawSUPNUM(29, 4, (bestScore[1] % 100) / 10);
      drawSUPNUM(38, 4, bestScore[1] % 10);

      drawSUPNUM(2, 6, bestScore[2] / 10000);		//绘制分数
      drawSUPNUM(11, 6, (bestScore[2] % 10000) / 1000);
      drawSUPNUM(20, 6, (bestScore[2] % 1000) / 100);
      drawSUPNUM(29, 6, (bestScore[2] % 100) / 10);
      drawSUPNUM(38, 6, bestScore[2] % 10);

      drawSUPNUM(72, 0, bestScore[3] / 10000);		//绘制分数
      drawSUPNUM(81, 0, (bestScore[3] % 10000) / 1000);
      drawSUPNUM(90, 0, (bestScore[3] % 1000) / 100);
      drawSUPNUM(99, 0, (bestScore[3] % 100) / 10);
      drawSUPNUM(108, 0, bestScore[3] % 10);

      drawSUPNUM(72, 2, bestScore[4] / 10000);		//绘制分数
      drawSUPNUM(81, 2, (bestScore[4] % 10000) / 1000);
      drawSUPNUM(90, 2, (bestScore[4] % 1000) / 100);
      drawSUPNUM(99, 2, (bestScore[4] % 100) / 10);
      drawSUPNUM(108, 2, bestScore[4] % 10);

      drawSUPNUM(72, 4, bestScore[5] / 10000);		//绘制分数
      drawSUPNUM(81, 4, (bestScore[5] % 10000) / 1000);
      drawSUPNUM(90, 4, (bestScore[5] % 1000) / 100);
      drawSUPNUM(99, 4, (bestScore[5] % 100) / 10);
      drawSUPNUM(108, 4, bestScore[5] % 10);

      drawSUPNUM(72, 6, bestScore[6] / 10000);		//绘制分数
      drawSUPNUM(81, 6, (bestScore[6] % 10000) / 1000);
      drawSUPNUM(90, 6, (bestScore[6] % 1000) / 100);
      drawSUPNUM(99, 6, (bestScore[6] % 100) / 10);
      drawSUPNUM(108, 6, bestScore[6] % 10);
    }

  if(opr == confirm)
    {
      opr = idle;
      NOW = &M_MAINMENU;				//转向下一菜单
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
      NOW = &M_MINE;						//扫雷调试入口
      clear();
    }
}

//扫雷游戏
void m_mine(struct _menu* this)
{
  if(LAST != &M_MINE)
    {
      LAST = &M_MINE;
      drawVerticalDottedLine(64);
      drawBLOCKSCORE();
      mineInit();
    }
  //返回
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
