#ifndef __CH_ENLIB_H
#define __CH_ENLIB_H

// ------------------  ͼƬ�����ݽṹ���� ------------------------ //
struct Picture{
	unsigned int x_pix;//��������ص�
	unsigned int y_pix;//��������ص�
	unsigned char *pic_buf;//���ͼƬ��buf
};

// ------------------  Ӣ����ģ�����ݽṹ���� ------------------------ //
struct EnCharType                 // ������ģ���ݽṹ 
{
	unsigned char  Msk[12];             // ����������(12*8��12������) 
	unsigned char  Index;            // ����
};

extern unsigned char code logo_buf[];
extern struct Picture code logo;

extern struct EnCharType code en_char[];


#endif

