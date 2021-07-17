#pragma once
#include <memory>
#include <vector>
#include "Component.h"

class InventoryComponent final : public Component
{
public:
	enum class ItemType
	{
		MeleeKey,
		RangedKey,
		Consumable,
		none
	};
	
	InventoryComponent() = default;
	virtual ~InventoryComponent() override = default;
	InventoryComponent(const InventoryComponent& other) = delete;
	InventoryComponent(InventoryComponent&& other) = delete;
	void operator=(const InventoryComponent& rhs) = delete;
	void operator=(const InventoryComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	void AddItem(std::shared_ptr<GameObject> item);
	void SwitchItem(int idx);
	void OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy) const;
	size_t GetCurrentIdx() const;
	bool CanPickUpItem() const;
	ItemType GetCurrentItemType() const;
private:
	static const int m_MaxCapacity = 10;

	std::vector<std::weak_ptr<GameObject>> m_Items;
	std::weak_ptr<GameObject> m_pActiveItem;
};
