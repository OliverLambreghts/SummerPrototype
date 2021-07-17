#include "pch.h"
#include "ActivityComponent.h"

ActivityComponent::ActivityComponent()
	: m_IsActive{ false },
	m_IsDebugActive{ true }
{
}

void ActivityComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

bool ActivityComponent::GetActivity() const
{
	if (!m_IsDebugActive) return false;
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

void ActivityComponent::DebugDeactivate()
{
	m_IsDebugActive = false;
}
