#pragma once


//�������ݰ�

//���ݰ�ͷ
typedef struct DataHeader
{
	/*���ݳ���*/
	short dataLength;
	/*����*/
	short command;
}DH;

//���ݰ���
typedef struct UserInfo
{
	/*�û���*/
	char userName[16]="zhangSan";
	/*����*/
	char passWord[16]="12345678";
}USER;

//����ֵ�ṹ
typedef struct Result
{
	unsigned char result;
	char msg[32];
}RES;

enum COMMAND
{
	CMD_IN,
	CMD_OUT,
	CMD_UNK
};

enum RESULT
{
	RES_TRUE,
	RES_FALSE,
	RES_ERROR,
	RES_UNK
};

/*
 �ṹ�����ݰ� ����
*/
typedef struct DataPackage_SEND
{
	DH dh;
	RES res;
}DP_SEND;

/*
 �ṹ�����ݰ� ����
*/
typedef struct DataPackage_RECV
{
	DH dh;
	USER res;
}DP_RECV;