#include "pch.h"
#include "EnemyMovementComponent.h"
#include <iostream>
#include "ActivityComponent.h"
#include "DebugEnemyComponent.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"
#include "Sprite.h"
#include "SpriteRenderComponent.h"

EnemyMovementComponent::EnemyMovementComponent(float speed, const Point2f& pos)
	: m_Position{ pos },
	m_Speed{ speed },
	m_IsKnockedBack{ false },
	m_ActiveKBTimer{},
	m_KBVelocity{}
{
}

void EnemyMovementComponent::Update(float elapsedSec, GameObject& obj)
{
	if (!obj.GetComponent<ActivityComponent>()->GetActivity())
		return;

	auto player = obj.GetComponent<DebugEnemyComponent>()->GetPlayer();
	const auto playerPos = obj.GetComponent<DebugEnemyComponent>()->GetPlayer()->GetComponent<PlayerMovementComponent>()->GetPosition();

	float width = obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth();
	const float height = obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight();

	// Apply knockback when hit by player
	if (ApplyKnockBack(elapsedSec))
		return;

	if (utils::IsOverlapping(Rectf{ playerPos.x, playerPos.y,
		player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
		player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() },
		Rectf{ m_Position.x, m_Position.y, width, height }))
	{
		obj.GetComponent<SpriteRenderComponent>()->Idle();
		return;
	}

	obj.GetComponent<SpriteRenderComponent>()->Move();

	// Calculate desired velocity (for the direction)
	Vector2f velocity = playerPos - m_Position;
	velocity = velocity.Normalized();
	velocity *= m_Speed;

	m_Position += velocity * elapsedSec;

	DetermineDirection(velocity, obj);
}

void EnemyMovementComponent::DetermineDirection(const Vector2f& velocity, GameObject& obj) const
{
	double angle = atan2(velocity.y, velocity.x);
	angle = angle * 180 / M_PI;

	if (angle > 0)
	{
		if (angle > 45 && angle < 135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::up);
		}
		else if (angle > 135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::left);
		}
		else if (angle < 45)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::right);
		}
	}
	else
	{
		if (angle < -45 && angle > -135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::down);
		}
		else if (angle < -135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::left);
		}
		else if (angle > -45)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::right);
		}
	}
}

bool EnemyMovementComponent::ApplyKnockBack(float elapsedSec)
{
	if (m_IsKnockedBack)
	{
		m_ActiveKBTimer += elapsedSec;

		auto KBVel = m_KBVelocity.Normalized();
		KBVel *= m_KBSpeed;

		m_Position += KBVel * elapsedSec;

		if (m_ActiveKBTimer >= m_KnockBackTimer)
		{
			m_ActiveKBTimer = 0.f;
			m_IsKnockedBack = false;
		}
		return true;
	}
	return false;
}

void EnemyMovementComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& EnemyMovementComponent::GetPosition() const
{
	return m_Position;
}

void EnemyMovementComponent::ActivateKnockBack(const Point2f& playerPos)
{
	m_IsKnockedBack = true;

	m_KBVelocity = Vector2f{ playerPos, m_Position };
}

