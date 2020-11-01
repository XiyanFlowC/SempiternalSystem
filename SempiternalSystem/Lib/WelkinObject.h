#pragma once

#ifndef __WELKINOBJECT_H_
#define __WELKINOBJECT_H_
#include "WelkinString.h"
#include "WelkinTypes.h"
#include "WelkinEvents.h"
#include "WelkinVision.h"

namespace WelkinEngine
{
	extern const Size StandardBoundingSize;

	/*���������Ϸ��С����λ
	*/
	class Object
	{
	protected:
		int m_ID;

		string m_mnemonic_name;
	public:
		int GetID();

		string GetMnemonic();

		void SetMenemonic(string p_mnemonic_name);

		Object(int ID);

		Object(int ID, string p_mnemonic_name);
	};/*class Object*/

//#ifdef _HeightEnabled

//#else
//	/*
//	* ƽ��ĵ�ͼλ����Ϣ
//	*/
//	struct Position {
//		int x;
//		int y;
//	};
//
//	struct Size {
//		int x;
//		int y;
//		int z;
//	};
//#endif

	/*��ͼ�ϵġ����ε�λ
	*/
	class Entity : public Object {
		void EmptyInit();
	protected:
		Position m_map_loc;         /*�����ڵ�ͼ�ϵ�λ��*/
		Size     m_interactiveSize; /*���������*/
		Size     m_boundingSize;    /*��ײ�����*/
		Events*  m_event;           /*�¼�����*/

		enum EventType {
			NullEvent = 0,
			OnEntityInteractive = 1,
			OnEntiryStrick = 2,
			OnGenerate = 3,
			OnDispose = 4,
			OnScriptInvoke = 5,
			OnScriptTerminate = 6,
			DoNotUse_EventCount
		};
		
	public:
		Entity(int ID);

		Entity(int ID, string p_mnemonic_name, Position p_loc, Size p_interS, Size p_boundS);

		virtual inline void SetPosition(Position p_new_position)
		{
			m_map_loc = p_new_position;
		}

		virtual inline Position GetPosition()
		{
			return m_map_loc;
		}

		virtual inline Size GetInteractiveSize()
		{
			return m_interactiveSize;
		}

		virtual inline Size GetBoundingSize()
		{
			return m_boundingSize;
		}

		virtual inline void SetBoundingSize(Size p_newBounding)
		{
			m_boundingSize = p_newBounding;
		}

		virtual inline void SetInteractiveSize(Size p_newInteractiveBounding)
		{
			m_interactiveSize = p_newInteractiveBounding;
		}
	};/*class Entity*/

	class PhysicsEntity : public Entity
	{
	protected:
		PhysicsParameter m_physic;          /*����������Ϊ����֧��*/
		PhysicsParameter m_tmp_physic;      /*����ó�����ʱ������Ϣ*/
		Position         m_tmp_loc;         /*����ó�����ʱλ����Ϣ*/
	public:
		/*�Զ��ؼ���(�����)������Ϊ(�ƶ�),
		* ���ú�Ӧ���������µ�λ�ý���ȫ����ײ�����������ֶ�����λ��
		* �ر�ע�⣬�����������������ݴ浽m_tmp_physics��m_tmp_loc
		* Ҫ�������ǣ�ֻ��Ҫ���ã�GetTempPhysicsStatus��GetTempLocation
		* ��Ҫ����ApplyTempValues()ʹ��Ч
		* ����ֵΪ��������ƶ���������
		*/
		virtual Vector UpdatePhysics(Vector p_F, double p_delta_t);

		/*��ȡ��ʱ��������
		* ��UpdatePhysics�����ó�
		* δ����UpdatePhysicsǰֵ��ȷ��
		* ͨ��ApplyTempValues��Ӧ��
		*/
		virtual inline PhysicsParameter GetTempPhysicsStatus()
		{
			return m_tmp_physic;
		}

		/*��ȡ��ʱλ����Ϣ
		* ��UpdatePhysics()�ó�
		* δ����UpdatePhysicsǰֵ��ȷ��
		* ͨ������ApplyTempValues()��Ӧ��
		*/
		virtual inline Position GetTempLocation()
		{
			return m_tmp_loc;
		}

		/*Ӧ����ʱ����
		* Ӧ����UpdatePhysics()�ó�������
		* ����UpdatePhysicsǰ���ô˺������µĽ����ȷ��
		*/
		virtual inline void ApplyTempValues()
		{
			m_physic = m_tmp_physic;
			m_map_loc = m_tmp_loc;
		}

		virtual inline void SetPhysicsStatus(PhysicsParameter p_physic)
		{
			m_physic = p_physic;
		}

		virtual inline PhysicsParameter GetPhysicsStatus()
		{
			return m_physic;
		}
	};//End of physics entity
}/*namespace WelkinEngine*/
#endif/*__WELKINOBJECT_H_*/