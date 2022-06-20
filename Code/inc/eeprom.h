#ifndef _EEPROM_H_
#define _EEPROM_H_

/*Declare SFR associated with the IAP */
sfr IAP_DATA    =   0xE2;           //Flash�Ĵ���
sfr IAP_ADDRH   =   0xE3;           //Flash �ߵ�ַ
sfr IAP_ADDRL   =   0xE4;           //Flash �͵�ַ
sfr IAP_CMD     =   0xE5;           //Flash ����Ĵ���
sfr IAP_TRIG    =   0xE6;           //Flash ���ƴ�����
sfr IAP_CONTR   =   0xE7;           //Flash ���ƼĴ���

/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE    0               //����
#define CMD_READ    1               //��
#define CMD_PROGRAM 2               //д
#define CMD_ERASE   3               //����

/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/
//#define ENABLE_IAP 0x80           //if SYSCLK<40MHz
#define ENABLE_IAP   0x81           //if SYSCLK<20MHz
//#define ENABLE_IAP x82            //if SYSCLK<10MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<5MHz

//Start address for STC89C58xx EEPROM
#define IAP_ADDRESS 0xF200

typedef unsigned char BYTE;
typedef unsigned int WORD;

void Delay(BYTE n);
void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);

#endif