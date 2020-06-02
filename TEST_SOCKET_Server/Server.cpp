#define WIN32_LEAN_AND_MEAN  //��ֹ Windows.h �� WinSock2.h �궨���ͻ �����������ͷ�ļ�˳��
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //����ʾ������Ϣ 
#include<iostream>
#include<vector>
#include<Windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h> //getaddrinfo() ����ͷ�ļ�
#include"DataPackage.h"
using namespace std;

#pragma warning(disable:6319)
//#pragma comment(lib,"ws2_32.lib") //���Ӿ�̬�� ,������ӵ������������еĸ�������

vector<SOCKET> GLO_Sock_Client;
DP_SEND dp_jion{}; //�㲥����

int  handle_request(SOCKET _socket_client);


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
		WSACleanup();//�ر�SOCKET����  ��� windows socket ����
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
	//localIP = inet_ntoa(*(struct in_addr*) * localHost.h_addr_list);

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
		closesocket(_socket_server);
		WSACleanup();//�ر�SOCKET����  ��� windows socket ����
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
		closesocket(_socket_server);
		WSACleanup();//�ر�SOCKET����  ��� windows socket ����
		return 1;
	}
	else
	{
		cout << "Listening on socket..." << endl;
	}

	
	SOCKET  _socket_client=INVALID_SOCKET;//Զ�̿ͻ������ֽ���Ϣ
	SOCKADDR_IN _client_addr;
	int addlen = sizeof(SOCKADDR_IN);
    
	

	
	while (true)
	{
		/*
			select����ȷ��һ�������׽��ֵ�״̬�������Ҫ���ȴ�ִ��ͬ��I/O�� (������ Socket)
			������
				 _In_ int nfds,   Windows�º��� ��UNIX��LINUXʹ�á� ����nfds����ֻ��Ϊ����Berkeley�����������׽��ּ��ݡ�
				 _Inout_opt_ fd_set FAR * readfds, һ����ѡָ�룬ָ��һ��Ҫ���ɶ��Ե��׽���
				 _Inout_opt_ fd_set FAR * writefds, ָ��Ҫ����д�Ե��׽��ּ��Ŀ�ѡָ�롣
				 _Inout_opt_ fd_set FAR * exceptfds, һ����ѡָ�룬ָ��һ��Ҫ��������׽��֡�
				 _In_opt_ const struct timeval FAR * timeout  ѡ��ȴ������ʱ�䣬��TIMEVAL�ṹ����ʽ�ṩ����timeout��������Ϊnull��ִ������������
			����ֵ��
					select��������׼���ò�������fd_set�ṹ�е��׽��־�������������ʱ�����ƹ����򷵻��㣬������������򷵻�SOCKET_ERROR���������ֵ��SOCKET_ERROR�������ʹ��WSAGetLastError�����ض��Ĵ�����롣
		*/
		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExp;	
	    TIMEVAL time{ 1,0};//{�룬����} ��ʱ

		FD_ZERO(&fdRead);//��ʼ������
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExp);

		FD_SET(_socket_server, &fdRead);//��sock����Ҫ���Ե��������� ����˵��sock������rfd���� ������һ������select��rfd���в��Ե�ʱ��ͻ����sock��(��Ϊ���ǽ�sock�����rdf) һ�������������԰�����������Ե�������,
		FD_SET(_socket_server, &fdWrite);
		FD_SET(_socket_server, &fdExp);

		for (int i = (int)(GLO_Sock_Client.size()-1); i >=0 ; i--)//�������ϲ��ҿ���Socket
		{
			FD_SET(GLO_Sock_Client.at(i), &fdRead);
		}

			
		//��һ������ nfds ��һ������ֵ����ָ�� fd_set ���������е�socket �ķ�Χ���������������� windows ��ϵͳ�Զ������� ����ϵͳ��UNIX��LINUX��������Ҫָ����
		error = select(_socket_server+1,&fdRead,&fdWrite,&fdExp, &time);
		if (error == SOCKET_ERROR)
		{
			cout << "select function failed with error = " << WSAGetLastError() << endl;
			closesocket(_socket_server);
			WSACleanup();//�ر�SOCKET����  ��� windows socket ����
			return 1;
		}


		if (FD_ISSET(_socket_server, &fdRead))//��������socket���Ƿ��п������ݣ��Ƿ�׼������
		{
			FD_CLR(_socket_server, &fdRead);//������ݣ�����ʹ��
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
				cout << "��Ч�Ŀͻ��ˣ�accept failed with error = " << WSAGetLastError() << endl;
				closesocket(_socket_server);
				WSACleanup();//�ر�SOCKET����  ��� windows socket ����
				return 1;
			}
			
			for (int i = (int)(GLO_Sock_Client.size()-1); i >=0; i--)
			{
				dp_jion.dh.command = CMD_JION;
				dp_jion.res.result = RES_TRUE;
				strcpy_s(dp_jion.res.msg, "new client jion !");
				dp_jion.dh.dataLength = sizeof(DP_SEND);
				send(GLO_Sock_Client.at(i), (const char*)&dp_jion, sizeof(DP_SEND), 0);
			}

			//�ж���ͻ��˼��룬fdRead ����Ҫȥ���������ͻ��ˣ���������洢���ӵĿͻ���
			GLO_Sock_Client.push_back(_socket_client);
			char _ip_client[20]{ 0 };
			inet_ntop(AF_INET, (void*)&_client_addr.sin_addr, _ip_client, 16);//ת��IP��ַΪ�ַ���
			cout << "Client connected��" << _ip_client << " : " << _client_addr.sin_port << endl;
			
		}

		

		//��������������������
		for (size_t i = 0; i < fdRead.fd_count; i++)
		{
			int res=handle_request(fdRead.fd_array[i]);
			if (-1 == res)
			{
				auto iter = find(GLO_Sock_Client.begin(), GLO_Sock_Client.end(), fdRead.fd_array[i]);
				if (iter != GLO_Sock_Client.end())
				{
					GLO_Sock_Client.erase(iter);
				}
			}
		}
		
		cout << "��������ҵ���߼� ... ..." << endl;
	}
	

	


	//�ڰ˲����ر��׽��� closesocket
	for (size_t i = 0; i < GLO_Sock_Client.size(); i++)//�������ϲ��ҿ���Socket
	{
		closesocket(GLO_Sock_Client.at(i));
	}
	closesocket(_socket_server);
	WSACleanup();//�ر�SOCKET����  ��� windows socket ����
	return 0;
}

int  handle_request(SOCKET _socket_client)
{
	SOCKADDR_IN _client_addr;
	int len = sizeof(SOCKADDR_IN);
	int port = 0;
	//���岽�����ܿͻ�����Ϣ
	USER user{}; // todo. ��ȡ�û�����������жԱ� ,���ݿ��������ʽ
	//DP_RECV recdp{};//���ܻ�����
	char recvbuf[1024] = { 0 };//�������ݻ����� ���в������
	DP_SEND senddp{};//���ͻ�����
	int recvlen = recv(_socket_client, recvbuf, sizeof(DP_RECV), 0);
	DataHeader* dp_HD = (DataHeader*)recvbuf; //�������ݽṹת��
	if (recvlen <= 0)
	{
		cout << "Accept data length : [" << recvlen <<"]  "<< "client program exit ." << endl;
		return -1;
	}

	char _ip_client[20]{ 0 };
	if (!getpeername(_socket_client, (SOCKADDR*)&_client_addr, &len))//��ȡIP�Ͷ˿�
	{
		inet_ntop(AF_INET, (void*)&_client_addr.sin_addr, _ip_client, 16);//ת��IP��ַΪ�ַ���
		port = ntohs(_client_addr.sin_port);
	}
	
	cout <<"���� IP��" << _ip_client<<" : ["<< port<<"]  "<<
		"���ĳ��ȣ�[" << dp_HD->dataLength << "]  �������[" << dp_HD->command <<"]"<< endl;
	UserInfo* dp_UF = (UserInfo*)(recvbuf + sizeof(DataHeader));//ָ��ƫ��
	switch (dp_HD->command)
	{
	case CMD_IN:
		if (strcmp(dp_UF->userName, user.userName) == 0 && strcmp(dp_UF->passWord, user.passWord) == 0)
		{
			senddp.dh.command = dp_HD->command;
			senddp.dh.dataLength = sizeof(DP_SEND);
			senddp.res.result = RES_TRUE;
			strcpy_s(senddp.res.msg, "login success");
		}
		else
		{
			senddp.dh.command = dp_HD->command;
			senddp.dh.dataLength = sizeof(DP_SEND);
			senddp.res.result = RES_ERROR;
			strcpy_s(senddp.res.msg, "login failure");
		}
		break;
	case CMD_OUT:
		if (strcmp(dp_UF->userName, user.userName) == 0 && strcmp(dp_UF->passWord, user.passWord) == 0)
		{
			senddp.dh.command = dp_HD->command;
			senddp.dh.dataLength = sizeof(DP_SEND);
			senddp.res.result = RES_FALSE;
			strcpy_s(senddp.res.msg, "logout success");
		}
		else
		{
			senddp.dh.command = dp_HD->command;
			senddp.dh.dataLength = sizeof(DP_SEND);
			senddp.res.result = RES_ERROR;
			strcpy_s(senddp.res.msg, "logout failure");
		}
		break;
	default:
		senddp.dh.command = dp_HD->command;
		senddp.dh.dataLength = sizeof(DP_SEND);
		senddp.res.result = RES_UNK;
		strcpy_s(senddp.res.msg, "An unknown request");
		break;
	}

	//���߲���send ��ͻ��˷���һ������
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
	int error = send(_socket_client, (const char*)(&senddp), sizeof(DP_SEND), 0);
	if (error == SOCKET_ERROR)
	{
		cout << "send failed with error : " << WSAGetLastError() << endl;
		return -1;
	}
	else
	{
		cout << "send  success." << endl;
	}

	return 0;
}