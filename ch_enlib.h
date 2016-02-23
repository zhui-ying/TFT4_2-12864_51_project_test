#ifndef __CH_ENLIB_H
#define __CH_ENLIB_H

// ------------------  图片的数据结构定义 ------------------------ //
struct Picture{
	unsigned int x_pix;//横向的像素点
	unsigned int y_pix;//纵向的像素点
	unsigned char *pic_buf;//存放图片的buf
};

// ------------------  英文字模的数据结构定义 ------------------------ //
struct EnCharType                 // 汉字字模数据结构 
{
	unsigned char  Msk[12];             // 点阵码数据(12*8有12个数据) 
	unsigned char  Index;            // 索引
};

extern unsigned char code logo_buf[];
extern struct Picture code logo;

extern struct EnCharType code en_char[];


#endif

