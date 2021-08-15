#include "pch.h"
#include "DebugEnemyComponent.h"
#include <iostream>
#include "ActivityComponent.h"
#include "CoinComponent.h"
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
	// Je zet de loot in de volgorde van laagste droprate naar hoogste droprate. Als je als eerste item 20 meegeeft en voor
	// de tweede 40, dan hebben beiden 20% droprate want de LootComponent checked eerst de eerste item of de rate <= 20 en zo niet
	// gaat die voor de tweede checken als die <= 40 wat betekent dat beide items 20% kans hebben om te spawnen.
	// Health potion 20%
	clone->AddComponent(std::make_shared<LootComponent>());
	std::vector<std::shared_ptr<BaseEffect>> effects;
	effects.push_back(std::make_shared<HealEffect>(15.f));
	const auto healthPot = std::make_shared<ConsumableComponent>("Small Healing Potion", effects, "RedPotion.png");
	clone->GetComponent<LootComponent>()->AddItem(20, healthPot);

	// Coin 20%
	const auto coin = std::make_shared<CoinComponent>("Coin.png");
	clone->GetComponent<LootComponent>()->AddItem(40, coin);
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
