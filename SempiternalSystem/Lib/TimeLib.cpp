/*
	Name: Time Lib.cpp 
	Copyright: none 
	Date: 09/08/18 09:05
	Description: To support time get and return 
*/
#include"TimeLib.h"

namespace TimeLib
{	
	WIN32PerformanceCounter g_WIN32counter;
	long long get_time()
	{
		#ifdef WIN32
		//long long freq;
		//int ret = QueryPerformanceFrequency((long long*)(&freq));
		//if(ret == 0)
		//{
		//	return -1;
		//}
		long long t;
		QueryPerformanceCounter((_LARGE_INTEGER*)(&t));
		t /= g_WIN32counter.m_frequency;
		return t;
		#else
		struct timeval t;
		gettimeofday(&t, 0);
		long long s = t.tv_sec;
		s *= 1000;
		s += (t.tv_usec / 1000);
		return s;
		#endif
	}
	
	long long get_time_second()
	{
		return get_time() / 1000;
	}
	
	long long get_time_minute()
	{
		return get_time_second() / 60;
	}
	
	long long get_time_hour()
	{
		return get_time_minute() / 60;
	}
	
	long long get_time_day()
	{
		return get_time_hour() / 24;
	}
	
	std::string time_stamp()
	{
		char tim[9];
		time_t t = time(0);
		struct tm* b = gmtime(&t);
		strftime(tim, 9, "%H:%M:%S", b);
		return tim;
	}

	std::string local_time_stamp()
	{
		char tim[9];
		time_t t = time(0);
		struct tm* b = localtime(&t);
		strftime(tim, 9, "%H:%M:%S", b);
		return tim;
	}
	
	std::string time_stamp_underline()
	{
		char tim[9];
		time_t t = time(0);
		struct tm* b = gmtime(&t);
		strftime(tim, 9, "%H_%M_%S", b);
		return tim;
	}
	
	std::string date_stamp()
	{
		char date[11];
		time_t t = time(0);
		struct tm* b = gmtime(&t);
		strftime(date, 11, "%Y-%m-%d", b);
		return date;
	}

	std::string date_stamp_underline()
	{
		char date[11];
		time_t t = time(0);
		struct tm* b = gmtime(&t);
		strftime(date, 11, "%Y_%m_%d", b);
		return date;
	}
	
	//functions of class 'Timer'
	void Timer::Reset(long long time_passed)
	{
		m_start_time = time_passed;
		m_init_time = get_time();
	}
	
	Timer::Timer(long long time_passed)
	{
		Reset(time_passed);
	}
	
	long long Timer::get_ms()
	{
		return (get_time() - m_init_time) + m_start_time;
	}
	
	long long Timer::get_s()
	{
		return get_ms() / 1000;
	}
	
	long long Timer::get_m()
	{
		return get_s() / 60;
	}
	
	long long Timer::get_h()
	{
		return get_m() / 60;
	}
	
	long long Timer::get_d()
	{
		return get_h() / 24;
	}

	TimeVal::TimeVal()
		: m_hour(0), m_min(0), m_sec(0) {}

	TimeVal::TimeVal(TimeVal& a)
	{
		m_hour = a.GetHour();
		m_min = a.GetMin();
		m_sec = a.GetSec();
	}

	TimeVal::TimeVal(int p_hour, short p_min, short p_sec)
	{
		SetTime(p_hour, p_min, p_sec);
	}

	int TimeVal::SetTime(int p_hour, short p_min, short p_sec)
	{
		m_hour = p_hour, m_min = p_min, m_sec = p_sec;
		if(m_sec >= 60)
		{
			m_min += m_sec / 60;
			m_sec %= 60;
		}
		if(m_min >= 60)
		{
			m_hour += m_min / 60;
			m_min %= 60;
		}
		return 0;
	}

	int TimeVal::GetHour()
	{
		return this->m_hour;
	}

	short TimeVal::GetMin()
	{
		return this->m_min;
	}

	short TimeVal::GetSec()
	{
		return this->m_sec;
	}

	TimeVal& TimeVal::operator = (TimeVal& right)
	{
		SetTime(right.GetHour(), right.GetMin(), right.GetSec());
		return *this;
	}

	TimeVal operator - (TimeVal& left, TimeVal& right)
	{
		TimeVal c;
		int tmp, flag;
		int hour = 0;
		short sec = 0, min = 0;
		tmp = left.GetSec() - right.GetSec();
		if(tmp < 0)
		{
			sec = 60 + tmp;
			min -= 1;
		}
		else sec += tmp;
		tmp = left.GetMin() - right.GetMin();
		if(tmp + min < 0)
		{
			min = 60 + tmp;
			hour -= 1;
		}
		else min += tmp;
		hour += left.GetHour() - right.GetHour();
		c.SetTime(hour, min, sec);
		return c;
	}

	std::string TimeVal::ToString()
	{
		std::ostringstream stream;
		stream << m_hour << ":" << m_min << ":" << m_sec;
		return stream.str();
	}
}
