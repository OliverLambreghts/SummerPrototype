#include "pch.h"
#include "LootComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "MovementComponent.h"
#include "TransformComponent.h"

void LootComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	if (m_pSpawnedItem)
		return;
	
	if (!obj.GetComponent<HealthComponent>()->IsDead())
		return;

	m_SpawnPos = obj.GetComponent<MovementComponent>()->GetPosition();

	const auto randNr = rand() % 101;
	DetermineItem(randNr);
}

void LootComponent::AddItem(int dropRate, std::shared_ptr<ItemComponent> item)
{
	m_Loot[dropRate] = item;
}

void LootComponent::DetermineItem(int randNr)
{
	for (auto& loot : m_Loot)
	{
		if (randNr <= loot.first)
		{
			m_pSpawnedItem = loot.second->Clone();
			break;
		}
	}
	m_pSpawnedItem->GetComponent<TransformComponent>()->SetPosition(m_SpawnPos);
}
