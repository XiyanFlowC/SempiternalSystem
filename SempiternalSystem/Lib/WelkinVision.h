#pragma once

#include <vector>
#include "TimeLib.h"
#include "WelkinException.h"

namespace WelkinEngine
{
	struct RGBA32 {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	};

	/// <summary>
	/// ������ͼ
	/// </summary>
	class Texture {
		int height, width;
		unsigned char* data;
	public:
		Texture();

		Texture(int p_height, int p_width, unsigned char* p_texture);

		inline int GetHeight()
		{
			return height;
		}

		inline int GetWidth()
		{
			return width;
		}

		/// <summary>
		/// �������������Ҫ������һ�����������÷���ֵ
		/// </summary>
		/// <param name="texture">�������ݵ�ַ</param>
		/// <param name="p_height">����߶�</param>
		/// <param name="p_width">������</param>
		/// <returns>ָ����һ������</returns>
		unsigned char* SetTexture(unsigned char* texture, int p_height, int p_width);

		inline unsigned char* GetTextureData()
		{
			return data;
		}
	};

	/// <summary>
	/// ֡����ʱ����Ϣ����ͼ����
	/// </summary>
	struct Frame {
		long long sustainTime;
		Texture* m_texture;
	};

	/// <summary>
	/// ���򶯻�������������ͼ������ʾ
	/// </summary>
	class Animation {

	protected:
		std::vector<Frame> *framesA;     /*�����·�������*/
		std::vector<Frame> *framesB;     /*�����·�������*/
		std::vector<Frame> *framesC;     /*�����Ϸ�������Ϊ��ʱ����A��*/
		std::vector<Frame> *framesD;     /*�����Ϸ�������Ϊ��ʱ����B��*/
		std::vector<Frame> *framesUp;    /*�򶥲�������*/
		std::vector<Frame> *framesDown;  /*��ײ�������*/
		std::vector<Frame> *framesLeft;  /*���������*/
		std::vector<Frame> *framesRight; /*���ҵ�����*/
		int                currentFrame; /*֡����*/

		enum FaceDirection {
			Up,
			Down,
			Left,
			Right,
			LeftDown,
			RightDown,
			RightUp,
			LeftUp
		} m_face;

		long long m_lastFrameShownTime;
	public:
		/// <summary>
		/// �Ը�����֡���������ʼ������
		/// </summary>
		/// <param name="p_textureArray">������֡��������</param>
		Animation(std::vector<Frame>** p_textureArray);

		/// <summary>
		/// �Ը�����֡���г�ʼ���������������ձ�ʾ������
		/// </summary>
		/// <param name="p_A">���·���֡����</param>
		/// <param name="p_B">���·���֡����</param>
		/// <param name="p_C">���Ϸ���֡����</param>
		/// <param name="p_D">���Ϸ���֡����</param>
		/// <param name="p_Up">�򶥲���֡����</param>
		/// <param name="p_Down">��ײ���֡����</param>
		/// <param name="p_Left">���󷽵�֡����</param>
		/// <param name="p_Right">���ҷ���֡����</param>
		Animation(std::vector<Frame>* p_A,
			std::vector<Frame>* p_B,
			std::vector<Frame>* p_C,
			std::vector<Frame>* p_D,
			std::vector<Frame>* p_Up,
			std::vector<Frame>* p_Down,
			std::vector<Frame>* p_Left,
			std::vector<Frame>* p_Right
		);

		/// <summary>
		/// �Ը�����֡���г�ʼ������
		/// </summary>
		/// <param name="p_A">���·���֡����</param>
		/// <param name="p_B">���·���֡����</param>
		Animation(std::vector<Frame>* p_A, std::vector<Frame>* p_B);

		/// <summary>
		/// �Ļ�����
		/// </summary>
		/// <param name="p_face">����</param>
		inline void SwitchFacing(FaceDirection p_face)
		{
			m_face = p_face;
		}

		/// <summary>
		/// �������á�֡�Ƿ�׼���������첽����ʱ�ã�
		/// </summary>
		/// <returns>true��ʾ׼�����</returns>
		bool IsNextFrameReady();

		/// <summary>
		/// �������á��ͷ�Ϊ�첽����
		/// </summary>
		void Asyn();

		Texture *doFrameUpdate(std::vector<Frame>* p_targetFrames);

		/// <summary>
		/// ��ȡ��ǰ֡ͼ��
		/// </summary>
		/// <returns>ָ��ǰӦ��ʾ֡��ָ��</returns>
		Texture *GetCurrentTexture();
	};

	/// <summary>
	/// ��ʽ�������������ڵ�ͼ�ϳ�������ʾ������
	/// </summary>
	class BoxAnimation {
	protected:
		std::vector<Frame>* framesUp;        //��������
		long long           upLastShown;     //����������һ����ʾ��ʱ��
		std::vector<Frame>* framesLeft;      //�󷽶���
		long long           leftLastShown;   //�󷽶�����һ����ʾ��ʱ��
		std::vector<Frame>* framesRight;     //�ҷ�����
		long long           rightLastShown;  //�ҷ�������һ����ʾ��ʱ��
	public:
		/// <summary>
		/// ָʾ����ͼ��Ԥ�ϳ�
		/// ����Ҫ�����򶯻�ʱ��һ�£������׳�InvalidOperationException��
		/// ���ڷ��첽�����»�������
		/// </summary>
		void DoPresynthetise();

		/// <summary>
		/// �������á�ѯ����һ֡ͼ���Ƿ��Ѿ�׼����ʾ���첽�����ã�
		/// </summary>
		/// <returns>��Ϊtrue����׼�����</returns>
		bool IsNextFrameReady();

		Texture* GetCurrentTextureUp();
		Texture* GetCurrentTextureLeft();
		Texture* GetCurrentTextureRight();

		/// <summary>
		/// ��ȡ��ǰҪ��ʾ������ͼ��
		/// </summary>
		/// <returns>ָ��ϳɺõ�ͼ���ָ��</returns>
		Texture* GetSynthetizedTexture();
	};

	/// <summary>
	/// ����������
	/// </summary>
	class GroundColumnTexture {
	protected:
		Texture* textureU; /*���ϵ��������������*/
		Texture* textureA; /*�����·�������*/
		Texture* textureB; /*�����·�������Ϊ��ʱ����A��*/
		//bool isSynthetized;/*ָʾ�����Ƿ��Ѿ��ϳ�����*/

		//void Synthetize(); //Ԥ�ϳ����в�ͨ��
	public:
		GroundColumnTexture(Texture* p_upTexture, Texture* p_sideTexture);

		GroundColumnTexture(Texture* p_up, Texture* p_A, Texture* p_B);

		GroundColumnTexture(Texture** p_textureArray);

		void SetTopTexture(Texture* p_texture);

		void SetATexture(Texture* p_texture);

		void SetBTexture(Texture* p_texture);

		//void Synthetize(); //�ⲻ�ǲ��ʸøɵ�

		//Frame* GetCurrentFrame();

		//void GetCurrentTexture();

		Texture* GetTopTexture();

		Texture* GetATexture();

		Texture* GetBTexture();
	};
}//End of WelkinEngine