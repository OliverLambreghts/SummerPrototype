#include "pch.h"
#include "EnemyMovementComponent.h"

EnemyMovementComponent::EnemyMovementComponent(float speed, const Point2f& pos)
	:m_DirectionX{ DirectionX::none },
	m_Position{ pos },
	m_Speed{ speed },
	m_DirectionY{ DirectionY::none },
	m_State{ State::idle }
{
}

void EnemyMovementComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

void EnemyMovementComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& EnemyMovementComponent::GetPosition() const
{
	return m_Position;
}
