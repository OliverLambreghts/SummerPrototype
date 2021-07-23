#include "pch.h"
#include "CritProc.h"
#include <iostream>

CritProc::CritProc(float chance, std::shared_ptr<GameObject> player)
	: m_Chance{ chance },
	m_pPlayer{ player },
	m_Duration{ 1.f },
	m_Damage{ 10 }
{
}

bool CritProc::IsProcActive()
{
	const auto randNr = static_cast<float>(rand() % 100);
	return (randNr <= m_Chance);
}

void CritProc::Trigger(int& health)
{
	// Applies a fire dot on the enemy target here
	health -= m_Damage;
	std::cout << "Did " << m_Damage << " critical damage!\n";
}

float CritProc::GetDuration() const
{
	return m_Duration;
}

float CritProc::GetChance() const
{
	return m_Chance;
}