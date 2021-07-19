#include "pch.h"
#include "PlayerMovementComponent.h"
#include <iostream>

PlayerMovementComponent::PlayerMovementComponent()
	: m_DirectionX{ DirectionX::none },
	m_Position{},
	m_Speed{ 200 },
	m_DirectionY{ DirectionY::none },
	m_State{ State::idle },
	m_KBVelocity{},
	m_ActiveKBTimer{},
	m_IsKnockedBack{ false }
{
}

void PlayerMovementComponent::Update(float elapsedSec, GameObject& /*obj*/)
{
	if (ApplyKnockBack(elapsedSec))
		return;
	
	if (m_State == State::idle)
		return;

	HandleXMovement(elapsedSec);
	HandleYMovement(elapsedSec);
}

void PlayerMovementComponent::HandleXMovement(float elapsedSec)
{
	switch (m_DirectionX)
	{
	case DirectionX::left:
		m_Position.x -= m_Speed * elapsedSec;
		break;
	case DirectionX::right:
		m_Position.x += m_Speed * elapsedSec;
		break;
	}
}

void PlayerMovementComponent::HandleYMovement(float elapsedSec)
{
	switch (m_DirectionY)
	{
	case DirectionY::down:
		m_Position.y -= m_Speed * elapsedSec;
		break;
	case DirectionY::up:
		m_Position.y += m_Speed * elapsedSec;
		break;
	}
}

bool PlayerMovementComponent::ApplyKnockBack(float elapsedSec)
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
	m_Speed = speed;
}

void PlayerMovementComponent::SetState(State state)
{
	m_State = state;
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

void PlayerMovementComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& PlayerMovementComponent::GetPosition() const
{
	return m_Position;
}
