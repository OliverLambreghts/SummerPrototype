#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Component.h"
#include "ItemComponent.h"

class LootComponent final : public Component
{
public:
	LootComponent() = default;
	virtual ~LootComponent() override = default;
	LootComponent(const LootComponent& other) = delete;
	LootComponent(LootComponent&& other) = delete;
	void operator=(const LootComponent& rhs) = delete;
	void operator=(const LootComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	void AddItem(int dropRate, std::shared_ptr<ItemComponent> item);
private:
	void DetermineItem(int randNr);
	
	std::map<int, std::shared_ptr<ItemComponent>> m_Loot;
	Point2f m_SpawnPos;
	std::shared_ptr<GameObject> m_pSpawnedItem = nullptr;
};
