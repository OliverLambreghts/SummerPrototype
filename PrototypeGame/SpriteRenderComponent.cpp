#include "pch.h"
#include "SpriteRenderComponent.h"

#include <iostream>


#include "GameObject.h"
#include "MovementComponent.h"
#include "Sprite.h"

SpriteRenderComponent::SpriteRenderComponent(const std::string& fileName, int nrCols, int nrRows, float frameSec, int colToRender)
	: m_pSprite{ std::make_shared<Sprite>(fileName, nrCols, nrRows, frameSec) },
	m_RenderPos{},
	m_RowToRender{ colToRender },
	m_IsIdle{ true }
{

}

void SpriteRenderComponent::Update(float elapsedSec, GameObject& obj)
{
	auto newPos = obj.GetComponent<MovementComponent>()->GetPosition();
	if (m_RenderPos.x != newPos.x || m_RenderPos.y != newPos.y)
		m_RenderPos = newPos;

	m_pSprite->Update(elapsedSec);
}

void SpriteRenderComponent::ChangeRenderDirection(Direction direction)
{
	m_RowToRender = static_cast<int>(direction);
}

void SpriteRenderComponent::Render() const
{
	Rectf srcRect{};
	srcRect.width = m_pSprite->GetFrameWidth();
	srcRect.height = m_pSprite->GetFrameHeight();
	//srcRect.bottom = (m_pSprite->GetActFrame() / m_pSprite->GetCols()) * srcRect.height;
	srcRect.bottom = (m_pSprite->GetFrameHeight() * m_pSprite->GetRows()) - (m_pSprite->GetFrameHeight() * m_RowToRender);
	if (!m_IsIdle)
		srcRect.left = m_pSprite->GetActFrame() * srcRect.width;
	else
		srcRect.left = 0.f;

	m_pSprite->Draw(Rectf{ m_RenderPos.x, m_RenderPos.y, m_pSprite->GetFrameWidth(),m_pSprite->GetFrameWidth() },
		srcRect);
}

void SpriteRenderComponent::Idle()
{
	m_IsIdle = true;
}

void SpriteRenderComponent::Move()
{
	m_IsIdle = false;
}

const Sprite& SpriteRenderComponent::GetSprite() const
{
	return *m_pSprite;
}