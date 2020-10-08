#pragma once
#include <cstdlib>

//事件处理函数注册管理用类
class EventRegistry
{
protected:
	//已经注册的函数
	void (** registeredFunc) (int, void*);
	//注册上限
	int funcSize;

	struct RegistryTerm {
		int key;
		int index;
	} *terms;
public:
	//已经注册的函数数量
	int size;

	//按事件注册一个新函数
	//成功返回0,失败返回非0. 特别地,注册表已满将返回-1
	//p_func: 函数
	//key: 表项
	virtual int Register(void (*p_func) (int, void*), int key);

	//注册一个函数(不推荐)
	//p_func: 要注册的函数
	virtual int Register(void(*p_func)(int, void*));

	//按表项处理送达的事件
	virtual void HandleEvent(int p_key, int argc, void* args);

	//执行注册表中函数
	virtual void ExecFunc(int p_index, int argc, void* args);

	//以默认容量（128）初始化事件注册表
	EventRegistry();

	//以指定的容量初始化事件注册表
	EventRegistry(int p_size);

	~EventRegistry();
};

