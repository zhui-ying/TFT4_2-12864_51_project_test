#include "SD.h"

//DATA[512]����512�ֽڻ�������ע����Ҫʹ�� xdata�ؼ���
unsigned char xdata DATA[512];

//===========================================================
//дһ�ֽڵ�SD��,ģ��SPI���߷�ʽ
void SdWrite(unsigned char n)
{

	unsigned char i;

	for(i=8;i;i--)
	{
		SD_CLK=0;
		SD_MOSI=(n&0x80);
		n<<=1;
		SD_CLK=1;
	}
	SD_MOSI=1; 
} 
//===========================================================
//��SD����һ�ֽ�,ģ��SPI���߷�ʽ
unsigned char SdRead()
{
	unsigned char n,i;
	for(i=8;i;i--)
	{
		SD_CLK=0;
		SD_CLK=1;
		n<<=1;
		if(SD_MISO) n|=1;

	}
	return n;
}
//============================================================
//���SD������Ӧ
unsigned char SdResponse()
{
	unsigned char i=0,response;

	while(i<=8)
	{
		response = SdRead();   //��SD����һ��?
		if(response==0x00)
		break;
		if(response==0x01)
		break;
		i++;
	}
	return response;
} 
//================================================================
//�����SD��
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{

	SdWrite(command|0x40);                     //дһ�ֽڵ�SD?
	SdWrite(((unsigned char *)&argument)[0]);
	SdWrite(((unsigned char *)&argument)[1]);
	SdWrite(((unsigned char *)&argument)[2]);
	SdWrite(((unsigned char *)&argument)[3]);
	SdWrite(CRC);
}

//��ʼ��SD��
unsigned char SdInit(void)
{
	int delay=0, trials=0;
	unsigned char i;
	unsigned char response=0x01;

	SD_CS=1;
	for(i=0;i<=9;i++)
	SdWrite(0xff);
	SD_CS=0;

	//Send Command 0 to put MMC in SPI mode
	SdCommand(0x00,0,0x95);


	response=SdResponse();

	if(response!=0x01)
	{
		return 0;
	} 

	while(response==0x01)
	{
		SD_CS=1;
		SdWrite(0xff);
		SD_CS=0;
		SdCommand(0x01,0x00ffc000,0xff);
		response=SdResponse();
	} 

	SD_CS=1;
	SdWrite(0xff);
	return 1; 
}
//================================================================
//��SD��ָ����ַд����,һ�����512�ֽ�
/*unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
{
unsigned int count;
unsigned char dataResp;
//Block size is 512 bytes exactly
//First Lower SS

SD_CS=0;
//Then send write command
SdCommand(0x18,address,0xff);

if(SdResponse()==00)
{
SdWrite(0xff);
SdWrite(0xff);
SdWrite(0xff);
//command was a success - now send data
//start with DATA TOKEN = 0xFE
SdWrite(0xfe);
//now send data
for(count=0;count<len;count++) SdWrite(*Block++);

for(;count<512;count++) SdWrite(0);
//data block sent - now send checksum
SdWrite(0xff); //���ֽ�CRCУ��, Ϊ0XFFFF ��ʾ������CRC
SdWrite(0xff);
dataResp=SdRead();

while(SdRead()==0);

dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
SD_CS=1;
SdWrite(0xff);
if(dataResp==0x0b)
{
return 0;
}
if(dataResp==0x05)
return 1;

return 0;
}
return 0;
}  */

//=======================================================================
//��SD��ָ����ַ��ȡ����,һ�����512�ֽ�
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{
	unsigned int count;
	//Block size is 512 bytes exactly
	//First Lower SS

	//printf("MMC_read_block\n");

	SD_CS=0;
	//Then send write command
	SdCommand(0x11,address,0xff);

	if(SdResponse()==00)
	{
		//command was a success - now send data
		//start with DATA TOKEN = 0xFE
		while(SdRead()!=0xfe);

		for(count=0;count<len;count++) *Block++=SdRead(); 

		for(;count<512;count++) SdRead();

		//data block sent - now send checksum
		SdRead();
		SdRead();
		//Now read in the DATA RESPONSE token
		SD_CS=1;
		SdRead();
		return 1;
	}
	//printf("Command 0x11 (Read) was not received by the MMC.\n");
	return 0;
}
