#include "pch.h"
#include "LootComponent.h"
#include "ActivityComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ItemManagerComponent.h"
#include "MovementComponent.h"
#include "SceneManager.h"
#include "TransformComponent.h"
#include "Scene.h"

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

bool LootComponent::HasSpawnedItem() const
{
	return m_HasSpawnedItem;
}

void LootComponent::SetFlag()
{
	m_HasSpawnedItem = true;
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

	if (m_pSpawnedItem)
	{
		m_pSpawnedItem->GetComponent<TransformComponent>()->SetPosition(m_SpawnPos);
		m_pSpawnedItem->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(m_pSpawnedItem);
		ItemManagerComponent::AddNewItem(m_pSpawnedItem);
	}
		
	m_HasSpawnedItem = true;
}
