#pragma once
#include <cstdio>
#include <cstring>
#include <cerrno>
#include "TypeParser.h"

enum Type {
	Int,
	String,
	Bool,
	Float
};

struct KVL {
	const char* symbol;
	enum Type type;
	void* dataPointer;
};

class Configuration
{
protected:
	int ParseKVP(const char* p_key, const char* p_value);
public:
	int videoHSize, videoVSize;
	bool telnetEnable;

	Configuration();

	/// <summary>
	/// ��ȡ�����ļ�
	/// </summary>
	/// <param name="p_fileName">�����ļ�·��</param>
	/// <returns>�ɹ�Ϊ�㣬����Ϊ��һֵ</returns>
	int LoadConfigurationFromFile(const char* p_fileName);
private:
	static const int keyValueListLength = 3;
	KVL KeyValueList[keyValueListLength] = {
		{"iVideoHorizonal", Type::Int, &videoHSize},
		{"iVideoVertical", Type::Int, &videoVSize},
		{"bTelnetEnable", Type::Bool, &telnetEnable}
	};
};

