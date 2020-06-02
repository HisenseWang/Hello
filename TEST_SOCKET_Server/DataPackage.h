#pragma once

#define RANDOM(m,n) (rand()%(n-m+1)+m) //产生是随机整数的宏

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

//网络数据包

//数据包头
typedef struct DataHeader
{
	/*数据长度*/
	short dataLength;
	/*命令*/
	short command;
	int sock_id = RANDOM(1, 100);
}DH;

//数据包体
typedef struct UserInfo
{
	/*用户名*/
	char userName[16]="zhangSan";
	/*密码*/
	char passWord[16]="12345678";
	/*jion socket*/
	int sock_id = 1;
}USER;

//返回值结构
typedef struct Result
{
	RESULT result;
	char msg[32];
}RES;



/*
 结构化数据包 发送
*/
typedef struct DataPackage_SEND
{
	DH dh;
	RES res;
}DP_SEND;

/*
 结构化数据包 接受
*/
typedef struct DataPackage_RECV
{
	DH dh;
	USER res;
}DP_RECV;

