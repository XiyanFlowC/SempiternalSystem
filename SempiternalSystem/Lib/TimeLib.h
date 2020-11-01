/*
	Name: TimeLib.h 
	Copyright: none 
	Date: 09/08/18 09:31
	Description: 
*/
#ifndef TIMELIB_H
#define TIMELIB_H

#ifdef WIN32
#include<windows.h>
#else
#include<sys/time.h>
#endif

#include<ctime>
#include<string>
#include<sstream>

namespace TimeLib
{
	#ifdef WIN32
	class WIN32PerformanceCounter
	{
		public:
			long long m_frequency;
			
			WIN32PerformanceCounter()
			{
				QueryPerformanceFrequency((_LARGE_INTEGER*)(&m_frequency));
				m_frequency = m_frequency / 1000;
			}
	};
	#endif
	
	class Timer
	{
		public:
			Timer(long long time_passed = 0);
			void Reset(long long time_passed = 0);
			long long get_ms();
			long long get_s();
			long long get_m();
			long long get_h();
			long long get_d();
		protected:
			long long m_init_time;
			long long m_start_time;
	};

	class TimeVal
	{
	public:
		TimeVal();
		TimeVal(TimeVal& a);
		TimeVal(int p_hour, short p_min, short p_sec);
		int SetTime(int p_hour, short p_min, short p_sec);
		int GetHour();
		short GetMin();
		short GetSec();
		TimeVal& operator = (TimeVal& right);
		//friend TimeVal operator - (TimeVal& left, TimeVal& right);
		//friend TimeVal operator + (TimeVal& left, TimeVal& right);
		std::string ToString();
	protected:
		int m_hour;
		short m_min;
		short m_sec;
	};
	
	long long get_time();
	long long get_time_second();
	long long get_time_minute();
	long long get_time_hour();
	long long get_time_day();
	
	std::string time_stamp();
	std::string time_stamp_underline();
	std::string date_stamp();
	std::string date_stamp_underline();
	std::string local_time_stamp();
}
#endif
