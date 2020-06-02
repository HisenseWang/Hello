#pragma once

#define RANDOM(m,n) (rand()%(n-m+1)+m) //��������������ĺ�

enum  COMMAND
{
	CMD_IN,
	CMD_OUT,
	CMD_JION,
	CMD_UNK
};

enum  RESULT
{
	RES_TRUE,
	RES_FALSE,
	RES_ERROR,
	RES_UNK
};

//�������ݰ�

//���ݰ�ͷ
typedef struct DataHeader
{
	/*���ݳ���*/
	short dataLength;
	/*����*/
	short command;
	int sock_id = RANDOM(1, 100);
}DH;

//���ݰ���
typedef struct UserInfo
{
	/*�û���*/
	char userName[16]="zhangSan";
	/*����*/
	char passWord[16]="12345678";
	/*jion socket*/
	int sock_id = 1;
}USER;

//����ֵ�ṹ
typedef struct Result
{
	RESULT result;
	char msg[32];
}RES;



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

