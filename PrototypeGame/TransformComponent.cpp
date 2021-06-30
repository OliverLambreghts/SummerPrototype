#include "pch.h"
#include "TransformComponent.h"

void TransformComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

void TransformComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& TransformComponent::GetPosition() const
{
	return m_Position;
}
