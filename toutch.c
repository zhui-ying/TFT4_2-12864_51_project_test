#include "toutch.h"

//unsigned int TP_X,TP_Y;	   //当前触控坐标

struct pix_ Tp_pix;	 //当前触控坐标的取样值  

/*******************************************************************************
* 函 数 名 ：SpiStart
* 函数功能 ：模拟SPI启动
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void SpiStart()                                     //SPI开始
{
	T_CS=1;
	T_CLK=1;
	T_DIN=1;
	T_CLK=1;
}

/*******************************************************************************
* 函 数 名 ：WriteCharTo7843
* 函数功能 ：SPI写数据
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void WriteCharTo7843(unsigned char num)
{
	unsigned char count=0;
	T_CLK=0;
	for(count=0;count<8;count++)
	{
		num<<=1;
		T_DIN=CY;
		T_CLK=0; _nop_(); //上升沿有效
		T_CLK=1; _nop_();
	}
}

/*******************************************************************************
* 函 数 名 ：ReadFromCharFrom7843
* 函数功能 ：SPI 读数据
* 输    入 ：无
* 输    出 ：读取到的数据
*******************************************************************************/
unsigned int ReadFromCharFrom7843()           
{
	unsigned char count=0;
	unsigned int num=0;
	for(count=0;count<12;count++)
	{
		num<<=1;
		T_CLK=1; _nop_(); //下降沿有效
		T_CLK=0; _nop_();
		if(T_OUT) num++;
	}
	return(num);
}	  

/*******************************************************************************
* 函 数 名 ：Makpix
* 函数功能 ：滤波功能
* 输    入 ：pix1，pix2 采样得到的2次数据
* 输    出 ：1 成功 0 失败
*******************************************************************************/
unsigned char Makpix(struct pix_ pix1,struct pix_ pix2)
{
unsigned char l=0;
int x=pix1.x>pix2.x?pix1.x-pix2.x:pix2.x-pix1.x;
int y=pix1.y>pix2.y?pix1.y-pix2.y:pix2.y-pix1.y;
if(x<14&&y<11)	 //软件滤波，2次取样的值相差太大的视为噪声
{
	l=1;
	Tp_pix.x=pix1.x;
	Tp_pix.y=pix1.y;
}
return l;
}

/*******************************************************************************
* 函 数 名 ：AD7843
* 函数功能 ：获取一个坐标点
* 输    入 ：无
* 输    出 ：pix 当前坐标
*******************************************************************************/
struct pix_ AD7843()            
{
	struct pix_ pix;
//delayms(1);                     //中断后延时以消除抖动，使得采样数据更准确
SpiStart();                       //启动SPI
//while(BUSY);                //如果BUSY信号不好使可以删除不用

T_CS=0;
WriteCharTo7843(0x90);        //送控制字 10010000 即用差分方式读X坐标 详细请见有关资料
//while(BUSY);               //如果BUSY信号不好使可以删除不用
T_CLK=1; _nop_();
T_CLK=0; _nop_();
pix.y=ReadFromCharFrom7843();
WriteCharTo7843(0xD0);//送控制字 11010000 即用差分方式读Y坐标 详细请见有关资料
T_CLK=1; _nop_();
T_CLK=0; _nop_();
pix.x=ReadFromCharFrom7843();
T_CS=1; 
return pix;
}

/*******************************************************************************
* 函 数 名 ：Getpix
* 函数功能 ：取采样值，此处使用软件滤波，2次取样的值相差太大的视为噪声
* 输    入 ：无
* 输    出 ：确认的坐标值
*******************************************************************************/
unsigned char Getpix() //
{
	struct pix_ pix1;
	struct pix_ pix2; 
	pix1=AD7843();
	pix2=AD7843();
 	return	Makpix(pix1,pix2);	
}

/*******************************************************************************
* 函 数 名 ：ReadToutchAxis
* 函数功能 ：循环读取点击坐标数据
* 输    入 ：无
* 输    出 ：pix_ pix.x = 0xffff表示无触屏点击 
*******************************************************************************/
struct pix_ ReadToutchAxis()
{
	struct pix_ pix;
	pix.x = 0xffff;
	pix.y = 0xffff;
	
		if(T_IRQ==0)
		{	
				if(Getpix()==1) //得到当前TP的取样值，此处使用软件滤波，2次取样的值相差太大的视为噪声
			{
				unsigned int lx,ly;	
				lx = 0;ly = 0;
				//lx=240-((Tp_pix.x-330)*10/142); //坐标反了 modfied by LC 2015.10.24 22:07
				//ly=320-((Tp_pix.y-380)/11);
				lx=(Tp_pix.x-330)*10/142; 
				ly=(Tp_pix.y-380)/11;		
				
				if(lx > 240) lx = 240;
				if(ly >320) ly = 320;

				pix.x = lx;
				pix.y = ly;
			}
				
		}
		return pix;
}