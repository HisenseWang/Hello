#define WIN32_LEAN_AND_MEAN  //��ֹ Windows.h �� WinSock2.h �궨���ͻ �����������ͷ�ļ�˳��
#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
using namespace std;

//#pragma comment(lib,"ws2_32.lib") //���Ӿ�̬�� ,������ӵ������������еĸ�������

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
	WSAStartup(ver, &wsadata); //����SOCKET����

	/*
		����ˣ�
			1������һ�� socket
			2: bind �����ڽ��ܿͻ��ͻ����������ӵ�����˿�
			3��listen ��������˿�
			4��accept �ȴ����ܿͻ�������
			5��send ��ͻ��˷���һ������
			6���ر��׽��� closesocket
		�ͻ��ˣ�
			1������һ��socket
			2�����ӷ����� connect
			3�����ܿͻ�����Ϣ recv
			4���ر��׽��� closesocket

		
	*/



	WSACleanup();//�ر�SOCKET����  ��� windows socket ����
	return 0;
}