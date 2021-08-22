#include "pch.h"
#include "Sprite.h"
#include <iostream>

Sprite::Sprite(const std::string& fileName, int nrCols, int nrRows, float frameSec)
	: m_pTexture{ new Texture{fileName} },
	m_Cols{nrCols},
	m_Rows{nrRows},
	m_FrameSec{frameSec},
	m_ActFrame{0},
	m_AccuSec{0.0f}
{

}

Sprite::Sprite(Sprite&& rhs) noexcept
	: m_pTexture{ rhs.m_pTexture },
	m_Cols{ rhs.m_Cols },
	m_Rows{ rhs.m_Cols },
	m_FrameSec{ rhs.m_FrameSec },
	m_AccuSec{ rhs.m_AccuSec },
	m_ActFrame{ rhs.m_ActFrame }
{
	std::cout << "Move constructor\n";
	rhs.m_pTexture = nullptr;
	rhs.m_Cols = 0;
	rhs.m_Rows = 0;
	rhs.m_FrameSec = 0;
	rhs.m_AccuSec = 0;
	rhs.m_ActFrame = 0;
}

Sprite& Sprite::operator=(Sprite&& rhs) noexcept
{
	if (&rhs != this)
	{
		delete m_pTexture;
		m_pTexture = rhs.m_pTexture;
		m_Cols = rhs.m_Cols;
		m_Rows = rhs.m_Rows;
		m_FrameSec = rhs.m_FrameSec;
		m_AccuSec = rhs.m_AccuSec;
		m_ActFrame = rhs.m_ActFrame;
		rhs.m_pTexture = nullptr;
		rhs.m_Cols = 0;
		rhs.m_Rows = 0;
		rhs.m_FrameSec = 0;
		rhs.m_AccuSec = 0;
		rhs.m_ActFrame = 0;
	}
	return *this;
}

Sprite::~Sprite()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Sprite::Update(float elapsedSec)
{
	int totFrames{ m_Cols * m_Rows };
	m_AccuSec += elapsedSec;
	if (m_AccuSec > m_FrameSec)
	{
		// Determine next frame number
		++m_ActFrame %= totFrames;
		m_AccuSec -= m_FrameSec;
	}
}

void Sprite::Draw(const Point2f& pos, float scale) const
{
	int col{ m_ActFrame % m_Cols };
	int row{ m_ActFrame / m_Cols };
	
	Rectf srcRect{};
	srcRect.width = m_pTexture->GetWidth() / m_Cols;
	srcRect.height = m_pTexture->GetHeight() / m_Rows;
	srcRect.left = srcRect.width * col;
	srcRect.bottom = srcRect.height * (row + 1);

	Rectf destRect{};
	destRect.left = pos.x;
	destRect.bottom = pos.y;
	destRect.width = srcRect.width * scale;
	destRect.height = srcRect.height * scale;

	m_pTexture->Draw(destRect, srcRect);
}

void Sprite::Draw(const Rectf& destRect) const
{
	Rectf srcRect{};
	srcRect.width = GetFrameWidth();
	srcRect.height = GetFrameHeight();
	srcRect.bottom = (m_ActFrame / m_Cols) * srcRect.height;
	srcRect.left = m_ActFrame * srcRect.width;

	m_pTexture->Draw(destRect, srcRect);
}

void Sprite::Draw(const Rectf& destRect, const Rectf& srcRect) const
{
	m_pTexture->Draw(destRect, srcRect);
}

float Sprite::GetFrameWidth() const
{
	return m_pTexture->GetWidth() / m_Cols;
}

float Sprite::GetFrameHeight() const
{
	return  m_pTexture->GetHeight() / m_Rows;
}

int Sprite::GetActFrame() const
{
	return m_ActFrame;
}

int Sprite::GetRows() const
{
	return m_Rows;
}
int Sprite::GetCols() const
{
	return m_Cols;
}

void Sprite::SetFrameSec(float frameSec)
{
	m_FrameSec = frameSec;
}

void Sprite::ResetSprite()
{
	m_ActFrame = 0;
	m_AccuSec = 0.f;
}