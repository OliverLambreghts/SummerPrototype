#include "pch.h"
#include "FireProc.h"

FireProc::FireProc(float chance, std::shared_ptr<GameObject> player)
	: m_Chance{ chance },
	m_pPlayer{ player }
{
}

bool FireProc::IsProcActive()
{
	const auto randNr = static_cast<float>(rand() % 100);
	return (randNr <= m_Chance);
}

void FireProc::Trigger()
{
	// Applies a fire dot on the enemy target here
}
