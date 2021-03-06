#ifndef __ST7565_H
#define __ST7565_H

#include<reg52.h>
#include<intrins.h>

//---包含字库头文件
#define CHAR_CODE


//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--定时使用的IO口--//
#define DATA_PORT P0
sbit lcd_CS  = P3^5; //片选
sbit lcd_RST = P3^6; //复位
sbit lcd_RS  = P3^4; //命令与数据选择
sbit lcd_SCLK= P0^0; //串行钟
sbit lcd_SID = P0^1; //串行输入

//--定义全局函数--//
void Lcd12864WriteCmd(cmd);
void Lcd12864WriteData(dat);
void Lcd12864_Init();
void LcdClearScreen(void);
uchar LcdWriteString(uchar x, uchar y, uchar *cn);

#endif
