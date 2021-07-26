#include "pch.h"
#include "SpeedEffect.h"
#include <iostream>
#include "PlayerMovementComponent.h"

SpeedEffect::SpeedEffect(float amount, float duration)
	: m_Amount{ amount },
	m_Duration{ duration }
{
}

void SpeedEffect::ApplyEffect(std::shared_ptr<GameObject> obj)
{
	std::cout << "Speed increased for " << m_Duration << " seconds\n";
	
	obj->GetComponent<PlayerMovementComponent>()->SetSpeed(
		obj->GetComponent<PlayerMovementComponent>()->GetSpeed() * m_Amount);

	obj->GetComponent<PlayerMovementComponent>()->SetEffect(this);
}

float SpeedEffect::GetAmount() const
{
	return m_Amount;
}

float SpeedEffect::GetDuration() const
{
	return m_Duration;
}
