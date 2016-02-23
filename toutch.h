#ifndef __TOUTCH_H
#define __TOUTCH_H
#include<reg52.h>
#include <intrins.h>

/* touch panel interface define */

sbit T_IRQ       =    P3^2;   //��ⴥ������Ӧ�ź�
sbit T_OUT      =    P1^4;
sbit T_DIN       =    P1^5;	
sbit T_CS        =    P1^6;
sbit T_CLK	   =    P1^7;  

//��������ṹ��
struct pix_
{
	unsigned int x;
	unsigned int y;
};

struct pix_ ReadToutchAxis();

#endif
