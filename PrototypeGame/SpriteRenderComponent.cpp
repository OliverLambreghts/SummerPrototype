#include "pch.h"
#include "SpriteRenderComponent.h"
#include <iostream>
#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "Sprite.h"
#include "utils.h"

SpriteRenderComponent::SpriteRenderComponent(const std::string& fileName, int nrCols, int nrRows, float frameSec, int rowToRender)
	: m_pSprite{ std::make_shared<Sprite>(fileName, nrCols, nrRows, frameSec) },
	m_RenderPos{},
	m_RowToRender{ rowToRender },
	m_IsIdle{ true },
	m_IsMirrored{ false }
{

}

void SpriteRenderComponent::Update(float elapsedSec, GameObject& obj)
{
	const auto newPos = obj.GetComponent<MovementComponent>()->GetPosition();
	if (m_RenderPos.x != newPos.x || m_RenderPos.y != newPos.y)
		m_RenderPos = newPos;

	m_pSprite->Update(elapsedSec);
}

void SpriteRenderComponent::ChangeRenderDirection(Direction direction)
{
	m_RowToRender = static_cast<int>(direction);
	m_ActiveDirections.push_back(direction);
}

void SpriteRenderComponent::Render() const
{
	Rectf srcRect{};
	srcRect.width = m_pSprite->GetFrameWidth();
	srcRect.height = m_pSprite->GetFrameHeight();
	srcRect.bottom = (m_pSprite->GetFrameHeight() * m_pSprite->GetRows()) - (m_pSprite->GetFrameHeight() * m_RowToRender);
	if (!m_IsIdle)
		srcRect.left = m_pSprite->GetActFrame() * srcRect.width;
	else
		srcRect.left = 0.f;

	glPushMatrix();
	glTranslatef(m_RenderPos.x, m_RenderPos.y, 0.f);
	// Check for horizontal mirroring
	if(m_IsMirrored)
		glScalef(-1.f, 1.f, 1.f);
	m_pSprite->Draw(Rectf{ 0.f, 0.f, m_pSprite->GetFrameWidth(), m_pSprite->GetFrameHeight() }, srcRect);
	glPopMatrix();

	/*m_pSprite->Draw(Rectf{ m_RenderPos.x, m_RenderPos.y, m_pSprite->GetFrameWidth(),m_pSprite->GetFrameWidth() },
		srcRect);*/

		// --- DEBUG RENDER HITBOX ---
		/*const float width = GetSprite().GetFrameWidth();
		const float height = GetSprite().GetFrameHeight();
		utils::DrawRect(m_RenderPos, width, height);*/
		// --- DEBUG RENDER HITBOX ---
}

void SpriteRenderComponent::Idle()
{
	m_IsIdle = true;
}

void SpriteRenderComponent::Move()
{
	m_IsIdle = false;
}

Sprite& SpriteRenderComponent::GetSprite() const
{
	return *m_pSprite;
}

SpriteRenderComponent::Direction SpriteRenderComponent::GetDirection() const
{
	if (m_ActiveDirections.empty())
		return Direction::down;

	return m_ActiveDirections.back();
}

void SpriteRenderComponent::ToggleMirror()
{
	m_IsMirrored = !m_IsMirrored;
}

bool SpriteRenderComponent::IsMirrored() const
{
	return m_IsMirrored;
}
