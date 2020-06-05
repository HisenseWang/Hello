#define WIN32_LEAN_AND_MEAN  //��ֹ Windows.h �� WinSock2.h �궨���ͻ �����������ͷ�ļ�˳��
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //����ʾ������Ϣ 
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h>
#include<thread>
#include"DataPackage.h"
using namespace std;

#pragma warning(disable:6319)//�������ʽ��ʹ�ö���������ľ���
//#pragma comment(lib,"ws2_32.lib") //���Ӿ�̬�� ,������ӵ������������еĸ�������
//�ͻ��ˣ�


int  handle_request(SOCKET _socket); //���ݽ���
string get_result(int r);
void Get_input_information(SOCKET sock);

bool isRuning = true;

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
	int iResult = WSAStartup(ver, &wsadata); //����SOCKET����
	if (iResult != NO_ERROR)
	{
		cout << "WSAStartup function failed with error: " << iResult << endl;
	}



	//	1������һ��socket
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
	//	2�����ӷ����� connect
	char* strip = (char*)("127.0.0.1");
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
	//ѭ�����������
	while (true)
	{
		fd_set fdRead;
		FD_ZERO(&fdRead);//��ʼ������
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
				cout << "����ͻ�����Ϣ ���󣡳��������" << endl;
				break;
			}
		}

		if (!isRuning)
		{
			break;
		}

	}


	//	{
	//		//��ѡ
	//		//char* sendbuf =(char*) ("this is a test");
	//		iResult = send(_socket_server, sendbuf, strlen(sendbuf), 0);//��������
	//		if (iResult == SOCKET_ERROR)
	//		{
	//			cout << "send failed: " << WSAGetLastError() << endl;
	//			closesocket(_socket_server);
	//			WSACleanup();
	//			return 1;
	//		}
	//		cout << "Bytes Sent : " << iResult << endl;
	//		/*
	//		   SD_RECEIVE (0): �ػ����ղ���
	//		   SD_SEND	  (1): �رշ��Ͳ���
	//		   SD_BOTH    (2): �رշ��ͺͽ��ղ���
	//		*/
	//	iResult = shutdown(_socket_server, SD_SEND);//�رշ��Ͳ���
	//	if (iResult == SOCKET_ERROR)
	//	{
	//		cout << "shutdown failed: " << WSAGetLastError() << endl;
	//		closesocket(_socket_server);
	//		WSACleanup();
	//		return 1;
	//	}
	//
	//}

	//   //	3�����ܿͻ�����Ϣ recv
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


	 //	4���ر��׽��� closesocket
	closesocket(_socket_server);
	WSACleanup();//�ر�SOCKET����  ��� windows socket ����
	return 0;

}

int  handle_request(SOCKET _socket_client)
{
	//���岽�����ܿͻ�����Ϣ
	
	//DP_RECV recdp{};//���ܻ�����
	char recvbuf[1024] = { 0 };//�������ݻ����� ���в������
	int recvlen = recv(_socket_client, recvbuf, sizeof(DP_RECV), 0);
	DataHeader* dp_HD = (DataHeader*)recvbuf; //�������ݽṹת��
	if (recvlen <= 0)
	{
		cout << "����� �� Accept data length : [" << recvlen << "]  " << "server program exit ." << endl;
		return -1;
	}
	cout << "���ĳ��ȣ�[" << dp_HD->dataLength << "]  �������[" << dp_HD->command << "]" << endl;
	RES* dp_rs = (RES*)(recvbuf + sizeof(DataHeader));//ָ��ƫ��
	switch (dp_HD->command)
	{
		case CMD_IN:
			cout << "��¼ ���ؽ����" << get_result(dp_rs->result) << " ������Ϣ��" << dp_rs->msg << endl;
			break;
		case CMD_OUT:
			cout << "�ǳ� ���ؽ����" << get_result(dp_rs->result) << " ������Ϣ��" << dp_rs->msg << endl;
			break;
		case CMD_JOIN:
			cout << "���� ���ؽ����" << get_result(dp_rs->result)<<" socket id="<< dp_HD->sock_id << " ������Ϣ��" << dp_rs->msg << endl;
			break;
		case CMD_UNK:
			cout << "δ֪ ���ؽ����" << get_result(dp_rs->result) << " ������Ϣ��" << dp_rs->msg << endl;
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
		return "ͨ��";
		break;
	case RES_FALSE:
		return "��ֹ";
		break;
	case RES_ERROR:
		return "����";
		break;
	case RES_UNK:
		return "δ֪";
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
			cout << "�����˳���" << endl;
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