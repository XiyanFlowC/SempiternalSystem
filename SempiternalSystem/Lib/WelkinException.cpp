#include "WelkinException.h"

inline void WelkinEngine::Exception::LookUp(int& error_code)
{
	m_error_code = error_code;
	m_error = "Undefine";
	m_info = "This Exception has not defined yet.";
}

inline WelkinEngine::Exception::Exception() :infomation("undefined."), line(-1), m_error_code(-1) {}

inline WelkinEngine::Exception::Exception(const char* expr, int error)
{
	m_error = expr;
	m_error_code = error;
}

inline WelkinEngine::Exception::Exception(int error_code)
{
	//m_error_code = error_code;
	LookUp(error_code);
}

inline WelkinEngine::Exception::Exception(string p_err)
{
	m_error = p_err;
}

inline WelkinEngine::Exception::Exception(string p_err, int p_line, string p_file)
{
	m_error = p_err;
	line = p_line;
	file = p_file;
}

inline WelkinEngine::string WelkinEngine::Exception::Get()
{
	return "“Ï≥££∫" + m_error + "Message or ErrCode:" + m_info + "\nOn File:" + file + " Line:" + IntToString(line);
}

inline char* WelkinEngine::Exception::ToString()
{
	string ret;
	ret += "“Ï≥££∫";
	ret += m_error + '\n';
	ret += "Message£∫";
	ret += m_info + '\n';
	ret += "ErrorCode:";
	ret += IntToString(m_error_code);
	strcpy(infomation, ret.c_str());
	return infomation;
}

WelkinEngine::ParameterException::ParameterException()
{
}

WelkinEngine::ParameterException::ParameterException(string p_err, int p_line, string p_file, string p_info)
	: Exception(p_err, p_line, p_file)
{
	m_info = p_info;
}

WelkinEngine::InvalidOperationException::InvalidOperationException()
{
}

WelkinEngine::InvalidOperationException::InvalidOperationException(string p_err, int p_line, string p_file, string p_info)
	:Exception(p_err, p_line, p_file)
{
	m_info = p_info;
}


