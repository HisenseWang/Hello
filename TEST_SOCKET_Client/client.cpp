#define WIN32_LEAN_AND_MEAN  //防止 Windows.h 与 WinSock2.h 宏定义冲突 ，或调换两个头文件顺序
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //不显示警告信息 
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h>
using namespace std;

//#pragma comment(lib,"ws2_32.lib") //连接静态库 ,可以添加到属性连接器中的附加依赖
//客户端：



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
	int iResult=WSAStartup(ver, &wsadata); //启动SOCKET连接
	if (iResult != NO_ERROR)
	{
		cout << "WSAStartup function failed with error: " << iResult << endl;
	}
	
		
		
	//	1：建立一个socket
	SOCKET _sock_server= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock_server == INVALID_SOCKET)
	{
		cout << "socket function failed with error = " << WSAGetLastError() << endl;
		return 1;
	}
	else
	{
		cout << "socket function  success" << endl;
	}
	//	2：连接服务器 connect
	char* strip = (char *)("127.0.0.1");
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
    iResult=connect(_sock_server, (SOCKADDR*)&_sock_addr, sizeof(SOCKADDR_IN));
	if (iResult == SOCKET_ERROR)
	{
		cout << "connect function failed with error: " << WSAGetLastError() << endl;
		iResult = closesocket(_sock_server);
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

	
	{
		//可选
		char* sendbuf =(char*) ("this is a test");
		iResult = send(_sock_server, sendbuf, strlen(sendbuf), 0);//发送数据
		if (iResult == SOCKET_ERROR) 
		{
			cout << "send failed: " << WSAGetLastError() << endl;
			closesocket(_sock_server);
			WSACleanup();
			return 1;
		}
		cout << "Bytes Sent : " << iResult << endl;
		/*
		   SD_RECEIVE (0): 关机接收操作
		   SD_SEND	  (1): 关闭发送操作
		   SD_BOTH    (2): 关闭发送和接收操作	
		*/
		iResult = shutdown(_sock_server, SD_SEND);//关闭发送操作
		if (iResult == SOCKET_ERROR) 
		{
			cout << "shutdown failed: " << WSAGetLastError() << endl;
			closesocket(_sock_server);
			WSACleanup();
			return 1;
		}

	}

	//	3：接受客户端消息 recv
	do
	{
		char buf[256] = { 0 };
		iResult =recv(_sock_server, buf, sizeof(buf), 0);
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
			cout << "recv failed : "<< WSAGetLastError() << endl;
		}
		
	} while (iResult > 0);
	

	//	4：关闭套接字 closesocket
	closesocket(_sock_server);
	WSACleanup();//关闭SOCKET连接  清除 windows socket 环境
	return 0;

	/**
	
	
	
	***/
}