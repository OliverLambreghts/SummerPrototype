#include "pch.h"
#include "FireProc.h"

#include <iostream>

FireProc::FireProc(float chance, std::shared_ptr<GameObject> player)
	: m_Chance{ chance },
	m_pPlayer{ player },
	m_Duration{ 4.f },
	m_Damage{ 4 }
{
}

bool FireProc::IsProcActive()
{
	const auto randNr = static_cast<float>(rand() % 100);
	return (randNr <= m_Chance);
}

void FireProc::Trigger(int& health)
{
	// Applies a fire dot on the enemy target here
	health -= m_Damage;
	std::cout << "Did " << m_Damage << " fire damage!\n";
}

float FireProc::GetDuration() const
{
	return m_Duration;
}
