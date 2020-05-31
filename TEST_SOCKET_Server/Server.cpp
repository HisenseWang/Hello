#define WIN32_LEAN_AND_MEAN  //防止 Windows.h 与 WinSock2.h 宏定义冲突 ，或调换两个头文件顺序
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //不显示警告信息 
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h> //getaddrinfo() 函数头文件
using namespace std;

//#pragma comment(lib,"ws2_32.lib") //连接静态库 ,可以添加到属性连接器中的附加依赖



int main(void)
{

	WORD ver = MAKEWORD(2, 2);//创建版本号  MAKEWORD 创建版本号宏定义
	WSADATA wsadata{}; //结构体

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
	
	*/

	/*
	  socket 创建socket
	  参数： _In_ int af ：Internet协议版本地址族：AF_INET= IPV4
			_In_ int type ：传输数据类型 ，套接字的类型规范：SOCK_STREAM = 数据流
			_In_ int protocol：协议类型 IPPROTO_TCP = TCP/IP
	  返回值：
			如果没有错误发生，则 套接字返回引用新套接字的描述符。否则，将返回INVALID_SOCKET的值，并且可以通过调用WSAGetLastError来检索特定的错误代码 。
	*/ 
	//第一步 建立一个 socket
	SOCKET _socket_server= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket_server == INVALID_SOCKET)
	{
		cout << "socket function failed with error = " << WSAGetLastError() << endl;
		return 1;
	}
	else
	{
		cout << "socket function  success" << endl;
	}

	//获取本地主机信息
	//hostent* localHost=nullptr;//hostent结构体内部包含了主机名、别名、IP地址列表等内容
	//char* localIP;
	//localHost = gethostbyname(""); //gethostbyname 参数为一个域名 只限IPV4 【已弃用】 使用 getaddrinfo（）函数代替，返回一个指向hostent 的指针 
	//
	//localIP = inet_ntoa(*(struct in_addr*) * localHost->h_addr_list);

	/*
		SOCKADDR_IN 结构体：

	*/
	SOCKADDR_IN _sock_addr{}; // SOCKADDR 和 sockaddr_in 结构体类似，但是 sockaddr_in 结构体参数赋值更方便
	_sock_addr.sin_family = AF_INET;//地址族类型
	_sock_addr.sin_port = htons(4567);//端口
	_sock_addr.sin_addr.S_un.S_addr = INADDR_ANY;// inet_addr(localIP);//本机地址，相同参数值 1：INADDR_ANY  2: inet_addr("127.0.0.1")

	//第二步 bind 绑定用于接受客户客户端用于连接的网络端口
	/*
	  bind 该 绑定功能与相关联的套接字本地地址
	  参数： _In_ SOCKET s   标识未绑定套接字的描述符
		    _In_reads_bytes_(namelen) const struct sockaddr FAR * name  指向要分配给绑定套接字的本地地址的sockaddr结构的指针
			_In_ int namelen  名称参数所指向的值的长度（以字节为单位）
	  返回值：
			如果没有错误发生，则 绑定返回零。否则，它将返回SOCKET_ERROR，并且可以通过调用WSAGetLastError来检索特定的错误代码 。
	*/
	int error =bind(_socket_server, (SOCKADDR*)&_sock_addr, sizeof(SOCKADDR_IN));
	if (error == SOCKET_ERROR)
	{
		cout << "bind failed with error = " << WSAGetLastError() << endl;
		return 1;
	}
	else
	{
		cout << "bind returned success" << endl;
	}

	//第三步：listen 监听网络端口
	/*
		listen 监听网络端口
		参数： _In_ SOCKET s  标识绑定的未连接套接字的描述符
			  _In_ int backlog 挂起的连接队列的最大长度。如果设置为SOMAXCONN，则负责套接字s的基础服务提供商将积压设置为最大合理值。如果设置为SOMAXCONN_HINT（N）（其中N为数字），则积压值将为N，并调整为介于（200，65535）范围内。请注意，SOMAXCONN_HINT可用于将积压设置为比SOMAXCONN可能更大的值。仅Microsoft TCP / IP服务提供商支持SOMAXCONN_HINT。没有标准条款来获取实际积压值
		返回值 
				如果没有错误发生，则 listen返回零。否则，将返回SOCKET_ERROR的值，并且可以通过调用WSAGetLastError来检索特定的错误代码 。
	*/
	error=listen(_socket_server, 10);
	if (error == SOCKET_ERROR)
	{
		cout << "listen function failed with error = " << WSAGetLastError() << endl;
		return 1;
	}
	else
	{
		cout << "Listening on socket..." << endl;
	}

	
	SOCKET  _socket_client{};//远程客户端套字节信息
	SOCKADDR_IN _client_addr;
	int addlen = sizeof(SOCKADDR_IN);
	const char* sendbuf = (const char*)("Client: sending data test");
	while (true)
	{
		/*
		   accept:接受客户端连接
		   参数：   _In_ SOCKET s   一个描述符，用于标识已使用侦听功能置于侦听状态的套接字 。实际上，连接是通过accept返回的套接字建立的
			       _Out_writes_bytes_opt_(*addrlen) struct sockaddr FAR * addr  通信层已知的指向缓冲区的可选指针，该缓冲区接收连接实体的地址。addr参数的确切格式由创建sockaddr结构的套接字时建立的地址族确定 。
			       _Inout_opt_ int FAR * addrlen  指向整数的可选指针，该整数包含addr参数指向的结构的长度
	       返回值：
				如果没有发生错误， accept将返回SOCKET类型的值，该值是新套接字的描述符。该返回值是实际建立连接的套接字的句柄。
				否则，将返回INVALID_SOCKET的值，并且可以通过调用WSAGetLastError来检索特定的错误代码 。
				addrlen引用的整数最初包含addr指向的空间量。返回时，它将包含返回地址的实际长度（以字节为单位）。

	    */
		//第四步：accept 等待接受客户端连接
		_socket_client = accept(_socket_server, (SOCKADDR*)&_client_addr, &addlen);
		if (_socket_client == INVALID_SOCKET)
		{
			cout << "accept failed with error = " << WSAGetLastError() << endl;
			return 1;
		}
		else
		{
			char _ip_client[20]{ 0 };
			inet_ntop(AF_INET, (void*)&_client_addr.sin_addr, _ip_client, 16);//转换IP地址为字符串
			cout << "Client connected："<< _ip_client <<" : "<<_client_addr.sin_port << endl;
		}

		//第五步，send 向客户端发送一条数据
		/*
		  send 向客户端发送一条数据:
		  参数：
				_In_ SOCKET s      标识已连接套接字的描述符
				_In_reads_bytes_(len) const char FAR * buf  指向包含要传输数据的缓冲区的指针
				_In_ int len    buf参数指向的缓冲区中数据的长度（以字节为单位）
				_In_ int flags  一组标志，指定进行呼叫的方式。通过将按位或运算符与以下任何值一起使用来构造此参数
					MSG_DONTROUTE：指定不应对数据进行路由。Windows套接字服务提供程序可以选择忽略此标志。
					MSG_OOB      ：发送OOB数据（仅SOCK_STREAM之类的流式套接字
		  返回值：
			   如果未发生错误，则send返回已发送的字节总数， 该总数可以小于len参数中请求发送的字节数。否则，将返回SOCKET_ERROR的值，并且可以通过调用WSAGetLastError来检索特定的错误代码
		*/

		error = send(_socket_client, sendbuf, (int)strlen(sendbuf), 0);
		if (error == SOCKET_ERROR)
		{
			cout << "send failed with error : " << WSAGetLastError() << endl;
			return 1;
		}
		else
		{
			cout << "send  success." << endl;
		}
	}
	

	


	//第六步：关闭套接字 closesocket
	closesocket(_socket_server);

	WSACleanup();//关闭SOCKET连接  清除 windows socket 环境
	return 0;
}