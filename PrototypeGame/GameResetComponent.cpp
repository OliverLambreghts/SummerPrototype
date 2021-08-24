#include "pch.h"
#include "GameResetComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "SceneManager.h"
#include "StatManager.h"

GameResetComponent::GameResetComponent(std::function<void()> initFcn)
	: m_pInitialize{ initFcn }
{
}

void GameResetComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	if (!obj.GetComponent<HealthComponent>()->IsDead())
		return;

	Reset();
}

void GameResetComponent::Reset() const
{
	// Update best stats
	if (StatManager::GetInstance().GetCurrentStats().coinsCollected > StatManager::GetInstance().GetBestStats().coinsCollected)
		StatManager::GetInstance().GetBestStats().coinsCollected = StatManager::GetInstance().GetCurrentStats().coinsCollected;
	if (StatManager::GetInstance().GetCurrentStats().roomsDiscovered > StatManager::GetInstance().GetBestStats().roomsDiscovered)
		StatManager::GetInstance().GetBestStats().roomsDiscovered = StatManager::GetInstance().GetCurrentStats().roomsDiscovered;
	if (StatManager::GetInstance().GetCurrentStats().enemiesKilled > StatManager::GetInstance().GetBestStats().enemiesKilled)
		StatManager::GetInstance().GetBestStats().enemiesKilled = StatManager::GetInstance().GetCurrentStats().enemiesKilled;
	if (StatManager::GetInstance().GetCurrentStats().itemsFound > StatManager::GetInstance().GetBestStats().itemsFound)
		StatManager::GetInstance().GetBestStats().itemsFound = StatManager::GetInstance().GetCurrentStats().itemsFound;
	if (StatManager::GetInstance().GetCurrentStats().timeSurvived > StatManager::GetInstance().GetBestStats().timeSurvived)
		StatManager::GetInstance().GetBestStats().timeSurvived = StatManager::GetInstance().GetCurrentStats().timeSurvived;

	// Update total stats
	StatManager::GetInstance().GetTotalStats().timeSurvived += StatManager::GetInstance().GetCurrentStats().timeSurvived;
	StatManager::GetInstance().GetTotalStats().coinsCollected += StatManager::GetInstance().GetCurrentStats().coinsCollected;
	StatManager::GetInstance().GetTotalStats().roomsDiscovered += StatManager::GetInstance().GetCurrentStats().roomsDiscovered;
	StatManager::GetInstance().GetTotalStats().enemiesKilled += StatManager::GetInstance().GetCurrentStats().enemiesKilled;
	StatManager::GetInstance().GetTotalStats().itemsFound += StatManager::GetInstance().GetCurrentStats().itemsFound;

	// Print all stats
	StatManager::GetInstance().PrintCurrentStats();
	StatManager::GetInstance().PrintBestStats();
	StatManager::GetInstance().PrintTotalStats();

	// Reset everything
	StatManager::GetInstance().ResetCurrentStats();
	SceneManager::GetInstance().Reset(m_pInitialize);
}
