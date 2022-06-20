#include "img.h"
#include "display.h"
#include "draw.h"
#include <reg52.h>
#include "block.h"

sbit RS = P2 ^ 0;
sbit RW = P2 ^ 1;
sbit E = P2 ^ 2;
sbit CSA = P2 ^ 4;
sbit CSB = P2 ^ 5;

void drawFONT(unsigned char x, unsigned char y, unsigned char len, unsigned char re,
              unsigned char FONT0[], unsigned char FONT1[])
{
  unsigned int j;
  if(x < 0 || x > 127)
    return;
  else if(x < 64)
    {
      CSA = 1;
      CSB = 0;
    }
  else
    {
      CSA = 0;
      CSB = 1;
      x = x - 63;
    }

  for(j = 0; j < len; j++)
    {

      RS = 0;
      RW = 0;
      P1 = 0xb8 + y;
      write();
      P1 = 0X40 + x + j;
      write(); //页y列x+j

      RS = 1;
      RW = 0;
      if(!re)
        P1 = FONT0[j];
      else
        P1 = ~FONT0[j];
      write();

      RS = 0;
      RW = 0;
      P1 = 0xb8 + y + 1;
      write();
      P1 = 0X40 + x + j;
      write(); //页y+1列x+j


      RS = 1;
      RW = 0;
      if(!re)
        P1 = FONT1[j];
      else
        P1 = ~FONT1[j];
      write();
    }
}

void drawNAME()
{
  drawFONT(5, 0, 48, 0, NAME_PAGE0, NAME_PAGE1);
}

void drawNUMBER()
{
  drawSUPNUM(0, 2, 2);
  drawSUPNUM(8, 2, 0);
  drawSUPNUM(16, 2, 2);
  drawSUPNUM(24, 2, 0);
  drawSUPNUM(32, 2, 1);
  drawSUPNUM(40, 2, 1);
  drawSUPNUM(48, 2, 4);
  drawSUPNUM(56, 2, 4);
  drawSUPNUM(64, 2, 9);
  drawSUPNUM(72, 2, 0);
}

void drawMAINMENU(unsigned char i)
{
  drawFONT(2, 0, 32, 0, MAINMANU_PAGE0, MAINMANU_PAGE1);
  drawFONT(22, 2, 32, i == 1, MAINMANU_PAGE2, MAINMANU_PAGE3);
  drawFONT(22, 4, 32, i == 2, MAINMANU_PAGE4, MAINMANU_PAGE5);
  drawFONT(22, 6, 32, i == 3, MAINMANU_PAGE6, MAINMANU_PAGE7);
}
void drawBLOCKSCORE()
{
  drawFONT(66, 0, 32, 0, FONT_SCORE0, FONT_SCORE1);
}

void drawGAME(unsigned char x, unsigned char y)
{
  drawFONT(x, y, 32, 0, FONT_GAME0, FONT_GAME1);
}

void drawOVER(unsigned char x, unsigned char y)
{
  drawFONT(x, y, 32, 0, FONT_OVER0, FONT_OVER1);
}

void drawGOOD(unsigned char x, unsigned char y)
{
  drawFONT(x, y, 40, 0, FONT_GOOD0, FONT_GOOD1);
}

void drawBEST(unsigned char x, unsigned char y)
{
  drawFONT(x, y, 32, 0, FONT_BEST0, FONT_BEST1);
}
void drawVER(unsigned char x, unsigned char y)
{
  drawFONT(x, y, 32, 0, FONT_VER0, FONT_VER1);
}
void placeIMG_BLOCK(unsigned char x, unsigned char y)
{
  char j;
  for(j = 0 ; j < 8; j++)
    {

      CSA = 1;
      CSB = 0;
      RS = 0;
      RW = 0;
      P1 = 0xb8 + 7 - y;
      write();
      P1 = 0X40 + x * 8 + j;
      write(); //页7-y列x*8+j屏A

      RS = 1;
      RW = 0;
      P1 = IMG_BLOCK[j];
      write();
    }
}

void placeVOID_BLOCK(unsigned char x, unsigned char y)
{
  char j;
  for(j = 0 ; j < 8; j++)
    {

      CSA = 1;
      CSB = 0;
      RS = 0;
      RW = 0;
      P1 = 0xb8 + 7 - y;
      write();
      P1 = 0X40 + x * 8 + j;
      write(); //页7-y列x*8+j屏A

      RS = 1;
      RW = 0;
      P1 = 0x00;
      write();
    }
}

void drawVerticalDottedLine(char x)
{
  unsigned int j;
  if(x < 0 || x > 127)
    return;
  else if(x < 64)
    {
      CSA = 1;
      CSB = 0;
    }
  else
    {
      x = x - 63;
      CSA = 0;
      CSB = 1;
    }

  for(j = 0 ; j < 8; j++)
    {

      RS = 0;
      RW = 0;
      P1 = 0xb8 + j;
      write();
      P1 = 0X40 + x;
      write(); //页j列x

      RS = 1;
      RW = 0;
      P1 = 0x55;
      write();

    }
}

void drawSUPNUM(unsigned char x, unsigned char y, unsigned char num)
{
  unsigned int j;
  if(x < 0 || x > 127)
    return;
  else if(x < 64)
    {
      CSA = 1;
      CSB = 0;
    }
  else
    {
      x = x - 63;
      CSA = 0;
      CSB = 1;
    }
  for(j = 0; j < 8; j++)
    {
      RS = 0;
      RW = 0;
      P1 = 0xb8 + y;
      write();
      P1 = 0X40 + x + j;
      write(); //页y列x+j

      RS = 1;
      RW = 0;
      P1 = SUPER_NUM0[8 * num + j];
      write();

      RS = 0;
      RW = 0;
      P1 = 0xb8 + y + 1;
      write();
      P1 = 0X40 + x + j;
      write(); //页y+1列x+j

      RS = 1;
      RW = 0;
      P1 = SUPER_NUM1[8 * num + j];
      write();
    }
}

void drawVOID8(unsigned char x, unsigned char y, unsigned char len)
{
  unsigned int j;
  if(x < 0 || x > 127)
    return;
  else if(x < 64)
    {
      CSA = 1;
      CSB = 0;
    }
  else
    {
      x = x - 63;
      CSA = 0;
      CSB = 1;
    }
  for(j = 0; j < len; j++)
    {
      RS = 0;
      RW = 0;
      P1 = 0xb8 + y;
      write();
      P1 = 0X40 + x + j;
      write(); //页y列x+j

      RS = 1;
      RW = 0;
      P1 = 0x00;
      write();

      RS = 0;
      RW = 0;
      P1 = 0xb8 + y + 1;
      write();
      P1 = 0X40 + x + j;
      write(); //页y+1列x+j

      RS = 1;
      RW = 0;
      P1 = 0x00;
      write();
    }
}

void placeIMG_ROTATE(unsigned char x, unsigned char y)
{
  unsigned int j;
  if(x < 0 || x > 127)
    return;
  else if(x < 64)
    {
      CSA = 1;
      CSB = 0;
    }
  else
    {
      x = x - 63;
      CSA = 0;
      CSB = 1;
    }
  for(j = 0; j < 8; j++)
    {
      RS = 0;
      RW = 0;
      P1 = 0xb8 + y;
      write();
      P1 = 0X40 + x + j;
      write(); //页y列x+j

      RS = 1;
      RW = 0;
      P1 = IMG_ROTATE[j];
      write();

    }
}

void placeRIMG_BLOCK(unsigned char x, unsigned char y)
{
  unsigned int j;
  if(x < 0 || x > 127)
    return;
  else if(x < 64)
    {
      CSA = 1;
      CSB = 0;
    }
  else
    {
      x = x - 63;
      CSA = 0;
      CSB = 1;
    }
  for(j = 0; j < 8; j++)
    {
      RS = 0;
      RW = 0;
      P1 = 0xb8 + y;
      write();
      P1 = 0X40 + x + j;
      write(); //页y列x+j

      RS = 1;
      RW = 0;
      P1 = IMG_BLOCK[j];
      write();
    }
}