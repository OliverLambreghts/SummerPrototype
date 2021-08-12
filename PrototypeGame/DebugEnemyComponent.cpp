#include "pch.h"
#include "DebugEnemyComponent.h"
#include <iostream>
#include "ActivityComponent.h"
#include "CombatComponent.h"
#include "ConsumableComponent.h"
#include "EnemyMovementComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "HealEffect.h"
#include "HealthComponent.h"
#include "LootComponent.h"
#include "SpriteRenderComponent.h"

DebugEnemyComponent::DebugEnemyComponent(int health, int power, int speed, const Point2f& pos, std::shared_ptr<GameObject> player)
	: m_Health{ health }, m_Power{ power }, m_Speed{ speed },
	m_Position{ pos },
	m_pPlayer{ player }
{
}

std::shared_ptr<GameObject> DebugEnemyComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<DebugEnemyComponent>(m_Health, m_Power, m_Speed, m_Position, m_pPlayer));
	clone->AddComponent(std::make_shared<EnemyMovementComponent>(static_cast<float>(m_Speed), m_Position));
	clone->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/TestSheet.png", 9, 4, 1.f / 10.f, 1));
	clone->GetComponent<SpriteRenderComponent>()->Move();
	clone->AddComponent(std::make_shared<ActivityComponent>());
	clone->AddComponent(std::make_shared<HealthComponent>(m_Health));
	clone->AddComponent(std::make_shared<CombatComponent>());

	// --- LOOT ---
	clone->AddComponent(std::make_shared<LootComponent>());
	std::vector<std::shared_ptr<BaseEffect>> effects;
	effects.push_back(std::make_shared<HealEffect>(15.f));
	const auto healthPot = std::make_shared<ConsumableComponent>("Small Healing Potion", effects, "RedPotion.png");
	clone->GetComponent<LootComponent>()->AddItem(20, healthPot);
	// --- LOOT ---
	
	return clone;
}

void DebugEnemyComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

std::shared_ptr<GameObject> DebugEnemyComponent::GetPlayer() const
{
	return m_pPlayer;
}

int DebugEnemyComponent::GetPower() const
{
	return m_Power;
}
