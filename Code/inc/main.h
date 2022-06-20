#ifndef _MAIN_H_
#define _MAIN_H_

#include <reg52.h>
#include <intrins.h>
#include <absacc.h>
#include "display.h"
#include "menu.h"
#include "draw.h"
#include "button.h"
#include "delay.h"

#define VER_ADDRESS 0xaa00

sbit RS = P2 ^ 0;
sbit RW = P2 ^ 1;
sbit E = P2 ^ 2;
sbit CSA = P2 ^ 4;
sbit CSB = P2 ^ 5;
sbit LCDRST =P2^3;
sbit PIN_CONFIRM=P3^5;
sbit PIN_1=P3^2;
sbit PIN_3=P3^4;
sbit PIN_4=P3^5;

#endif