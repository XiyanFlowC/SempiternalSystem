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
	/// 读取配置文件
	/// </summary>
	/// <param name="p_fileName">配置文件路径</param>
	/// <returns>成功为零，否则为任一值</returns>
	int LoadConfigurationFromFile(const char* p_fileName);
private:
	static const int keyValueListLength = 3;
	KVL KeyValueList[keyValueListLength] = {
		{"iVideoHorizonal", Type::Int, &videoHSize},
		{"iVideoVertical", Type::Int, &videoVSize},
		{"bTelnetEnable", Type::Bool, &telnetEnable}
	};
};

