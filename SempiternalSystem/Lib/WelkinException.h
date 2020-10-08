#ifndef __WELKINEXCEPTION_H_
#define __WELKINEXCEPTION_H_
#include "WelkinString.h"

#pragma once

namespace WelkinEngine
{
	class Exception// : public std::exception
	{
	public:
		enum BasicErr
		{
			PopEmptyStack = 0x0
		};

		virtual void LookUp(int& error_code);

		Exception();
		
		Exception(const char* expr, int error);

		Exception(int error_code);

		Exception(string p_err);

		Exception(string p_err, int p_line, string p_file);

		string Get();

		char* ToString();

		/*bool operator == (Exception& a)
		{
			return m_error_code == a.m_error_code;
		}*/
	protected:
		char infomation[512];
		string m_error;
		string m_info;
		int line;
		int m_error_code;
		string file;
	};

	class ParameterException : public Exception {
	public:
		ParameterException();

		ParameterException(string p_err, int p_line, string p_file, string p_info);
	};

	class InvalidOperationException : public Exception {
	public:
		InvalidOperationException();

		InvalidOperationException(string p_err, int p_line, string p_file, string p_info);
	};
}
#endif
