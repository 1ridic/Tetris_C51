#include "delay.h"
#include <intrins.h>

void delay(int n)

{
  int i, j;
  for(i = n; i > 0; i--)
    for(j = 4; j > 0; j--);
}

void delayms(int n)		//@11.0592MHz
{
  unsigned char i, j, k;
  for(k = n; k > 0; k--)
    {
      _nop_();
      _nop_();
      _nop_();
      i = 11;
      j = 190;
      do
        {
          while (--j);
        }
      while (--i);
    }
}
