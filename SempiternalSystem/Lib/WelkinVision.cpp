#include "WelkinVision.h"

WelkinEngine::Animation::Animation(std::vector<Frame>** p_textureArray)
{
	Animation(p_textureArray[0],
		p_textureArray[1],
		p_textureArray[2],
		p_textureArray[3],
		p_textureArray[4],
		p_textureArray[5],
		p_textureArray[6],
		p_textureArray[7]);
}

WelkinEngine::Animation::Animation(std::vector<Frame>* p_A, std::vector<Frame>* p_B, std::vector<Frame>* p_C, std::vector<Frame>* p_D, std::vector<Frame>* p_Up, std::vector<Frame>* p_Down, std::vector<Frame>* p_Left, std::vector<Frame>* p_Right)
	: framesA(p_A), framesB(p_B), framesC(p_C),
	framesD(p_D), framesUp(p_Up), framesDown(p_Down),
	framesLeft(p_Left), framesRight(p_Right), m_lastFrameShownTime(TimeLib::get_time())
{
	currentFrame = 0;
	m_face = FaceDirection::LeftDown;
}

WelkinEngine::Animation::Animation(std::vector<Frame>* p_A, std::vector<Frame>* p_B)
{
	Animation(p_A, p_B, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

WelkinEngine::Texture* WelkinEngine::Animation::doFrameUpdate(std::vector<Frame>* p_targetFrames)
{
	if (p_targetFrames == nullptr || p_targetFrames->empty())//check if value is invalid
		throw InvalidOperationException("Frames pointer is null or empty.", __LINE__, __FILE__, "Required operation need to init member frames first and has data.");
	if ((*p_targetFrames)[currentFrame].sustainTime && (*p_targetFrames)[currentFrame].sustainTime < TimeLib::get_time() - m_lastFrameShownTime) ++currentFrame;
	if (currentFrame >= p_targetFrames->size()) currentFrame = 0;
	return (*p_targetFrames)[currentFrame].m_texture;
}

WelkinEngine::Texture* WelkinEngine::Animation::GetCurrentTexture()
{
	switch (m_face)
	{
	case FaceDirection::LeftDown:
		return doFrameUpdate(framesA);
	case FaceDirection::RightDown:
		return doFrameUpdate(framesB);
	case FaceDirection::RightUp:
		if (framesC == nullptr) return doFrameUpdate(framesA);//if C side is nullptr, use A side to reduce memory using
	case FaceDirection::LeftUp:
		if (framesD == nullptr) return doFrameUpdate(framesB);
	case FaceDirection::Right:
		return doFrameUpdate(framesRight);
	case FaceDirection::Left:
		return doFrameUpdate(framesLeft);
	case FaceDirection::Up:
		return doFrameUpdate(framesUp);
	case FaceDirection::Down:
		return doFrameUpdate(framesDown);
	}
	return nullptr; 
}

WelkinEngine::Texture::Texture() : height(0), width(0), data(nullptr)
{
}

WelkinEngine::Texture::Texture(int p_height, int p_width, unsigned char* p_texture)
	: height(p_height), width(p_width), data(p_texture)
{
}

unsigned char* WelkinEngine::Texture::SetTexture(unsigned char* texture, int p_height, int p_width)
{
	auto lastptr = data;

	data = texture, height = p_height, width = p_width;

	return lastptr;
}

WelkinEngine::GroundColumnTexture::GroundColumnTexture(Texture* p_upTexture, Texture* p_sideTexture)
{
	GroundColumnTexture(p_upTexture, p_sideTexture, nullptr);
}

WelkinEngine::GroundColumnTexture::GroundColumnTexture(Texture* p_up, Texture* p_A, Texture* p_B)
	: textureU(p_up), textureA(p_A), textureB(p_B)
{
}

WelkinEngine::GroundColumnTexture::GroundColumnTexture(Texture** p_textureArray)
{
	GroundColumnTexture(p_textureArray[0], p_textureArray[1], p_textureArray[2]);
}

void WelkinEngine::GroundColumnTexture::SetTopTexture(Texture* p_texture)
{
	textureU = p_texture;
}

void WelkinEngine::GroundColumnTexture::SetATexture(Texture* p_texture)
{
	textureA = p_texture;
}

void WelkinEngine::GroundColumnTexture::SetBTexture(Texture* p_texture)
{
	textureB = p_texture;
}

WelkinEngine::Texture* WelkinEngine::GroundColumnTexture::GetTopTexture()
{
	if (textureU == nullptr) throw InvalidOperationException();
	return textureU;
}

WelkinEngine::Texture* WelkinEngine::GroundColumnTexture::GetATexture()
{
	if (textureA == nullptr) throw InvalidOperationException();
	return textureA;
}

WelkinEngine::Texture* WelkinEngine::GroundColumnTexture::GetBTexture()
{
	if (textureB == nullptr) return textureA;
	return textureB;
}
