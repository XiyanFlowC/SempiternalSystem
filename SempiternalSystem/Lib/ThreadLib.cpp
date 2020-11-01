/*
	Name:Thread Lib.cpp 
	Copyright:none 
	Date: 05/08/18 13:41
	Description:Thread support and lib portable 
	Version:0.0.0
*/
#include"ThreadLib.h"

namespace ThreadLib
{
//	typedef void (*ThreadFunc)(void*);
//	
	#ifdef WIN32
//	typedef DWORD ThreadID;
	std::map< DWORD, HANDLE > g_handlemap;
	#else
//	typedef pthread_t ThreadID;
	#endif
	
//	struct DummyData
//	{
//		public:
//			ThreadFunc m_func;
//			void* m_data;
//	}
	
	#ifdef WIN32
	DWORD WINAPI DummyRun(void* p_data)
	#else
	void* DummyRun(void* p_data)
	#endif
	{
		DummyData* data = (DummyData*) p_data;
		data->m_func(data->m_data);
		delete data;
		return 0;
	}
	
	ThreadID Create(ThreadFunc p_func, void* p_param)
	{
		ThreadID t;
		DummyData* data = new DummyData;
		data->m_func = p_func;
		data->m_data = p_param;
		#ifdef WIN32
		HANDLE h;
		h = CreateThread(NULL, 0, DummyRun, data, 0, &t);
		if(h != 0)
		{
			g_handlemap[t] = h;
		}
		#else
		pthread_create(&t, 0, DummyRun, data);
		#endif
		if(t == 0)
		{
			delete data;
			//return -1;//temp error deal
			//throw Exception("CreationFailure");
			throw ThreadException(10001);
		}
		return t;
	}
	
	inline ThreadID GetID()
	{
		#ifdef WIN32
		return GetCurrentThreadId();
		#else
		return pthread_self();
		#endif
	}
	
	inline void WaitForFinish(ThreadID p_thread)
	{
		#ifdef WIN32
		WaitForSingleObject(g_handlemap[p_thread], INFINITE);
		CloseHandle(g_handlemap[p_thread]);
		g_handlemap.erase(p_thread);
		#else
		pthread_join(p_thread, NULL);
		#endif
	} 
	
	inline void Kill(ThreadID& p_thread)
	{
		#ifdef WIN32
		TerminateThread(g_handlemap[p_thread], 0);
		CloseHandle( g_handlemap[p_thread]);
		g_handlemap.erase(p_thread);
		#else
		pthread_cancel(p_thread);
		#endif
	}
	
	inline void YieldThread(int p_milliseconds = 1)
	{
		#ifdef WIN32
		Sleep(p_milliseconds);
		#else
		usleep(p_milliseconds * 1000);
		#endif
	}
	
	ThreadException::ThreadException(int p_errorCode) : errorCode(p_errorCode)
	{
	}

	/*	class Mutex
	{
		protected:
			#ifdef WIN32
			CRITICAL_SECTION m_mutex;
			#else
			pthread_mutext_t m_mutex;
			#endif
			
			Mutex()
			{
				#ifdef WIN32
				InitializeCriticalSection(&m_mutex);
				#else
				pthread_mutex_init(&m_mutex, 0);
				#endif
			}
			
			~Mutex()
			{
				#ifdef WIN32
				DeleteCriticalSection(&m_mutex);
				#else
				pthread_mutex_destroy(&m_mutex);
				#endif
			}
			
			inline void Lock()
			{
				#ifdef WIN32
				EnterCriticalSection(&m_mutex);
				#else
				pthread_mutex_lock(&m_mutex);
				#endif
			}
			
			inline void Unlock()
			{
				#ifdef WIN32
				LeaveCriticalSection(&m_mutex);
				#else
				pthread_mutex_unlock(&m_mutex);
				#endif
			}
	}*/
}
