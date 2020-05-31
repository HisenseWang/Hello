#define WIN32_LEAN_AND_MEAN  //��ֹ Windows.h �� WinSock2.h �궨���ͻ �����������ͷ�ļ�˳��
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //����ʾ������Ϣ 
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h>
using namespace std;

//#pragma comment(lib,"ws2_32.lib") //���Ӿ�̬�� ,������ӵ������������еĸ�������
//�ͻ��ˣ�



int main(void)
{
	WORD ver = MAKEWORD(2, 2);//�����汾��  MAKEWORD �����汾�ź궨��
	WSADATA wsadata; //�ṹ��

	/*
	 WSAStartup
	 �˺�����Ӧ�ó����г�ʼ��winsockDLL
	 ������  _In_ WORD wVersionRequested �� ���ó���ʹ��windows socket����߰汾�� ���ֽ�ָ��С�İ汾�ţ����ֽ�ָ���ߵİ汾�š�
			_Out_ LPWSADATA lpWSAData �� ָ��WSADATA���ݽṹ��ָ�룬����Windows Socket��ʵ��ϸ�� ���� Windows ���� LP��ͷ�Ĳ���������������;���ȥ��LP��Ĳ������������� LPWSADATA ��Ӧ WSADATA
	 ����ֵ������ɹ���WSAStartup��������0��

	*/
	int iResult=WSAStartup(ver, &wsadata); //����SOCKET����
	if (iResult != NO_ERROR)
	{
		cout << "WSAStartup function failed with error: " << iResult << endl;
	}
	
		
		
	//	1������һ��socket
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
	//	2�����ӷ����� connect
	char* strip = (char *)("127.0.0.1");
	SOCKADDR_IN _sock_addr{};
	_sock_addr.sin_family = AF_INET;
	_sock_addr.sin_port = htons(4567);
	inet_pton(AF_INET, strip, &_sock_addr.sin_addr.S_un.S_addr);
	//_sock_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //inet_pton(AF_INET,"127.0.0.1" ,(void*)&_sock_addr.sin_addr.S_un.S_addr);

	/*
	  connect:
	    ������	 _In_ SOCKET s,  ��ʶδ�����׽��ֵ�������
				 _In_reads_bytes_(namelen) const struct sockaddr FAR * name,  ָ��Ӧ�������ӵ�sockaddr�ṹ��ָ��
				 _In_ int namelen   name������ָ���sockaddr�ṹ�ĳ��ȣ����ֽ�Ϊ��λ��
		����ֵ��
				���δ���������� connect�����㡣������������SOCKET_ERROR�����ҿ���ͨ������WSAGetLastError�������ض��Ĵ������ ��
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
		//��ѡ
		char* sendbuf =(char*) ("this is a test");
		iResult = send(_sock_server, sendbuf, strlen(sendbuf), 0);//��������
		if (iResult == SOCKET_ERROR) 
		{
			cout << "send failed: " << WSAGetLastError() << endl;
			closesocket(_sock_server);
			WSACleanup();
			return 1;
		}
		cout << "Bytes Sent : " << iResult << endl;
		/*
		   SD_RECEIVE (0): �ػ����ղ���
		   SD_SEND	  (1): �رշ��Ͳ���
		   SD_BOTH    (2): �رշ��ͺͽ��ղ���	
		*/
		iResult = shutdown(_sock_server, SD_SEND);//�رշ��Ͳ���
		if (iResult == SOCKET_ERROR) 
		{
			cout << "shutdown failed: " << WSAGetLastError() << endl;
			closesocket(_sock_server);
			WSACleanup();
			return 1;
		}

	}

	//	3�����ܿͻ�����Ϣ recv
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
	

	//	4���ر��׽��� closesocket
	closesocket(_sock_server);
	WSACleanup();//�ر�SOCKET����  ��� windows socket ����
	return 0;

	/**
	
	
	
	***/
}