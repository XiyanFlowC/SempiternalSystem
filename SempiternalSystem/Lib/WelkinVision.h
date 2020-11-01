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
	/// 材质贴图
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
		/// 设置纹理，如果需要销毁上一个纹理，请利用返回值
		/// </summary>
		/// <param name="texture">纹理数据地址</param>
		/// <param name="p_height">纹理高度</param>
		/// <param name="p_width">纹理宽度</param>
		/// <returns>指向上一个纹理</returns>
		unsigned char* SetTexture(unsigned char* texture, int p_height, int p_width);

		inline unsigned char* GetTextureData()
		{
			return data;
		}
	};

	/// <summary>
	/// 帧，带时间信息的贴图数据
	/// </summary>
	struct Frame {
		long long sustainTime;
		Texture* m_texture;
	};

	/// <summary>
	/// 各向动画。用于人物或地图对象显示
	/// </summary>
	class Animation {

	protected:
		std::vector<Frame> *framesA;     /*向左下方的纹理*/
		std::vector<Frame> *framesB;     /*向右下方的纹理*/
		std::vector<Frame> *framesC;     /*向右上方的纹理（为空时复用A）*/
		std::vector<Frame> *framesD;     /*向左上方的纹理（为空时复用B）*/
		std::vector<Frame> *framesUp;    /*向顶部的纹理*/
		std::vector<Frame> *framesDown;  /*向底部的纹理*/
		std::vector<Frame> *framesLeft;  /*向左的纹理*/
		std::vector<Frame> *framesRight; /*向右的纹理*/
		int                currentFrame; /*帧计数*/

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
		/// 以给定的帧序列数组初始化动画
		/// </summary>
		/// <param name="p_textureArray">所给的帧序列数组</param>
		Animation(std::vector<Frame>** p_textureArray);

		/// <summary>
		/// 以给定的帧序列初始化动画，参数留空表示不可用
		/// </summary>
		/// <param name="p_A">左下方的帧序列</param>
		/// <param name="p_B">右下方的帧序列</param>
		/// <param name="p_C">右上方的帧序列</param>
		/// <param name="p_D">左上方的帧序列</param>
		/// <param name="p_Up">向顶部的帧序列</param>
		/// <param name="p_Down">向底部的帧序列</param>
		/// <param name="p_Left">向左方的帧序列</param>
		/// <param name="p_Right">向右方的帧序列</param>
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
		/// 以给定的帧序列初始化动画
		/// </summary>
		/// <param name="p_A">左下方的帧序列</param>
		/// <param name="p_B">右下方的帧序列</param>
		Animation(std::vector<Frame>* p_A, std::vector<Frame>* p_B);

		/// <summary>
		/// 改换朝向
		/// </summary>
		/// <param name="p_face">朝向</param>
		inline void SwitchFacing(FaceDirection p_face)
		{
			m_face = p_face;
		}

		/// <summary>
		/// 【不可用】帧是否准备就绪（异步处理时用）
		/// </summary>
		/// <returns>true表示准备完毕</returns>
		bool IsNextFrameReady();

		/// <summary>
		/// 【不可用】释放为异步处理
		/// </summary>
		void Asyn();

		Texture *doFrameUpdate(std::vector<Frame>* p_targetFrames);

		/// <summary>
		/// 获取当前帧图像
		/// </summary>
		/// <returns>指向当前应显示帧的指针</returns>
		Texture *GetCurrentTexture();
	};

	/// <summary>
	/// 盒式动画。适用于在地图上呈立体显示的物体
	/// </summary>
	class BoxAnimation {
	protected:
		std::vector<Frame>* framesUp;        //顶部动画
		long long           upLastShown;     //顶部动画上一次显示的时间
		std::vector<Frame>* framesLeft;      //左方动画
		long long           leftLastShown;   //左方动画上一次显示的时间
		std::vector<Frame>* framesRight;     //右方动画
		long long           rightLastShown;  //右方动画上一次显示的时间
	public:
		/// <summary>
		/// 指示进行图形预合成
		/// （需要各方向动画时长一致，否则抛出InvalidOperationException）
		/// （在非异步条件下会阻塞）
		/// </summary>
		void DoPresynthetise();

		/// <summary>
		/// 【不可用】询问下一帧图像是否已经准备显示（异步处理用）
		/// </summary>
		/// <returns>若为true则已准备完毕</returns>
		bool IsNextFrameReady();

		Texture* GetCurrentTextureUp();
		Texture* GetCurrentTextureLeft();
		Texture* GetCurrentTextureRight();

		/// <summary>
		/// 获取当前要显示的整个图像
		/// </summary>
		/// <returns>指向合成好的图像的指针</returns>
		Texture* GetSynthetizedTexture();
	};

	/// <summary>
	/// 地面柱材质
	/// </summary>
	class GroundColumnTexture {
	protected:
		Texture* textureU; /*向上的纹理或整个纹理*/
		Texture* textureA; /*向左下方的纹理*/
		Texture* textureB; /*向右下方的纹理（为空时复用A）*/
		//bool isSynthetized;/*指示材质是否已经合成完了*/

		//void Synthetize(); //预合成是行不通的
	public:
		GroundColumnTexture(Texture* p_upTexture, Texture* p_sideTexture);

		GroundColumnTexture(Texture* p_up, Texture* p_A, Texture* p_B);

		GroundColumnTexture(Texture** p_textureArray);

		void SetTopTexture(Texture* p_texture);

		void SetATexture(Texture* p_texture);

		void SetBTexture(Texture* p_texture);

		//void Synthetize(); //这不是材质该干的

		//Frame* GetCurrentFrame();

		//void GetCurrentTexture();

		Texture* GetTopTexture();

		Texture* GetATexture();

		Texture* GetBTexture();
	};
}//End of WelkinEngine