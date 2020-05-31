#define WIN32_LEAN_AND_MEAN  //��ֹ Windows.h �� WinSock2.h �궨���ͻ �����������ͷ�ļ�˳��
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //����ʾ������Ϣ 
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h> //getaddrinfo() ����ͷ�ļ�
using namespace std;

//#pragma comment(lib,"ws2_32.lib") //���Ӿ�̬�� ,������ӵ������������еĸ�������



int main(void)
{

	WORD ver = MAKEWORD(2, 2);//�����汾��  MAKEWORD �����汾�ź궨��
	WSADATA wsadata{}; //�ṹ��

	/*
	 WSAStartup
	 �˺�����Ӧ�ó����г�ʼ��winsockDLL
	 ������  _In_ WORD wVersionRequested �� ���ó���ʹ��windows socket����߰汾�� ���ֽ�ָ��С�İ汾�ţ����ֽ�ָ���ߵİ汾�š�
			_Out_ LPWSADATA lpWSAData �� ָ��WSADATA���ݽṹ��ָ�룬����Windows Socket��ʵ��ϸ�� ���� Windows ���� LP��ͷ�Ĳ���������������;���ȥ��LP��Ĳ������������� LPWSADATA ��Ӧ WSADATA
	 ����ֵ������ɹ���WSAStartup��������0��

	*/
	WSAStartup(ver, &wsadata); //����SOCKET����

	/*
		����ˣ�
			1������һ�� socket
			2: bind �����ڽ��ܿͻ��ͻ����������ӵ�����˿�
			3��listen ��������˿�
			4��accept �ȴ����ܿͻ�������
			5��send ��ͻ��˷���һ������
			6���ر��׽��� closesocket
	
	*/

	/*
	  socket ����socket
	  ������ _In_ int af ��InternetЭ��汾��ַ�壺AF_INET= IPV4
			_In_ int type �������������� ���׽��ֵ����͹淶��SOCK_STREAM = ������
			_In_ int protocol��Э������ IPPROTO_TCP = TCP/IP
	  ����ֵ��
			���û�д��������� �׽��ַ����������׽��ֵ������������򣬽�����INVALID_SOCKET��ֵ�����ҿ���ͨ������WSAGetLastError�������ض��Ĵ������ ��
	*/ 
	//��һ�� ����һ�� socket
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

	//��ȡ����������Ϣ
	//hostent* localHost=nullptr;//hostent�ṹ���ڲ���������������������IP��ַ�б������
	//char* localIP;
	//localHost = gethostbyname(""); //gethostbyname ����Ϊһ������ ֻ��IPV4 �������á� ʹ�� getaddrinfo�����������棬����һ��ָ��hostent ��ָ�� 
	//
	//localIP = inet_ntoa(*(struct in_addr*) * localHost->h_addr_list);

	/*
		SOCKADDR_IN �ṹ�壺

	*/
	SOCKADDR_IN _sock_addr{}; // SOCKADDR �� sockaddr_in �ṹ�����ƣ����� sockaddr_in �ṹ�������ֵ������
	_sock_addr.sin_family = AF_INET;//��ַ������
	_sock_addr.sin_port = htons(4567);//�˿�
	_sock_addr.sin_addr.S_un.S_addr = INADDR_ANY;// inet_addr(localIP);//������ַ����ͬ����ֵ 1��INADDR_ANY  2: inet_addr("127.0.0.1")

	//�ڶ��� bind �����ڽ��ܿͻ��ͻ����������ӵ�����˿�
	/*
	  bind �� �󶨹�������������׽��ֱ��ص�ַ
	  ������ _In_ SOCKET s   ��ʶδ���׽��ֵ�������
		    _In_reads_bytes_(namelen) const struct sockaddr FAR * name  ָ��Ҫ��������׽��ֵı��ص�ַ��sockaddr�ṹ��ָ��
			_In_ int namelen  ���Ʋ�����ָ���ֵ�ĳ��ȣ����ֽ�Ϊ��λ��
	  ����ֵ��
			���û�д��������� �󶨷����㡣������������SOCKET_ERROR�����ҿ���ͨ������WSAGetLastError�������ض��Ĵ������ ��
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

	//��������listen ��������˿�
	/*
		listen ��������˿�
		������ _In_ SOCKET s  ��ʶ�󶨵�δ�����׽��ֵ�������
			  _In_ int backlog ��������Ӷ��е���󳤶ȡ��������ΪSOMAXCONN�������׽���s�Ļ��������ṩ�̽���ѹ����Ϊ������ֵ���������ΪSOMAXCONN_HINT��N��������NΪ���֣������ѹֵ��ΪN��������Ϊ���ڣ�200��65535����Χ�ڡ���ע�⣬SOMAXCONN_HINT�����ڽ���ѹ����Ϊ��SOMAXCONN���ܸ����ֵ����Microsoft TCP / IP�����ṩ��֧��SOMAXCONN_HINT��û�б�׼��������ȡʵ�ʻ�ѹֵ
		����ֵ 
				���û�д��������� listen�����㡣���򣬽�����SOCKET_ERROR��ֵ�����ҿ���ͨ������WSAGetLastError�������ض��Ĵ������ ��
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

	
	SOCKET  _socket_client{};//Զ�̿ͻ������ֽ���Ϣ
	SOCKADDR_IN _client_addr;
	int addlen = sizeof(SOCKADDR_IN);
	const char* sendbuf = (const char*)("Client: sending data test");
	while (true)
	{
		/*
		   accept:���ܿͻ�������
		   ������   _In_ SOCKET s   һ�������������ڱ�ʶ��ʹ������������������״̬���׽��� ��ʵ���ϣ�������ͨ��accept���ص��׽��ֽ�����
			       _Out_writes_bytes_opt_(*addrlen) struct sockaddr FAR * addr  ͨ�Ų���֪��ָ�򻺳����Ŀ�ѡָ�룬�û�������������ʵ��ĵ�ַ��addr������ȷ�и�ʽ�ɴ���sockaddr�ṹ���׽���ʱ�����ĵ�ַ��ȷ�� ��
			       _Inout_opt_ int FAR * addrlen  ָ�������Ŀ�ѡָ�룬����������addr����ָ��Ľṹ�ĳ���
	       ����ֵ��
				���û�з������� accept������SOCKET���͵�ֵ����ֵ�����׽��ֵ����������÷���ֵ��ʵ�ʽ������ӵ��׽��ֵľ����
				���򣬽�����INVALID_SOCKET��ֵ�����ҿ���ͨ������WSAGetLastError�������ض��Ĵ������ ��
				addrlen���õ������������addrָ��Ŀռ���������ʱ�������������ص�ַ��ʵ�ʳ��ȣ����ֽ�Ϊ��λ����

	    */
		//���Ĳ���accept �ȴ����ܿͻ�������
		_socket_client = accept(_socket_server, (SOCKADDR*)&_client_addr, &addlen);
		if (_socket_client == INVALID_SOCKET)
		{
			cout << "accept failed with error = " << WSAGetLastError() << endl;
			return 1;
		}
		else
		{
			char _ip_client[20]{ 0 };
			inet_ntop(AF_INET, (void*)&_client_addr.sin_addr, _ip_client, 16);//ת��IP��ַΪ�ַ���
			cout << "Client connected��"<< _ip_client <<" : "<<_client_addr.sin_port << endl;
		}

		//���岽��send ��ͻ��˷���һ������
		/*
		  send ��ͻ��˷���һ������:
		  ������
				_In_ SOCKET s      ��ʶ�������׽��ֵ�������
				_In_reads_bytes_(len) const char FAR * buf  ָ�����Ҫ�������ݵĻ�������ָ��
				_In_ int len    buf����ָ��Ļ����������ݵĳ��ȣ����ֽ�Ϊ��λ��
				_In_ int flags  һ���־��ָ�����к��еķ�ʽ��ͨ������λ��������������κ�ֵһ��ʹ��������˲���
					MSG_DONTROUTE��ָ����Ӧ�����ݽ���·�ɡ�Windows�׽��ַ����ṩ�������ѡ����Դ˱�־��
					MSG_OOB      ������OOB���ݣ���SOCK_STREAM֮�����ʽ�׽���
		  ����ֵ��
			   ���δ����������send�����ѷ��͵��ֽ������� ����������С��len�����������͵��ֽ��������򣬽�����SOCKET_ERROR��ֵ�����ҿ���ͨ������WSAGetLastError�������ض��Ĵ������
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
	

	


	//���������ر��׽��� closesocket
	closesocket(_socket_server);

	WSACleanup();//�ر�SOCKET����  ��� windows socket ����
	return 0;
}