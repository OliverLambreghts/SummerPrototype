#include "pch.h"
#include "ActivityComponent.h"

ActivityComponent::ActivityComponent()
	: m_IsActive{ false }
{
}

void ActivityComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

bool ActivityComponent::GetActivity() const
{
	return m_IsActive;
}

void ActivityComponent::Activate()
{
	m_IsActive = true;
}

void ActivityComponent::Deactivate()
{
	m_IsActive = false;
}
