#define WIN32_LEAN_AND_MEAN  //防止 Windows.h 与 WinSock2.h 宏定义冲突 ，或调换两个头文件顺序
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
using namespace std;

//#pragma comment(lib,"ws2_32.lib") //连接静态库 ,可以添加到属性连接器中的附加依赖

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
	WSAStartup(ver, &wsadata); //启动SOCKET连接

	/*
		服务端：
			1：建立一个 socket
			2: bind 绑定用于接受客户客户端用于连接的网络端口
			3：listen 监听网络端口
			4：accept 等待接受客户端连接
			5：send 向客户端发送一条数据
			6：关闭套接字 closesocket
		客户端：
			1：建立一个socket
			2：连接服务器 connect
			3：接受客户端消息 recv
			4：关闭套接字 closesocket

		
	*/



	WSACleanup();//关闭SOCKET连接  清除 windows socket 环境
	return 0;
}