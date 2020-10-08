/*
When using Windows...
Warning: This header shoud be included before ANY other header exeptly windows.h!!!
Or the behavior of these codes are undefined!!!(exceptly select function)
*/

//You should add this pre-compile command before use this header:
//#pragma comment(lib, "ws2_32.lib")


#ifndef __WELKINNETWORK_H_
#define __WELKINNETWORK_H_

#ifdef WIN32

#ifndef _WINSOCKAPI_

#define FD_SETSIZE 1024//Max Select Count: Change it if you need!

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cassert>

#endif

#else
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#endif
//ƽ̨���ݶ������

//#include <exception>
#include "WelkinException.h"

#define BUF_SIZE 1024

namespace WelkinEngine
{
	namespace Network
	{
		typedef unsigned char u8_t;
		typedef char i8_t;
		typedef u8_t Element;
		typedef Element element;//used by OpCode(if exist)
		typedef unsigned short port_t;//May wrong?

		/*WSA��ʼ�����룬ƽ̨������
		* ���ۺ�ʱ��Ӧ�����ȵ��ô�
		* ��Linux�²����κ���
		*/
		inline void EnvironmentInit();

		/*WSA����ú�����ƽ̨������
		* ���ۺ�ʱ��Ӧ�������ô�
		* ��Linux�²����κ���
		*/
		inline void EnvironmentExit();

		/*��SocketAPI�л�ȡ����
		* Ϊ��ӦWinSock��BSDSocket��ʹ��
		*/
		int GetError();

		/*
		�����쳣���࣬�̳����Զ����쳣��
		*/
		class NetworkException : public Exception
		{
		public:
			NetworkException(int err);

			NetworkException(string error = "Unknown Error");

			NetworkException(string error, int p_line, string p_file = "", string info = "");

			virtual int LookUp(int error_code);
		};//End of class NetworkException

		class Socket
		{
		public:
			//virtual ~Socket() {};
			/*
			��������ȡ���׽��ֱ�ʶ
			*/
			SOCKET GetSock();
			/*
			��������ȡ���׽��ֶ˿�
			*/
			unsigned short GetPort();
			/*
			�������ر��׽���
			�������رշ�����Ĭ��Ϊ2����ȫ�ر��׽��֣�
			*/
			virtual void Close(int type = 2);

			/*
			��������������
			*/
			void SetBlocking(bool mode);

			bool IsReady();

			/*�������󶨵�ַ�Ͷ˿�
			*/
			void Bind(const char* p_ip_addr, port_t p_port);
		protected:
			/*���췽��
			* �����հ��׽����Ҳ���ʼ��
			* ���Ƽ�
			*/
			Socket();

			/*���췽��
			* ��������ָ�����͵��׽���
			*/
			Socket(int p_af, int p_type, int p_proto);

			/*
			* ���췽��
			* �����׽��ֲ���
			*/
			Socket(int p_af, int p_type, int p_proto, char* p_ip_addr, port_t p_port);

			SOCKET m_sock;
			sockaddr_in m_local_address;
			bool is_blocking;
		};//End of class Socket

		/*
		�����׽��֣��࣬�̳��Ի����׽�����
		ʹ��TCP/IPv4
		*/
		class DataSocket : public Socket
		{
		public:
			DataSocket(bool p_blocking = false);

			DataSocket(SOCKET& p_sock);

			DataSocket(DataSocket& p_sock);

			DataSocket(string p_addr, port_t p_port, bool p_blocking = false);

			~DataSocket();

			/*
			�����������׽���
			������Ҫ���ܵ��׽���
			��ע��������µ�ַ���˿���Ϣ
			*/
			void SetSocket(SOCKET p_sock);

			/*
			��������fd���������Ϣ��������
			�ڸ��°��׽��ֺ���ʼ�յ��ô˷���
			*/
			void UpdateInfo();

			/*
			���������ӵ�ָ����ַ�˿�
			������ָ����ַ���˿�
			*/
			void Connect(string p_addr, port_t p_port);

			/*
			* ��������������
			* ��������Ϣ������λ�ã����ݴ�С���ֽ�
			* �쳣��NetworkException
			*/
			int Send(char* msg, int count);

			/*
			��������������
			��������Ϣ���ַ�����
			����ֵ�����������Ϊ0
			*/
			//int Send(string msg);

			/*
			��������������
			���������������ַ�����
			����ֵ�����������Ϊ0
			*/
			//int Recv(string& buffer);

			
			/*
			��������������
			����������������������С
			����ֵ�����ܵ������ݴ�С
			*/
			int Recv(char* buffer, int size);

			protected:
			sockaddr_in m_remote_addr;//Զ����������
		};//End of class DataSocket

		/*
		�����׽��֣��࣬�̳��Ի����׽�����
		�ṩTCP/IP��������
		*/
		class ListeningSocket : public Socket
		{
		public:
			ListeningSocket();

			ListeningSocket(bool p_block);

			ListeningSocket(string p_addr, port_t p_port, bool p_block = false);
			//ListeningSocket(ListeningSocket& T) {};

			/*
			���������׽�����ָ���˿��ϼ���(Need Update
			��������ַ���˿�
			*/
			void ListenOn(string p_addr, port_t p_port, int p_max_queue_length = 32);

			/*���������׽��ְ��󶨶˿���������
			* ��Ҫ�Ѿ��ɹ�����Bind()
			*/
			void Listen(int max_queue_length = 32);

			/*
			���ܶ����еĵ�һ����������
			����ֵ��һ���µ�DataSocketʵ��
			�����ǣ���̬�ڴ���䣬ע���ڴ�й©��
			*/
			class DataSocket* Accept();

		protected:
			bool m_listening;
		};//End of class ListeningSocket

		class Protocol {
		public:
			/*
			* ��������,��msg����ȡԭʼ���ݲ�д��
			* ����ֵΪ���������ݳ���
			* ��Ϊ0��ʾ���������ݲ��ɷ���
			*/
			virtual int ProcessSendMsg(char* msg, int length) = 0;

			/*
			* ������ܵ�������
			* �����ݽ��������ɴ���
			* ����������Ϸ���0
			*/
			virtual int ProcessRecvMsg(char* msg, int length) = 0;
		};

		/*
		ͳһ������ϵͳ�����Զ���Э����׽�����󶨣��׽���ʹ��TCP/IP
		*/
		class Connection
		{
		protected:
			DataSocket* m_sock;
			Protocol* m_proto;
		public:
			~Connection();

			Connection(Connection& p_conn);

			/*
			* ���췽����������ϵͳ�ܿ������׽���
			* ���׽��ֺ�Э��ķ��ʱ����ռ
			* ��Զ��Ӧ�ý�һ���׽��ֻ�Э������������ӻ��ֶ�����
			*/
			Connection(DataSocket* p_sock, Protocol* p_proto);

			/*
			* ��������
			*/
			int Recv();

			/*
			* ��������
			* ���ݽ����Ƚ���Protocol������ٷ���
			*/
			int Send(char* msg, int length);

			/*
			* ����Э��,ֱ�ӷ��ͻ�������ָ������
			*/
			int SendDirectly(char* msg, int length);

			void Close();
		};//End of class Connection
	}//End of namespace WelkinEngine
}//End of namespace WelkinEngine

#endif
