#pragma once

#ifndef __WELKINTYPES_H_
#define __WELKINTYPES_H_

namespace WelkinEngine
{
	/*
	* ����ĵ�ͼλ����Ϣ
	*/
	struct Position {
		double x;
		double y;
		double z;
	};

	/*
	* ��ͼ��С��Ϣ
	* ��Position������ͬ����
	*/
	typedef Position Size, Vector;

	struct PhysicsParameter {
		Vector m_v;     /*�ƶ��ٶ�*/
		double m_mu;    /*��Ħ������*/
		double m_m;     /*����*/
		double m_pcb_f; /*Ԥ���㻺��-Ħ����*/
		double m_deE;   /*��ײ˥��ϵ��*/
	};

	//const static Size StandardBounding = { 1.0f, 1.0f, 1.0f };
}

#endif