#define WIN32_LEAN_AND_MEAN  //防止 Windows.h 与 WinSock2.h 宏定义冲突 ，或调换两个头文件顺序
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //不显示警告信息 
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h>
#include<thread>
#include"DataPackage.h"
using namespace std;

#pragma warning(disable:6319)//消除表达式中使用逗号运算符的警告
//#pragma comment(lib,"ws2_32.lib") //连接静态库 ,可以添加到属性连接器中的附加依赖
//客户端：


int  handle_request(SOCKET _socket); //数据接受
string get_result(int r);
void Get_input_information(SOCKET sock);

bool isRuning = true;

int main(void)
{
	WORD ver = MAKEWORD(2, 2);//创建版本号  MAKEWORD 创建版本号宏定义
	WSADATA wsadata; //结构体

	/*
	 WSAStartup
	 此函数在应用程序中初始化winsockDLL
	 参数：  _In_ WORD wVersionRequested ： 调用程序使用windows socket的最高版本。 高字节指定小的版本号，低字节指定高的版本号。
			_Out_ LPWSADATA lpWSAData ： 指向WSADATA数据结构体指针，接收Windows Socket的实现细节 。在 Windows 中以 LP开头的参数大多数数据类型就是去掉LP后的参数类型名。如 LPWSADATA 对应 WSADATA
	 返回值：如果成功，WSAStartup函数返回0。

	*/
	int iResult = WSAStartup(ver, &wsadata); //启动SOCKET连接
	if (iResult != NO_ERROR)
	{
		cout << "WSAStartup function failed with error: " << iResult << endl;
	}



	//	1：建立一个socket
	SOCKET _socket_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket_server == INVALID_SOCKET)
	{
		cout << "socket function failed with error = " << WSAGetLastError() << endl;
		return 1;
	}
	else
	{
		cout << "socket function  success" << endl;
	}
	//	2：连接服务器 connect
	char* strip = (char*)("127.0.0.1");
	SOCKADDR_IN _sock_addr{};
	_sock_addr.sin_family = AF_INET;
	_sock_addr.sin_port = htons(4567);
	inet_pton(AF_INET, strip, &_sock_addr.sin_addr.S_un.S_addr);
	//_sock_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //inet_pton(AF_INET,"127.0.0.1" ,(void*)&_sock_addr.sin_addr.S_un.S_addr);

	/*
	  connect:
		参数：	 _In_ SOCKET s,  标识未连接套接字的描述符
				 _In_reads_bytes_(namelen) const struct sockaddr FAR * name,  指向应建立连接的sockaddr结构的指针
				 _In_ int namelen   name参数所指向的sockaddr结构的长度（以字节为单位）
		返回值：
				如果未发生错误，则 connect返回零。否则，它将返回SOCKET_ERROR，并且可以通过调用WSAGetLastError来检索特定的错误代码 。
	*/
	iResult = connect(_socket_server, (SOCKADDR*)&_sock_addr, sizeof(SOCKADDR_IN));
	if (iResult == SOCKET_ERROR)
	{
		cout << "connect function failed with error: " << WSAGetLastError() << endl;
		iResult = closesocket(_socket_server);
		if (iResult == SOCKET_ERROR)
		{
			cout << "closesocket function failed with error: " << WSAGetLastError() << endl;
		}
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "Connected to server is success." << endl;
	}

     thread t1(Get_input_information, _socket_server);
	 t1.detach();
	//循环发送与接受
	while (true)
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);//初始化集合
		FD_SET(_socket_server, &fdRead);
		timeval t = { 1,0 };
		iResult = select(_socket_server, &fdRead, NULL, NULL, &t);
		if (iResult < 0)
		{
			cout << "select function failed with error: " << WSAGetLastError() << endl;
			break;
		}
		if (FD_ISSET(_socket_server, &fdRead))
		{
			FD_CLR(_socket_server, &fdRead);
			if (handle_request(_socket_server) == -1)
			{
				cout << "处理客户端消息 错误！程序结束。" << endl;
				break;
			}
		}

		if (!isRuning)
		{
			break;
		}

	}


	//	{
	//		//可选
	//		//char* sendbuf =(char*) ("this is a test");
	//		iResult = send(_socket_server, sendbuf, strlen(sendbuf), 0);//发送数据
	//		if (iResult == SOCKET_ERROR)
	//		{
	//			cout << "send failed: " << WSAGetLastError() << endl;
	//			closesocket(_socket_server);
	//			WSACleanup();
	//			return 1;
	//		}
	//		cout << "Bytes Sent : " << iResult << endl;
	//		/*
	//		   SD_RECEIVE (0): 关机接收操作
	//		   SD_SEND	  (1): 关闭发送操作
	//		   SD_BOTH    (2): 关闭发送和接收操作
	//		*/
	//	iResult = shutdown(_socket_server, SD_SEND);//关闭发送操作
	//	if (iResult == SOCKET_ERROR)
	//	{
	//		cout << "shutdown failed: " << WSAGetLastError() << endl;
	//		closesocket(_socket_server);
	//		WSACleanup();
	//		return 1;
	//	}
	//
	//}

	//   //	3：接受客户端消息 recv
	/*
	do
	{
		char buf[256] = { 0 };
		iResult = recv(_socket_server, buf, sizeof(buf), 0);
		if (iResult > 0)
		{
			cout << "Bytes received : " << iResult << endl;
			cout << "recv msg :" << buf << endl;
		}
		else if (iResult == 0)
		{
			cout << "Connection closed. " << endl;
		}
		else
		{
			cout << "recv failed : " << WSAGetLastError() << endl;
		}

	} while (iResult > 0);
	 */


	 //	4：关闭套接字 closesocket
	closesocket(_socket_server);
	WSACleanup();//关闭SOCKET连接  清除 windows socket 环境
	return 0;

}

int  handle_request(SOCKET _socket_client)
{
	//第五步，接受客户端消息
	
	//DP_RECV recdp{};//接受缓存区
	char recvbuf[1024] = { 0 };//接受数据缓冲区 进行拆包处理
	int recvlen = recv(_socket_client, recvbuf, sizeof(DP_RECV), 0);
	DataHeader* dp_HD = (DataHeader*)recvbuf; //进行数据结构转换
	if (recvlen <= 0)
	{
		cout << "服务端 ： Accept data length : [" << recvlen << "]  " << "server program exit ." << endl;
		return -1;
	}
	cout << "报文长度：[" << dp_HD->dataLength << "]  报文命令：[" << dp_HD->command << "]" << endl;
	RES* dp_rs = (RES*)(recvbuf + sizeof(DataHeader));//指针偏移
	switch (dp_HD->command)
	{
		case CMD_IN:
			cout << "登录 返回结果：" << get_result(dp_rs->result) << " 返回信息：" << dp_rs->msg << endl;
			break;
		case CMD_OUT:
			cout << "登出 返回结果：" << get_result(dp_rs->result) << " 返回信息：" << dp_rs->msg << endl;
			break;
		case CMD_JOIN:
			cout << "加入 返回结果：" << get_result(dp_rs->result)<<" socket id="<< dp_HD->sock_id << " 返回信息：" << dp_rs->msg << endl;
			break;
		case CMD_UNK:
			cout << "未知 返回结果：" << get_result(dp_rs->result) << " 返回信息：" << dp_rs->msg << endl;
			break;
		default:
			break;
	}

	

	return 0;
}

string get_result(int r)
{
	switch (r)
	{
	case RES_TRUE:
		return "通过";
		break;
	case RES_FALSE:
		return "禁止";
		break;
	case RES_ERROR:
		return "错误";
		break;
	case RES_UNK:
		return "未知";
		break;
	default:
		return "NULL";
		break;
	}
}

void Get_input_information(SOCKET sock)
{
	while (isRuning)
	{
		char str[128] = { 0 };
		scanf_s("%s", str, 128);
		DP_SEND dp{};
		if (strcmp(str, "in") == 0)
		{
			dp.command = CMD_IN;
		}
		else if (strcmp(str, "out") == 0)
		{
			dp.command = CMD_OUT;
		}
		else if (strcmp(str, "exit") == 0)
		{
			cout << "程序退出！" << endl;
			isRuning = false;
			return;
		}
		else
		{
			dp.command = CMD_UNK;
		}
		int res = send(sock, (const char*)&dp, sizeof(DP_SEND), 0);
		if (res == SOCKET_ERROR)
		{
			cout << "send failed with error : " << WSAGetLastError() << endl;
			isRuning = false;
			return;
		}
		else
		{
			cout << "send  success." << endl;
		}
	}
}