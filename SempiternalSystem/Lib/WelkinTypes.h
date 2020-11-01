#pragma once

#ifndef __WELKINTYPES_H_
#define __WELKINTYPES_H_

namespace WelkinEngine
{
	/*
	* 立体的地图位置信息
	*/
	struct Position {
		double x;
		double y;
		double z;
	};

	/*
	* 地图大小信息
	* 和Position具有相同构造
	*/
	typedef Position Size, Vector;

	struct PhysicsParameter {
		Vector m_v;     /*移动速度*/
		double m_mu;    /*动摩擦因数*/
		double m_m;     /*质量*/
		double m_pcb_f; /*预计算缓冲-摩擦力*/
		double m_deE;   /*碰撞衰减系数*/
	};

	//const static Size StandardBounding = { 1.0f, 1.0f, 1.0f };
}

#endif