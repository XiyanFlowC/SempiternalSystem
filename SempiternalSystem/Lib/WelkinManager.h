#pragma once

#ifndef __WELKINMANAGER_H_
#define __WELKINMANAGER_H_

namespace WelkinEngine
{
	class Manager {
	protected:
		bool halted;
	public:
		virtual void DoUpdate() = 0;
		virtual void ShutDown() = 0;
		virtual void StartUp() = 0;

		virtual bool IsHalted();

		/*处理暂时停止
		* 是否遵从取决于DoUpdate()实现
		*/
		virtual void Halt();

		/*恢复处理
		* 是否遵从取决于DoUpdate()实现
		*/
		virtual void Wake();
	};
}

#endif/*__WELKINMANAGER_H_*/