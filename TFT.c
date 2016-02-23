#include "TFT.h"

/*******************************************************************************
* �� �� �� ��Delayms
* �������� ��ʵ�� ms������ʱ
* ��    �� ��ms
* ��    �� ����
*******************************************************************************/
static void Delayms(unsigned int ms)
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

void LCDWritBus(char VH,char VL)   //��������д�뺯��
{	
    LCD_DataPortH=VH;	
   	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=VL;		
	LCD_WR=0;
	LCD_WR=1;
}

void LcdWriteCom(char VH,char VL)	 //��������-8λ����
{	
    LCD_RS=0;
	LCDWritBus(VH,VL);
}
void LcdWriteData(char VH,char VL) //��������-8λ����
{
    LCD_RS=1;
	LCDWritBus(VH,VL);
}
void LcdWriteComData(int com,int val)		   //������������
{
	LcdWriteCom(com>>8,com);
    LcdWriteData(val>>8,val);
}
void AddressSet(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{

	LcdWriteCom(0x00,0x46);LcdWriteData(x2,x1);	  
    LcdWriteCom(0x00,0x47);LcdWriteData(y2>>8,y2);  
    LcdWriteCom(0x00,0x48);LcdWriteData(y1>>8,y1);
  	LcdWriteCom(0x00,0x20);LcdWriteData(x1>>8,x1);	  
    LcdWriteCom(0x00,0x21);LcdWriteData(y1>>8,y1); 
    LcdWriteCom(0x00,0x22);							 
}

void LcdInit(void)
{

    LCD_REST=1;
    Delayms(5);	
	LCD_REST=0;
	Delayms(5);
	LCD_REST=1;
	LCD_CS=1;
	LCD_RD=1;
	LCD_WR=1;
	Delayms(5);

	LCD_CS =0;  //��Ƭѡʹ��
	 LcdWriteComData(0x11,0x2004);		
    LcdWriteComData(0x13,0xCC00);		
    LcdWriteComData(0x15,0x2600);	
	LcdWriteComData(0x14,0x252A);	
//	LcdWriteComData(0x14,0x002A);		
	LcdWriteComData(0x12,0x0033);		
	LcdWriteComData(0x13,0xCC04);		
	Delayms(1); 
	LcdWriteComData(0x13,0xCC06);		
	Delayms(1); 
	LcdWriteComData(0x13,0xCC4F);		
	Delayms(1); 
	LcdWriteComData(0x13,0x674F);
	LcdWriteComData(0x11,0x2003);
	Delayms(1); 	
	LcdWriteComData(0x30,0x2609);		
	LcdWriteComData(0x31,0x242C);		
	LcdWriteComData(0x32,0x1F23);		
	LcdWriteComData(0x33,0x2425);		
	LcdWriteComData(0x34,0x2226);		
	LcdWriteComData(0x35,0x2523);		
	LcdWriteComData(0x36,0x1C1A);		
	LcdWriteComData(0x37,0x131D);		
	LcdWriteComData(0x38,0x0B11);		
	LcdWriteComData(0x39,0x1210);		
	LcdWriteComData(0x3A,0x1315);		
	LcdWriteComData(0x3B,0x3619);		
	LcdWriteComData(0x3C,0x0D00);		
	LcdWriteComData(0x3D,0x000D);		
	LcdWriteComData(0x16,0x0007);		
	LcdWriteComData(0x02,0x0013);		
	LcdWriteComData(0x03,0x0003);		
	LcdWriteComData(0x01,0x0127);		
	Delayms(1); 
	LcdWriteComData(0x08,0x0303);		
	LcdWriteComData(0x0A,0x000B);		
	LcdWriteComData(0x0B,0x0003);   
	LcdWriteComData(0x0C,0x0000);   
	LcdWriteComData(0x41,0x0000);    
	LcdWriteComData(0x50,0x0000);   
	LcdWriteComData(0x60,0x0005);    
    LcdWriteComData(0x70,0x000B);    
	LcdWriteComData(0x71,0x0000);    
	LcdWriteComData(0x78,0x0000);    
	LcdWriteComData(0x7A,0x0000);   
	LcdWriteComData(0x79,0x0007);		
	LcdWriteComData(0x07,0x0051);   
	Delayms(1); 	
	LcdWriteComData(0x07,0x0053);		
	LcdWriteComData(0x79,0x0000);

	LcdWriteCom(0x00,0x22);

//	LCD_CS =1;  //�ر�Ƭѡʹ��

}	 

/*******************************************************************************
* �� �� �� ��CLeanFullScreen
* �������� ��������������ˢ��һ����ɫ
* ��    �� ��VH RGB565��8λ��VL RGB565��8λ
* ��    �� ����
*******************************************************************************/
void CLeanFullScreen(char VH,char VL)
{
	int i,j;
	AddressSet(0,0,239,319);
    for(i=0;i<320;i++)
	 {
	  for (j=0;j<240;j++)
	   	{
        	 LcdWriteData(VH,VL);
	    }

	  }
}

/*******************************************************************************
* �� �� �� ��ShowPic
* �������� ����ָ��λ����ʾһ��ͼ
* ��    �� ��x,yͼƬ����ʼ����  picͼƬ
* ��    �� ��0 ��ʾ���� �� 1 ��ʾ����
*******************************************************************************/
char ShowPic(unsigned int x,unsigned int y,struct Picture pic)
{
	unsigned int index,total_pix;
	if(x+pic.x_pix > 240) return 0;
	if(y+pic.y_pix > 320) return 0;//������Χ�����ش���
	
	AddressSet(x,y,x+pic.x_pix-1,y+pic.y_pix-1);
	total_pix = pic.x_pix * pic.y_pix;
	for(index=0;index<total_pix;index++)
	{
		LcdWriteData(pic.pic_buf[index*2+1],pic.pic_buf[index*2]);	 		
	}
	return 1;
}

/*******************************************************************************
* �� �� �� ��CleanRect
* �������� �����һ���������
* ��    �� ��x,yͼƬ����ʼ����  x_lenght ����x�᳤��y_lenght ����y�᳤
* ��    �� ��0 ��ʾ���� �� 1 ��ʾ����
*******************************************************************************/
char CleanRect(unsigned int x,unsigned int y,unsigned int x_lenght,unsigned int y_lenght)
{
	unsigned int index,total_pix;
	if(x+x_lenght > 240) return 0;
	if(y+y_lenght > 320) return 0;//������Χ�����ش���
	
	AddressSet(x,y,x+x_lenght-1,y+y_lenght-1);
	total_pix = x_lenght * y_lenght;
	for(index=0;index<total_pix;index++)
	{
		LcdWriteData(0xff,0xff);	 		
	}
	return 1;
}

/*******************************************************************************
* �� �� �� ��SearchEnPosition
* �������� ������Ӣ���ַ�����
* ��    �� ��value
* ��    �� ����en_char��λ��
*******************************************************************************/
int SearchEnPosition(unsigned char value)
{
	int i=0;
	while(en_char[i].Index != ' ')
	{
		if(en_char[i].Index == value) return i;
		i++;
	}
	return i;
}

/*******************************************************************************
* �� �� �� ��ShowEnChar
* �������� ����ָ��λ����ʾһ��Ӣ���ַ� 8*12��С
* ��    �� ��x,y ����  value Ҫ��ʾ���ַ�  dcolorǰ��ɫ bgcolor ����ɫ
* ��    �� ����
*******************************************************************************/
void ShowEnChar(unsigned int x,unsigned int y,unsigned char value,unsigned int dcolor,unsigned int bgcolor)	
{  
	unsigned char i,j;
	int index; 
	
	if(x+8 > 240) return ;
	if(y+12 > 320) return ;//������Χ������
	
	index = SearchEnPosition(value);
	
  AddressSet(x,y,x+7,y+11); //��������      
	for(j=0;j<12;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((en_char[index].Msk[j] & (1<<(7-i)))!=0)//��λ��ʼ
			{
				LcdWriteData(dcolor>>8,dcolor);
			} 
			else
			{
				LcdWriteData(bgcolor>>8,bgcolor);
			}   
		}
	 }
}

/*******************************************************************************
* �� �� �� ��ShowString
* �������� ����ʾ�ַ���
* ��    �� ��x,y ����λ��   str Ҫ��ʾ���ַ���  dcolorǰ��ɫ bgcolor ����ɫ
* ��    �� ����
*******************************************************************************/
void ShowString(unsigned int x,unsigned int y,unsigned char *str,unsigned int dcolor,unsigned int bgcolor)
{  
	unsigned int x1,y1;
	x1=x;
	y1=y;
	while(*str!='\0')
	{	
		ShowEnChar(x1,y1,*str,dcolor,bgcolor);
		x1+=8;
		if(x1+8 >= 240)//����
		{
			x1 = x;
			y1 = y1+12;
		}
		str++;
	}	
}	

/*******************************************************************************
* �� �� �� ��ShowPoint
* �������� ����ʾһ��3*3�����
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void ShowPoint(unsigned int x,unsigned int y,unsigned int color)
{
	char color_h,color_l;
	color_h = color>>8;
	color_l = color;
	AddressSet(x,y,x+2,y+2);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);
	LcdWriteData(color_h,color_l);	
}

