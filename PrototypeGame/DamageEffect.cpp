#include "pch.h"
#include "DamageEffect.h"
#include <iostream>
#include "InventoryComponent.h"

DamageEffect::DamageEffect(float amount, float duration)
	: m_Amount{ amount },
	m_Duration{ duration }
{
}

void DamageEffect::ApplyEffect(std::shared_ptr<GameObject> obj)
{
	std::cout << "Damage increased for " << m_Duration << " seconds!\n";
	
	obj->GetComponent<InventoryComponent>()->SetEffect(this);
}

float DamageEffect::GetAmount() const
{
	return m_Amount;
}

float DamageEffect::GetDuration() const
{
	return m_Duration;
}
