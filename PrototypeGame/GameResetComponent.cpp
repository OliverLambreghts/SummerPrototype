#include "pch.h"
#include "GameResetComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "SceneManager.h"

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
	SceneManager::GetInstance().Reset(m_pInitialize);
}
