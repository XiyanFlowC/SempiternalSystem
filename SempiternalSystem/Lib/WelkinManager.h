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

		/*������ʱֹͣ
		* �Ƿ����ȡ����DoUpdate()ʵ��
		*/
		virtual void Halt();

		/*�ָ�����
		* �Ƿ����ȡ����DoUpdate()ʵ��
		*/
		virtual void Wake();
	};
}

#endif/*__WELKINMANAGER_H_*/