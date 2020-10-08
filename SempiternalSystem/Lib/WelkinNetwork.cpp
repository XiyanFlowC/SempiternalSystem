#include "WelkinNetwork.h"

namespace WelkinEngine
{
	namespace Network
	{
		inline void EnvironmentInit()
		{
#ifdef _WIN32
			WSADATA data;
			if(WSAStartup(MAKEWORD(2, 2), &data)) throw NetworkException(GetError());
#endif
		}

		inline void EnvironmentExit()
		{
#ifdef _WIN32
			WSACleanup();
#endif
		}

		int GetError()
		{
#ifdef WIN32
			return WSAGetLastError();
#else
			return errno;
#endif
		}

		port_t Socket::GetPort()
		{
			return m_local_address.sin_port;
		}

		void Socket::Close(int type)
		{
			if (shutdown(m_sock, type)) throw(NetworkException(GetError()));
#ifdef WIN32
			if (closesocket(m_sock)) throw(NetworkException(GetError()));
#else
			if (close(m_sock)) throw NetworkException(GetError());
#endif
		}

		SOCKET Socket::GetSock()
		{
			return (*this).m_sock;
		}

		void Socket::SetBlocking(bool p_blocking)
		{
			is_blocking = p_blocking;
			unsigned long nb = !p_blocking;
#ifdef _WIN32
			if( ioctlsocket(m_sock, FIONBIO, &nb) ) throw NetworkException(GetError());
#else
#error "WelkinEngine::Socket::SetBlocking() Not Implement."
#endif
		}

		bool Socket::IsReady()
		{
			return m_sock != INVALID_SOCKET;
		}

		void Socket::Bind(const char* p_ip_addr, port_t p_port)
		{
			if (!IsReady()) throw NetworkException(WSAENOTSOCK);
			int ret = inet_pton(m_local_address.sin_family, p_ip_addr, &m_local_address.sin_addr.S_un.S_addr);
			if (ret == -1) throw NetworkException(GetError());
			assert(ret != 0);//这种问题调试时还是崩掉吧
			if (ret == 0) throw ParameterException("inet_pton() faild.", __LINE__, __FILE__, "Parameter addr_string doesn't a valid IP address description.");
			m_local_address.sin_port = htons(p_port);
			if (bind(m_sock, (sockaddr*)&m_local_address, sizeof(m_local_address))) throw NetworkException(GetError());
		}

		Socket::Socket()
		{
			m_sock = INVALID_SOCKET;
			memset(&m_local_address, 0, sizeof(m_local_address));
		}

		Socket::Socket(int p_af, int p_type, int p_proto)
		{
			m_sock = socket(p_af, p_type, p_proto);
			is_blocking = true;
			if (m_sock == INVALID_SOCKET) throw NetworkException(GetError());
			m_local_address.sin_family = p_af;
		}

		Socket::Socket(int p_af, int p_type, int p_proto, char* p_ip_addr, port_t p_port)
		{
			m_sock = socket(p_af, p_type, p_proto);
			if (m_sock == INVALID_SOCKET) throw NetworkException(GetError());
			m_local_address.sin_family = p_af;
			Bind(p_ip_addr, p_port);
		}

		void DataSocket::UpdateInfo()
		{
			int longi = sizeof(m_remote_addr);
			getpeername(m_sock, (sockaddr*)&m_remote_addr, &longi);
			longi = sizeof(m_local_address);
			getsockname(m_sock, (sockaddr*)&m_local_address, &longi);
		}

		void DataSocket::Connect(string p_addr, port_t p_port)
		{
			m_remote_addr.sin_family = m_local_address.sin_family;
			int ret = inet_pton(AF_INET, p_addr.c_str(), &m_remote_addr.sin_addr.S_un.S_addr);
			if (ret == -1) throw NetworkException(GetError());
			assert(ret != 0);//这种问题调试时还是崩掉吧
			if (ret == 0) throw ParameterException("inet_pton() faild.", __LINE__, __FILE__, "Parameter addr_string doesn't a valid IP address description.");
			m_remote_addr.sin_port = htons(p_port);
			if (connect(m_sock, (sockaddr*)&m_remote_addr, sizeof(m_remote_addr)))
				throw NetworkException(GetError());
		}

		int DataSocket::Send(char* msg, int count)
		{
			int ret = send(m_sock, msg, count, 0);
			if (ret == SOCKET_ERROR && GetError() != WSAEWOULDBLOCK)
				throw NetworkException(GetError());
			return ret;
		}

		DataSocket::DataSocket(bool p_blocking)
			: Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
		{
			SetBlocking(p_blocking);
			memset(&m_remote_addr, 0, sizeof(m_remote_addr));
		}

		DataSocket::DataSocket(SOCKET& p_sock)
		{
			m_sock = p_sock;
			UpdateInfo();
		}

		DataSocket::DataSocket(DataSocket& p_sock)
		{
			m_sock = p_sock.m_sock;
			m_local_address = p_sock.m_local_address;
			m_remote_addr = p_sock.m_remote_addr;
		}

		DataSocket::DataSocket(string p_addr, port_t p_port, bool p_blocking)
			: Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
		{
			SetBlocking(p_blocking);
			memset(&m_remote_addr, 0, sizeof(m_remote_addr));
		}

		DataSocket::~DataSocket()
		{
			Close(2);
		}

		void DataSocket::SetSocket(SOCKET p_sock)
		{
			m_sock = p_sock;
		}

		/*int DataSocket::Send(string msg)
		{
			int ret;
			if(SOCKET_ERROR == (ret = send(m_sock, msg.c_str(), msg.size(), 0))) 
			{
				int err = GetError();
				if(err != EWOULDBLOCK)
					throw(NetworkException(err));
			}
			return ret;
		}*/
		
		int DataSocket::Recv(char* buffer, int size)
		{
			int ret = recv(m_sock, buffer, size, 0);
			if(ret == SOCKET_ERROR && WSAEWOULDBLOCK !=  GetError()) throw NetworkException(GetError());
			return ret;
		}

		//int DataSocket::Recv(string& buffer)
		//{
		//	int ret;
		//	char buf[BUF_SIZE];//TO DO:缓冲区大小提高
		//	ret = recv(m_sock, buf, sizeof(buf), 0);
		//	if(ret < 0 && EWOULDBLOCK != (ret = GetError())) throw NetworkException(ret);
		//	if(ret == EWOULDBLOCK) return EWOULDBLOCK;
		//	buf[ret] = '\0';
		//	buffer = buf;
		//	return ret;
		//}

		inline NetworkException::NetworkException(int err)
		{
			m_error_code = err;
			LookUp(err);
		}
		
		inline NetworkException::NetworkException(string error)
		{
			m_error = error;
		}

		inline NetworkException::NetworkException(string error, int p_line, string p_file, string info)
		{
			m_error = error;
			line = p_line;
			file = p_file;
			m_info = info;
		}

		inline int NetworkException::LookUp(int error_code)
		{
			m_error_code = error_code;
			switch (m_error_code)
			{
			case WSAEWOULDBLOCK:
				m_error = "Resource temporarily unavailable.";
				m_info = "向非阻塞套接字请求的操作不能立即完成。";
				break;
			case WSAENOTSOCK:
				m_error = "Socket operation on nonsocket.";
				m_info = "传入的文件描述符并非合法的套接字描述符。";
				break;
			case WSAESHUTDOWN:
				m_error = "Cannot send after socket shutdown.";
				m_info = "请求的文件描述符代表的套接字已经关闭，检查代码逻辑。";
				break;
			case WSAENOTCONN:
				m_error = "Socket is not connected.";
				m_info = "请求的文件描述符指向的套接字已开启，但尚未建立连接。";
				break;
			case WSAEISCONN:
				m_error = "Socket is already connected.";
				m_info = "请求执行连接的文件描述符指向的套接字早已建立连接。";
				break;
			case WSAENETUNREACH:
				m_error = "Network is unreachable.";
				m_info = "请求的地址无法抵达。";
				break;
			case WSAEHOSTDOWN:
				m_error = "Host is down.";
				m_info = "目标主机关闭，套接字操作失败。本地主机的网络连接尚未启动";
				break;
			case WSAECONNREFUSED:
				m_error = "Connection refused.";
				m_info = "由于目标主机积极拒绝，无法建立连接。";
				break;
			case WSAEHOSTUNREACH:
				m_error = "No route to host.";
				m_info = "由于没有抵达主机的路由，套接字操作失败。";
				break;
			case WSAETIMEDOUT:
				m_error = "Connection time out.";
				m_info = "连接尝试失败，由于连接在一段时间内没有正确响应或连接主机没有响应。";
				break;
#ifdef WIN32//windows 专属错误
			case WSASYSNOTREADY:
				m_error = "Network subsystem is unavailable.";
				m_info = "Windows Sockets 子系统尚未启动，检查是否调用WSAStartup？或过早调用WSACleanup？";
				break;
			case WSANOTINITIALISED:
				m_error = "Successful WSAStartup not yet performed.";
				m_info = "Windows Sockets 子系统尚未初始化，检查是否调用WSAStartup？或过早调用WSACleanup？";
				break;
#endif
			case WSAEADDRNOTAVAIL:
				m_error = "Cannot assign requested address.";
				m_info = "请求的地址在当前上下文中无效";
				break;
			default:
				m_error = "Undefine";
				m_info = "This Exception has not defined yet.";
			}
			return 0;
		}
		ListeningSocket::ListeningSocket()
			: Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), m_listening(false)
		{
		}

		ListeningSocket::ListeningSocket(bool p_block)
			: Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), m_listening(false)
		{
			SetBlocking(p_block);
		}

		ListeningSocket::ListeningSocket(string p_addr, port_t p_port, bool p_block)
			: Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
		{
			SetBlocking(p_block);
			ListenOn(p_addr, p_port);
		}

		void ListeningSocket::ListenOn(string p_addr, port_t p_port, int p_max_queue_length)
		{
			Bind(p_addr.c_str(), p_port);
			Listen(p_max_queue_length);
		}

		void ListeningSocket::Listen(int max_queue_length)
		{
			if (m_listening) throw InvalidOperationException("ListeningSocket::Listen(): Already in listening.", __LINE__, __FILE__, "Operation in proceed.");
			m_listening = true;
			if (listen(m_sock, max_queue_length)) throw NetworkException(GetError());
		}
		DataSocket* ListeningSocket::Accept()
		{
			int mlal = sizeof(m_local_address);
			SOCKET ret = accept(m_sock, (sockaddr*)&m_local_address, &mlal);
			if (ret == INVALID_SOCKET)
				if (GetError() != WSAEWOULDBLOCK) throw NetworkException(GetError());
				else return nullptr;
			DataSocket* ds = new DataSocket(ret);
			return ds;
		}

		Connection::~Connection()
		{
			delete m_sock;
			delete m_proto;
		}

		Connection::Connection(Connection& p_conn)
		{
			m_sock = p_conn.m_sock;
			m_proto = p_conn.m_proto;
		}

		Connection::Connection(DataSocket* p_sock, Protocol* p_proto)
		{
			m_sock = p_sock;
			m_proto = p_proto;
		}

		int Connection::Recv()
		{
			static char buffer[BUF_SIZE];
			int recvl = m_sock->Recv(buffer, sizeof(buffer));
			if (recvl)
			{
				return m_proto->ProcessRecvMsg(buffer, recvl);
			}
			return 0;
		}

		int Connection::Send(char* msg, int length)
		{
			int newl = m_proto->ProcessSendMsg(msg, length);
			int sended = 0;
			do
			{
				sended += m_sock->Send(msg + sended, newl - sended);
			} while (sended <= newl);
			return 0;
		}

		int Connection::SendDirectly(char* msg, int length)
		{
			int sended = 0;
			do
			{
				sended += m_sock->Send(msg + sended, length - sended);
			} while (sended <= length);
			return 0;
		}
		void Connection::Close()
		{
			m_sock->Close();
		}
	}//end of namespace Network
}//end of namespace WelkinEngine
