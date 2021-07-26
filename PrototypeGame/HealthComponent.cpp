#include "pch.h"
#include "HealthComponent.h"
#include <cassert>
#include <iostream>

HealthComponent::HealthComponent(int health)
	: m_Health{ health },
	m_ProcTimer{},
	m_CurrentProcAmount{},
	m_MaxHealth{ health }
{
}

void HealthComponent::Update(float elapsedSec, GameObject& /*obj*/)
{
	if (!m_pProc)
		return;

	m_ProcTimer += elapsedSec;

	if (m_ProcTimer < 1.f)
		return;

	m_ProcTimer = 0.f;
	++m_CurrentProcAmount;

	m_pProc->Trigger(m_Health);

	if (m_CurrentProcAmount == m_pProc->GetDuration())
	{
		m_pProc.reset();
		m_CurrentProcAmount = 0;
		m_ProcTimer = 0.f;
	}
}

void HealthComponent::AddHealth(int health)
{
	if (m_Health + health > m_MaxHealth)
		m_Health = m_MaxHealth;
	else
		m_Health += health;
	std::cout << "Entity now has " << m_Health << " HP\n";
}

bool HealthComponent::IsDead() const
{
	return m_Health <= 0;
}

void HealthComponent::SetProc(std::shared_ptr<BaseProc> proc)
{
	assert(proc);

	m_pProc = proc;
}
