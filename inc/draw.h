#ifndef _DRAW_H_
#define _DRAW_H_
////===================  �˵�   ==================
void drawNAME();
void drawNUMBER();
void drawMAINMENU(unsigned char);
void drawSUPNUM(unsigned char x,unsigned char y,unsigned char num);
void drawVOID8(unsigned char x,unsigned char y,unsigned char len);
void drawFONT(unsigned char x,unsigned char y,unsigned char len,
							unsigned char re,unsigned char FONT0[],unsigned char FONT1[]);
void drawVER(unsigned char x, unsigned char y);
////===================  ����   ==================

void placeIMG_BLOCK(unsigned char x,unsigned char y);
void placeVOID_BLOCK(unsigned char x,unsigned char y);
void drawGOOD(unsigned char x, unsigned char y);
void drawGAME(unsigned char x, unsigned char y);
void drawOVER(unsigned char x, unsigned char y);
void drawBLOCKSCORE();
void placeIMG_ROTATE(unsigned char x, unsigned char y);
void placeRIMG_BLOCK(unsigned char x, unsigned char y);
////===================  ��¼   ==================
void drawBEST(unsigned char x, unsigned char y);

////===================  ����   ==================
void drawVerticalDottedLine(char x);
#endif