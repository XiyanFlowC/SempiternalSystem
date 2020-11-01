#pragma once

#include "WelkinException.h"
#include <vector>

namespace WelkinEngine
{
	/*
	* 类，事件集合
	* 提供统一的方法来创建和触发事件
	* 进行事件处理的类须以友元形式声明
	*/
	class Events
	{
	public:
		/*
		* 构造方法，用于创建一个空的事件集合
		*/
		//Events();//在不能调整事件总数时不应使用

		/*
		* 构造方法，确定最大的事件可用数量（默认为16）
		*/
		Events(int p_cnt = 16);
		
		/*
		* 方法，注册事件
		* 使某个事件在发生时可被调用
		*/
		virtual void RegisterEvent(int p_eventID, void (*p_callback) (int, void*));

		/*
		* 方法，注册事件组
		* 使某些函数在事件发生是被一起回调
		* 注意！按照注册的先后顺序调用！
		* 参数：事件ID，处理器数目，处理器指针数组
		* 返回值：成功注册的函数个数
		*/
		virtual int RegisterEvents(int p_eventID, int p_func, void (**p_callbacks) (int, void*));

		/*
		* 方法，推送事件到各个事件处理函数
		*/
		virtual void PullEvnet(int p_eventID, int argc, void* args);
	protected:
		/*
		* 方法，声明事件
		*/
		//virtual void DeclareEvent(int p_eventID);
	private:
		std::vector<void (*) (int, void*)> *m_events;
		/*
		* 成员变量，注册的事件数
		*/
		int m_cnt;

	};
}
