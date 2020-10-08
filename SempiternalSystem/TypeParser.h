#pragma once
#include <cctype>
#include <string>
#include <exception>

#ifndef _STR_LINE_
#define ___ST_SS(x) #x
#define ___ST_SR(x) ___ST_SS(x)
#define _STR_LINE_ ___ST_SR(__LINE__)
#endif

class TypeParser
{
public:
	//ת���ַ�����int��
	//�쳣: �ַ�����ʽ����ʱ�׳�FormatIncorrectException
	static int string2int(const char* p_input);
};

class FormatIncorrectException
{
	std::string message;
public:
	FormatIncorrectException();

	FormatIncorrectException(std::string p_message);

	FormatIncorrectException(std::string p_file, std::string p_line);

	const char* what()
	{
		return message.c_str();
	}
};