#include "pch.h"
#include "HealEffect.h"
#include <iostream>
#include "HealthComponent.h"

HealEffect::HealEffect(float amount)
	: m_Amount{ amount }
{
}

void HealEffect::ApplyEffect(std::shared_ptr<GameObject> obj)
{
	std::cout << "Healed for " << m_Amount << '\n';
	
	obj->GetComponent<HealthComponent>()->AddHealth(static_cast<int>(m_Amount));
}

float HealEffect::GetAmount() const
{
	return m_Amount;
}

float HealEffect::GetDuration() const
{
	return -1;
}
