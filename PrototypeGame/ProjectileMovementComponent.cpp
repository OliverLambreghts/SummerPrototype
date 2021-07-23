#include "pch.h"
#include "ProjectileMovementComponent.h"

ProjectileMovementComponent::ProjectileMovementComponent(float speed, const Point2f pos, const Vector2f& fwdVec)
	: m_Position{ pos },
	m_Speed{ speed },
	m_ForwardVec{ fwdVec }
{
}

void ProjectileMovementComponent::Update(float elapsedSec, GameObject& /*obj*/)
{
	const auto velocity = m_ForwardVec * m_Speed;
	m_Position += velocity * elapsedSec;
}

void ProjectileMovementComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& ProjectileMovementComponent::GetPosition() const
{
	return m_Position;
}
