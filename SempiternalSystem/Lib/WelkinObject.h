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

	/*苍穹引擎游戏至小管理单位
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
//	* 平面的地图位置信息
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

	/*地图上的、有形单位
	*/
	class Entity : public Object {
		void EmptyInit();
	protected:
		Position m_map_loc;         /*对象在地图上的位置*/
		Size     m_interactiveSize; /*交互检测用*/
		Size     m_boundingSize;    /*碰撞检测用*/
		Events*  m_event;           /*事件调用*/

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
		PhysicsParameter m_physic;          /*对象物理行为计算支持*/
		PhysicsParameter m_tmp_physic;      /*计算得出的临时物理信息*/
		Position         m_tmp_loc;         /*计算得出的临时位置信息*/
	public:
		/*自动地计算(单体的)物理行为(移动),
		* 调用后应当立即对新的位置进行全局碰撞检测分析、并手动更新位置
		* 特别注意，计算所得物理结果将暂存到m_tmp_physics、m_tmp_loc
		* 要访问它们，只需要调用（GetTempPhysicsStatus或GetTempLocation
		* 需要调用ApplyTempValues()使生效
		* 返回值为计算出的移动向量数据
		*/
		virtual Vector UpdatePhysics(Vector p_F, double p_delta_t);

		/*获取临时物理数据
		* 由UpdatePhysics（）得出
		* 未调用UpdatePhysics前值不确定
		* 通过ApplyTempValues以应用
		*/
		virtual inline PhysicsParameter GetTempPhysicsStatus()
		{
			return m_tmp_physic;
		}

		/*获取临时位置信息
		* 由UpdatePhysics()得出
		* 未调用UpdatePhysics前值不确定
		* 通过调用ApplyTempValues()以应用
		*/
		virtual inline Position GetTempLocation()
		{
			return m_tmp_loc;
		}

		/*应用临时数据
		* 应用由UpdatePhysics()得出的数据
		* 调用UpdatePhysics前调用此函数导致的结果不确定
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