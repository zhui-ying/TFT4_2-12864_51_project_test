#ifndef __TFT_H
#define __TFT_H
#include <reg52.h>
#include <intrins.h>
#include "ch_enlib.h"

//#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz
//#define FOSC 12000000L //�������ã�ʹ��12M Hz
#define FOSC 24000000L //�������ã�ʹ��24M Hz

//IO�ӿڶ���
#define  LCD_DataPortH P0     //��8λ���ݿ�,8λģʽ��ֻʹ�ø�8λ 
sbit LCD_RS = P3^5;  		 //����/�����л�
sbit LCD_WR = P3^6;		  //д����
sbit LCD_RD =P3^4;		     //������
sbit LCD_CS=P3^7;		//Ƭѡ	
sbit LCD_REST = P3^3;	      //��λ 

void CLeanFullScreen(char VH,char VL);
char ShowPic(unsigned int x,unsigned int y,struct Picture pic);
void LcdInit(void);
char CleanRect(unsigned int x,unsigned int y,unsigned int x_lenght,unsigned int y_lenght);
void ShowString(unsigned int x,unsigned int y,unsigned char *str,unsigned int dcolor,unsigned int bgcolor);
void AddressSet(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LcdWriteData(char VH,char VL);
void ShowPoint(unsigned int x,unsigned int y,unsigned int color);

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#endif
