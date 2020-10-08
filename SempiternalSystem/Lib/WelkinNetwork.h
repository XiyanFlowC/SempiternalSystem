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
//平台兼容定义结束

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

		/*WSA初始化代码，平台兼容用
		* 无论何时都应该首先调用此
		* 在Linux下不做任何事
		*/
		inline void EnvironmentInit();

		/*WSA清空用函数，平台兼容用
		* 无论何时都应当最后调用此
		* 在Linux下不做任何事
		*/
		inline void EnvironmentExit();

		/*从SocketAPI中获取错误
		* 为适应WinSock与BSDSocket而使用
		*/
		int GetError();

		/*
		网络异常，类，继承自自定义异常类
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
			方法，获取此套接字标识
			*/
			SOCKET GetSock();
			/*
			方法，获取此套接字端口
			*/
			unsigned short GetPort();
			/*
			方法，关闭套接字
			参数，关闭方法，默认为2（完全关闭套接字）
			*/
			virtual void Close(int type = 2);

			/*
			方法，设置阻塞
			*/
			void SetBlocking(bool mode);

			bool IsReady();

			/*方法，绑定地址和端口
			*/
			void Bind(const char* p_ip_addr, port_t p_port);
		protected:
			/*构造方法
			* 创建空白套接字且不初始化
			* 不推荐
			*/
			Socket();

			/*构造方法
			* 创建具有指定类型的套接字
			*/
			Socket(int p_af, int p_type, int p_proto);

			/*
			* 构造方法
			* 创建套接字并绑定
			*/
			Socket(int p_af, int p_type, int p_proto, char* p_ip_addr, port_t p_port);

			SOCKET m_sock;
			sockaddr_in m_local_address;
			bool is_blocking;
		};//End of class Socket

		/*
		数据套接字，类，继承自基本套接字类
		使用TCP/IPv4
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
			方法，设置套接字
			参数，要接受的套接字
			备注，不会更新地址及端口信息
			*/
			void SetSocket(SOCKET p_sock);

			/*
			方法，从fd更新相关信息到此类中
			在更新绑定套接字后请始终调用此方法
			*/
			void UpdateInfo();

			/*
			方法，连接到指定地址端口
			参数，指定地址，端口
			*/
			void Connect(string p_addr, port_t p_port);

			/*
			* 方法，发送数据
			* 参数，信息缓冲区位置，数据大小（字节
			* 异常：NetworkException
			*/
			int Send(char* msg, int count);

			/*
			方法，发送数据
			参数，信息（字符串）
			返回值，正常情况下为0
			*/
			//int Send(string msg);

			/*
			方法，接受数据
			参数，缓冲区（字符串）
			返回值，正常情况下为0
			*/
			//int Recv(string& buffer);

			
			/*
			方法，接受数据
			参数，缓冲区，缓冲区大小
			返回值，接受到的数据大小
			*/
			int Recv(char* buffer, int size);

			protected:
			sockaddr_in m_remote_addr;//远端连接数据
		};//End of class DataSocket

		/*
		监听套接字，类，继承自基本套接字类
		提供TCP/IP连接请求
		*/
		class ListeningSocket : public Socket
		{
		public:
			ListeningSocket();

			ListeningSocket(bool p_block);

			ListeningSocket(string p_addr, port_t p_port, bool p_block = false);
			//ListeningSocket(ListeningSocket& T) {};

			/*
			方法，令套接字在指定端口上监听(Need Update
			参数，地址，端口
			*/
			void ListenOn(string p_addr, port_t p_port, int p_max_queue_length = 32);

			/*方法，令套接字按绑定端口立即监听
			* 需要已经成功调用Bind()
			*/
			void Listen(int max_queue_length = 32);

			/*
			接受队列中的第一个连接请求
			返回值：一个新的DataSocket实例
			（谨记：动态内存分配，注意内存泄漏）
			*/
			class DataSocket* Accept();

		protected:
			bool m_listening;
		};//End of class ListeningSocket

		class Protocol {
		public:
			/*
			* 发送数据,从msg处获取原始数据并写回
			* 返回值为处理后的数据长度
			* 若为0表示现有数据暂不可发送
			*/
			virtual int ProcessSendMsg(char* msg, int length) = 0;

			/*
			* 处理接受到的数据
			* 将数据解析后自由处置
			* 正常处理完毕返回0
			*/
			virtual int ProcessRecvMsg(char* msg, int length) = 0;
		};

		/*
		统一的连接系统，对自定义协议和套接字相绑定，套接字使用TCP/IP
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
			* 构造方法，由连接系统管控所给套接字
			* 到套接字和协议的访问必须独占
			* 永远不应该将一个套接字或协议分配给多个连接或手动析构
			*/
			Connection(DataSocket* p_sock, Protocol* p_proto);

			/*
			* 接受数据
			*/
			int Recv();

			/*
			* 发送数据
			* 数据将首先交由Protocol处理后再发送
			*/
			int Send(char* msg, int length);

			/*
			* 无视协议,直接发送缓冲区中指定数据
			*/
			int SendDirectly(char* msg, int length);

			void Close();
		};//End of class Connection
	}//End of namespace WelkinEngine
}//End of namespace WelkinEngine

#endif
