/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� ��TFT����SD����ȡ
* ʵ��˵�� ����ȡSD���ı���Ϣ��ͼƬ������
* ʵ��ƽ̨ ����̫51��Ƭ�������� V1.1
* ���ӷ�ʽ ��TFT�����������ӣ���SD�����뵽�������棬
*            ʹ�öŰ������ӣ�SD_CS��P1.0��SD_DIN��P1.1��SD_SCK��P1.2��SD_OUT��P1.3
* ע    �� ����ǰ����ֻ֧��С��4G��SD����SD���е�ͼƬ���ı���ַ��Ҫ����winhex����в鿴
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
****************************************************************************************/
#include<reg52.h>
#include <intrins.h>
#include <stdio.h>
#include "TFT.h"
#include "SD.h"
#include "toutch.h"
#include "uc1705.h"

//#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz
//#define FOSC 12000000L //�������ã�ʹ��12M Hz
#define FOSC 24000000L //�������ã�ʹ��24M Hz
#define TIME_MS 20 //�趨��ʱʱ�� ms ,��11.0592M�����£����׳���60ms


//ͨ��winhex�ɲ鿴���ļ�����ʼ��ַ
#define PIC1ADD 0x428000
#define PIC2ADD 0x402000
#define TXTADD  0x401000

sbit DULA = P2^6;
sbit WELA = P2^7;
sbit LED = P2^0;
sbit key1 = P2^1;
sbit key2 = P2^2;
sbit key_lock= P2^3;//��������������ʱΪ�� ����12864���ɿ�ʱΪ�� ���Բ���


struct pix_ touch_point;

/*******************************************************************************
* �� �� �� ��Delayms
* �������� ��ʵ�� ms������ʱ
* ��    �� ��ms
* ��    �� ����
*******************************************************************************/
void Delayms(unsigned int ms)
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
	#if FOSC == 11059200L
		for(j=0;j<114;j++);
	#elif FOSC == 12000000L
	  for(j=0;j<123;j++);
	#elif FOSC == 24000000L
		for(j=0;j<249;j++);
	#else
		for(j=0;j<114;j++);
	#endif
}

/*******************************************************************************
* �� �� �� ��Timer0Init
* �������� ����ʱ��0��ʼ��
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Timer0Init()
{
	TMOD=0x01; //���ö�ʱ��0������ʽΪ1
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	ET0=1; //������ʱ��0�ж�
	TR0=1;	//������ʱ��	
	EA=1;  //�����ж�
}

/*******************************************************************************
* �� �� �� ��UARTInit
* �������� �����ڳ�ʼ�� ������9600
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void UARTInit()
{
	SCON=0X50;			
	TMOD=0X20;			
	TH1=0XFd;		 
	TL1=0XFd;
	ES=1;								
	TR1=1;				
}

/*******************************************************************************
* �� �� �� ��PutNChar
* �������� ������N���ַ�����
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void PutNChar(char *buf,int length)
{
	int i;
  for(i=0;i<length;i++)
	{
	ES=0;
	TI=0;
	SBUF=buf[i];
	while(!TI);
	TI=0;
    ES=1;
	}
}

/*******************************************************************************
* �� �� �� ��ShowFullSDPic
* �������� ����ʾSD����bin�ļ���ʽ��ͼƬ
* ��    �� ��add ͼƬ��ʼ��ַ��ͨ��winhex�鿴
* ��    �� ����
*******************************************************************************/
void ShowFullSDPic(unsigned long add)
{
		unsigned long i=0,j=0,k=0;
		AddressSet(0,0,239,319);
		while(i < 76800)//240*320
		{
			SdReadBlock(DATA,add+k*512,512);
			for(j=0;j<256;j++)
			{
				LcdWriteData(DATA[j*2+1],DATA[j*2]);	//дһ�����ص�
				i++;
				if(i >= 76800) break;
			}
			k++;
		}
}

/*******************************************************************************
* �� �� �� ��Lcd12864Test
* �������� ��12864���Գ���,��������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Lcd12864Test(void)
{
	key_lock = 1;
	Delayms(10);
	//�ж����������Ƿ�Ϊ��
	if(key_lock == 1) return;//��Ϊ��ֱ�ӷ��أ���������Ĳ���
	Lcd12864_Init();
	LcdClearScreen();	
	while(1)
	{
		LcdWriteString(0, 0, "��̫�������޹�˾");
		LcdWriteString(0, 2, "��̫�������޹�˾");
		LcdWriteString(0, 4, "��̫�������޹�˾");
		LcdWriteString(0, 6, "��̫�������޹�˾");
		Delayms(800);
		LcdClearScreen();	
		Delayms(400);
	}
}

/*******************************************************************************
* �� �� ��: main
* ��������: ������
* ��    ��: ��
* ��    ��: ��
*******************************************************************************/
void main()
{
	//����ʵ��ʱ��ùر�����ܵ����������������������
	DULA = 1;
	P0 = 0xff;
	DULA = 0;
	WELA = 1;
	P0 = 0xff;
	WELA = 0;
	Timer0Init();
	
	Lcd12864Test();//�����12864���Ծ�ֱ��ͣ��������
	
	touch_point.x = 0;
	touch_point.y = 0;
	
	LcdInit();
	
	CLeanFullScreen(0xff,0xff);//����
	
	ShowString(20,0,"Color Screen Test V0.2",RED,WHITE);
	
	ShowString(20,20,"LCD init OK",RED,WHITE);
	
	if(!SdInit())
	{
		ShowString(20,40,"SD Init Fail",RED,WHITE);
	}
	
	//��ȡtxt����
	if(!SdReadBlock(DATA,TXTADD,512))
	{
		ShowString(20,40,"SD Read Fail",RED,WHITE);
	}
	else
	{
		ShowString(20,20,DATA,RED,WHITE);
		Delayms(2000);	
		
		//��ʾͼƬ
		ShowFullSDPic(PIC1ADD);
		Delayms(2000);		
	}	
	
	//��ʾ4������λ��
	
	ShowPoint(10,10,RED);//��ʾ��λ��
	ShowPoint(230,10,RED);//��ʾ��λ��
	ShowPoint(10,310,RED);//��ʾ��λ��
	ShowPoint(230,310,RED);//��ʾ��λ��
		
	ShowString(20,60,"Please Touch Me!",RED,WHITE);
	while (1)
	{

//		ShowFullSDPic(PIC2ADD);
//		Delayms(2000);
		
		touch_point = ReadToutchAxis();//��ȡ�����崥������
		if(touch_point.x != 0xffff)
		{
			ShowPoint(touch_point.x,touch_point.y,GREEN);//��ʾ���������
		}
	}
}

/*******************************************************************************
* �� �� �� ��Timer0Int
* �������� ����ʱ��0�жϺ��� �� ÿ��TIME_MS ms����
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void Timer0Int() interrupt 1
{
	static unsigned char count = 0;
	TH0=(65536-FOSC/12/1000*TIME_MS)/256;
	TL0=(65536-FOSC/12/1000*TIME_MS)%256;
	count++;
	if(count == 25)//1s
	{
		LED = !LED;//LED������
		count = 0;
	}
}
