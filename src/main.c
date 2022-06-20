#include "main.h"
#include "eeprom.h"

//#define WIPE_BEST

#define RELOAD (65535-6535)		//定时器填充值(1ms)
unsigned char TH, TL;

extern Menu *NOW;							//菜单指针
extern enum OPR opr;						//按键状态
extern unsigned int bestScore[];
extern unsigned int ver;

void int0() interrupt 0 			//外部中断：检测左右旋钮
{

  unsigned char LS, CS, flag = 0;
  LS = PIN_3;
  while(!PIN_1)
    {
      CS = PIN_3;
      flag = 1;
    }
  if(flag == 1)
    {
      if((LS == 0) && (CS == 1))
        opr = right;
      if((LS == 1) && (CS == 0))
        opr = left;
    }

}

void time() interrupt 1				//定时中断：检测确认按键
{
  EA = 0;
  TH0 = TH;
  TL0 = TL0 + TL;

  if(PIN_4 == 0)
    {
      delayms(6);
      if(PIN_4 == 0)
			{
        opr = confirm;
				delayms(25);
			}
    }
		
  EA = 1;
}

int main()
{

  TH = RELOAD / 256;	//填充值预计算
  TL = RELOAD - (RELOAD / 256) * 256;
  TH0 = TH;
  TL0 = TL;

  TMOD = 0x01; 				//使用定时器T0，高4位全为0,低4位GATE=0，C/T=0，采用工作方式1，M1=0,M1=1

  TR0 = 1; 						//启动定时器0
  EA = 1; 						//打开中断总开关
  ET0 = 1; 						//打开定时器0中断开关


  IT0 = 1;  						//中断触发方式为下边沿触发
  EX0 = 1;  						//打开外部中断0

  EA = 1;    						//打开中断总开关

  displayInit(); 			//屏幕初始化
  clear();
  menuInit();					//菜单初始化
	
	//若定义，清除排行榜
	#ifdef WIPE_BEST
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
	#endif
	//读取排行榜
  bestScore[0] = ((unsigned int)IapReadByte(IAP_ADDRESS + 0)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 1);
  bestScore[1] = ((unsigned int)IapReadByte(IAP_ADDRESS + 2)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 3);
  bestScore[2] = ((unsigned int)IapReadByte(IAP_ADDRESS + 4)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 5);
  bestScore[3] = ((unsigned int)IapReadByte(IAP_ADDRESS + 6)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 7);
  bestScore[4] = ((unsigned int)IapReadByte(IAP_ADDRESS + 8)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 9);
  bestScore[5] = ((unsigned int)IapReadByte(IAP_ADDRESS + 10)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 11);
  bestScore[6] = ((unsigned int)IapReadByte(IAP_ADDRESS + 12)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 13);
  bestScore[7] = ((unsigned int)IapReadByte(IAP_ADDRESS + 14)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 15);
  bestScore[8] = ((unsigned int)IapReadByte(IAP_ADDRESS + 16)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 17);
  bestScore[9] = ((unsigned int)IapReadByte(IAP_ADDRESS + 18)) * 256
                 + (unsigned int)IapReadByte(IAP_ADDRESS + 19);

  while(1)
    {

      NOW->f(NOW);

    }
}