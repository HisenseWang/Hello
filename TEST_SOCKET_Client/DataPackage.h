#pragma once

enum COMMAND
{
	CMD_IN,
	CMD_OUT,
	CMD_JOIN,
	CMD_UNK
};

enum RESULT
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
	int sock_id = 0;
}DH;

//数据包体
typedef struct UserInfo
{
	/*用户名*/
	char userName[16];
	/*密码*/
	char passWord[16];
}USER;

//返回值结构
typedef struct Result
{
	RESULT result;
	char msg[32];
}RES;




/*
 结构化数据包 C++ 风格继承，但是继承顺序必须与服务端C风格结构体内含变量一致
*/
typedef struct DataPackage_SEND:public DataHeader, public UserInfo
{
	//构造函数初始化
	DataPackage_SEND()
	{
		this->dataLength = sizeof(struct DataPackage_SEND);
		this->command = 0;

	}
}DP_SEND;//发送数据包

typedef struct DataPackage_RECV :public DataHeader, public Result
{
	//构造函数初始化
	DataPackage_RECV()
	{
		this->dataLength = sizeof(struct DataPackage_RECV);
		this->command = 0;
	}
}DP_RECV;//接受数据包



