#include <reg52.h>
#include "display.h"
#include "delay.h"


// DB	 P1
// CTL P3

sbit RS = P2 ^ 0;
sbit RW = P2 ^ 1;
sbit E = P2 ^ 2;
sbit CSA = P2 ^ 4;
sbit CSB = P2 ^ 5;



void write()
{
  RW = 0;
  E = 0;
  E = 1;
  delay(2);
  E = 0;
}



void clear()
{
  unsigned int i, j;
  CSA = 1;
  CSB = 0;
  for(i = 0; i < 8; i++)
    {
      RS = 0;
      RW = 0;
      P1 = 0xb8 + i;
      write();
      P1 = 0X40;
      write();

      for(j = 0; j < 64; j++)
        {
          RS = 1;
          RW = 0;
          P1 = 0x00;;
          write();
        }
    }
  CSA = 0;
  CSB = 1;
  for(i = 0; i < 8; i++)
    {
      RS = 0;
      RW = 0;
      P1 = 0xb8 + i;
      write();
      P1 = 0X40;
      write();

      for(j = 0; j < 64; j++)
        {
          RS = 1;
          RW = 0;
          P1 = 0x00;
          write();
        }
    }
}



void displayInit()
{
  E = 0;
  RS = 1;
  CSA = 1;
  CSB = 0;
  RS = 0;
  P1 = 0X3F;
  write();

  CSA = 0;
  CSB = 1;
  RS = 0;
  P1 = 0X3F;
  write();
}
