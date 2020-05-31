#pragma once


//网络数据包

//数据包头
typedef struct DataHeader
{
	/*数据长度*/
	short dataLength;
	/*命令*/
	short command;
}DH;

//数据包体
typedef struct UserInfo
{
	/*用户名*/
	char userName[16]="zhangSan";
	/*密码*/
	char passWord[16]="12345678";
}USER;

//返回值结构
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