#pragma once

#include "WelkinException.h"
#include <vector>

namespace WelkinEngine
{
	/*
	* �࣬�¼�����
	* �ṩͳһ�ķ����������ʹ����¼�
	* �����¼��������������Ԫ��ʽ����
	*/
	class Events
	{
	public:
		/*
		* ���췽�������ڴ���һ���յ��¼�����
		*/
		//Events();//�ڲ��ܵ����¼�����ʱ��Ӧʹ��

		/*
		* ���췽����ȷ�������¼�����������Ĭ��Ϊ16��
		*/
		Events(int p_cnt = 16);
		
		/*
		* ������ע���¼�
		* ʹĳ���¼��ڷ���ʱ�ɱ�����
		*/
		virtual void RegisterEvent(int p_eventID, void (*p_callback) (int, void*));

		/*
		* ������ע���¼���
		* ʹĳЩ�������¼������Ǳ�һ��ص�
		* ע�⣡����ע����Ⱥ�˳����ã�
		* �������¼�ID����������Ŀ��������ָ������
		* ����ֵ���ɹ�ע��ĺ�������
		*/
		virtual int RegisterEvents(int p_eventID, int p_func, void (**p_callbacks) (int, void*));

		/*
		* �����������¼��������¼�������
		*/
		virtual void PullEvnet(int p_eventID, int argc, void* args);
	protected:
		/*
		* �����������¼�
		*/
		//virtual void DeclareEvent(int p_eventID);
	private:
		std::vector<void (*) (int, void*)> *m_events;
		/*
		* ��Ա������ע����¼���
		*/
		int m_cnt;

	};
}
