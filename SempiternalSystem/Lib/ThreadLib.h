/*
	Name: ThreadLib.h 
	Copyright: none 
	Date: 05/08/18 18:58
	Description: Platform isolation
	Version: 0.0.0
*/
#ifndef ThreadLib_h
#define ThreadLib_h

//STL include
#include<map>

//OS include
#ifdef WIN32 

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#else

#include<pthread.h>

#endif
namespace ThreadLib
{
	class ThreadException {
		int errorCode;
	public:
		ThreadException(int p_errorCode);

		inline int GetError() {
			return errorCode;
		}
	};

	typedef void (*ThreadFunc)(void*);
	
	#ifdef WIN32
	typedef DWORD ThreadID;
	extern std::map< DWORD, HANDLE > g_handalmap;
	#else
	typedef pthread_t ThreadID;
	#endif
	
	struct DummyData
	{
		public:
			ThreadFunc m_func;
			void* m_data;
	};
	
	#ifdef WIN32
	DWORD WINAPI DummyRun(void* p_data);
	#else
	void* DummyRun(void* p_data);
	#endif
	
	ThreadID Create(ThreadFunc p_func, void* p_param);
	
	inline ThreadID GetID();
	
	inline void WaitForFinish(ThreadID p_thread);

	inline void Kill(ThreadID& p_thread);
	
	inline void YieldThread(int p_milliseconds);
	
	class Mutex
	{
		protected:
			#ifdef WIN32
			CRITICAL_SECTION m_mutex;
			#else
			pthread_mutext_t m_mutex;
			#endif
	public:
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
	};
}

#endif
