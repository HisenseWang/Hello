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
	char userName[16];
	/*����*/
	char passWord[16];
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
 �ṹ�����ݰ� C++ ���̳У����Ǽ̳�˳�����������C���ṹ���ں�����һ��
*/
typedef struct DataPackage_SEND:public DataHeader, public UserInfo
{
	//���캯����ʼ��
	DataPackage_SEND()
	{
		this->dataLength = sizeof(struct DataPackage_SEND);
		this->command = 0;
	}
}DP_SEND;//�������ݰ�

typedef struct DataPackage_RECV :public DataHeader, public Result
{
	//���캯����ʼ��
	DataPackage_RECV()
	{
		this->dataLength = sizeof(struct DataPackage_RECV);
		this->command = 0;
	}
}DP_RECV;//�������ݰ�

