#include "pch.h"
#include "PlayerMovementComponent.h"
#include <iostream>
#include "Game.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

PlayerMovementComponent::PlayerMovementComponent()
	: m_DirectionX{ DirectionX::none },
	m_Position{},
	m_Speed{ 200 },
	m_OldSpeed{  },
	m_DirectionY{ DirectionY::none },
	m_State{ State::idle },
	m_KBVelocity{},
	m_ActiveKBTimer{},
	m_IsKnockedBack{ false },
	m_EffectTimer{},
	m_ObstacleCollisionData{}
{
}

void PlayerMovementComponent::Update(float elapsedSec, GameObject& obj)
{
	UpdateEffectTimer(elapsedSec);

	if (ApplyKnockBack(elapsedSec, obj))
		return;

	if (m_State == State::idle)
		return;

	HandleXMovement(elapsedSec, obj);
	HandleYMovement(elapsedSec, obj);

	m_ObstacleCollisionData.first = false;
	m_ObstacleCollisionData.second.clear();
}

void PlayerMovementComponent::HandleXMovement(float elapsedSec, GameObject& obj)
{
	switch (m_DirectionX)
	{
	case DirectionX::left:
		if (m_ObstacleCollisionData.first)
		{
			for(auto& direction : m_ObstacleCollisionData.second)
			{
				if (direction.directionX == DirectionX::left)
					return;
			}
		}
		
		if (m_Position.x <= 0.f)
			return;

		m_Position.x -= m_Speed * elapsedSec;
		break;
	case DirectionX::right:
		if (m_ObstacleCollisionData.first)
		{
			for (auto& direction : m_ObstacleCollisionData.second)
			{
				if (direction.directionX == DirectionX::right)
					return;
			}
		}

		if (m_Position.x + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() >= Game::GetWindowDimension())
			return;

		m_Position.x += m_Speed * elapsedSec;
		break;
	}
}

void PlayerMovementComponent::HandleYMovement(float elapsedSec, GameObject& obj)
{
	switch (m_DirectionY)
	{
	case DirectionY::down:
		if (m_ObstacleCollisionData.first)
		{
			for (auto& direction : m_ObstacleCollisionData.second)
			{
				if (direction.directionY == DirectionY::down)
					return;
			}
		}

		if (m_Position.y <= 0.f)
			return;

		m_Position.y -= m_Speed * elapsedSec;
		break;
	case DirectionY::up:
		if (m_ObstacleCollisionData.first)
		{
			for (auto& direction : m_ObstacleCollisionData.second)
			{
				if (direction.directionY == DirectionY::up)
					return;
			}
		}

		if (m_Position.y + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() >= Game::GetWindowDimension())
			return;

		m_Position.y += m_Speed * elapsedSec;
		break;
	}
}

bool PlayerMovementComponent::ApplyKnockBack(float elapsedSec, GameObject& obj)
{
	if (IsAgainstWall(obj) || m_ObstacleCollisionData.first)
		return false;

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

void PlayerMovementComponent::UpdateEffectTimer(float elapsedSec)
{
	if (m_pEffect)
	{
		m_EffectTimer += elapsedSec;

		if(m_EffectTimer >= m_pEffect->GetDuration())
		{
			m_EffectTimer = 0.f;
			m_pEffect = nullptr;
			m_Speed = m_OldSpeed;
		}
	}
}

void PlayerMovementComponent::SetObstacleCollisionData(DirectionX directionX, DirectionY directionY)
{
	const ObstacleCollisionData directions{ directionX, directionY };
	
	m_ObstacleCollisionData.second.push_back(directions);
	m_ObstacleCollisionData.first = true;
}

bool PlayerMovementComponent::IsAgainstWall(GameObject& obj) const
{
	if (m_Position.y <= 0.f)
		return true;
	if (m_Position.y + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() >= Game::GetWindowDimension())
		return true;
	if (m_Position.x <= 0.f)
		return true;
	if (m_Position.x + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() >= Game::GetWindowDimension())
		return true;

	return false;
}

void PlayerMovementComponent::Move(DirectionX* directionX, DirectionY* directionY)
{
	m_State = State::moving;
	if (directionX)
		m_DirectionX = *directionX;
	if (directionY)
		m_DirectionY = *directionY;

	if (m_DirectionX == DirectionX::none && m_DirectionY == DirectionY::none)
		m_State = State::idle;
}

void PlayerMovementComponent::SetSpeed(float speed)
{
	m_OldSpeed = m_Speed;
	m_Speed = speed;
}

void PlayerMovementComponent::SetState(State state)
{
	m_State = state;
}

float PlayerMovementComponent::GetSpeed() const
{
	return m_Speed;
}

PlayerMovementComponent::State PlayerMovementComponent::GetState() const
{
	return m_State;
}

std::pair<PlayerMovementComponent::DirectionX, PlayerMovementComponent::DirectionY> PlayerMovementComponent::GetDirections() const
{
	return std::pair<DirectionX, DirectionY>(m_DirectionX, m_DirectionY);
}

void PlayerMovementComponent::ActivateKnockBack(const Point2f& enemyPos)
{
	m_IsKnockedBack = true;

	m_KBVelocity = Vector2f{ enemyPos, m_Position };
}

void PlayerMovementComponent::SetEffect(BaseEffect* pEffect)
{
	m_pEffect = pEffect;
}

void PlayerMovementComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& PlayerMovementComponent::GetPosition() const
{
	return m_Position;
}
