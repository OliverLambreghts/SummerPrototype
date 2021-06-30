#pragma once
#include "Texture.h"

class Sprite
{
public:
	Sprite(const std::string& fileName, int nrCols, int nrRows, float frameSec);
	Sprite(const Sprite& sprite) = delete;
	Sprite(Sprite&& sprite) noexcept;

	Sprite& operator=(const Sprite& sprite) = delete;
	Sprite& operator=(Sprite&& sprite) noexcept;

	~Sprite();
	void Update(float elapsedSec);
	void Draw(const Point2f& pos, float scale) const;
	void Draw(const Rectf& destRect) const;
	void Draw(const Rectf& destRect, const Rectf& srcRect) const;
	float GetFrameWidth() const;
	float GetFrameHeight() const;
	int GetActFrame();
	int GetRows();
	int GetCols();
	void SetFrameSec(float frameSec);
	void ResetSprite();
private:
	Texture* m_pTexture;
	int m_Cols;
	int m_Rows;
	float m_FrameSec;
	float m_AccuSec;
	int m_ActFrame;
};