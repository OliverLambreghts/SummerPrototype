#pragma once
#include <memory>
#include <vector>

#include "BaseEffect.h"
#include "Component.h"
#include "MazeComponent.h"

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
	
	InventoryComponent(std::shared_ptr<GameObject> world);
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
	void RemoveCurrentItem();
	void SetEffect(BaseEffect* pEffect);
	BaseEffect* GetActiveEffect() const;
private:
	void ResetActiveProjectile();
	void UpdateEffectTimer(float elapsedSec);
	
	static const int m_MaxCapacity = 10;

	std::vector<std::weak_ptr<GameObject>> m_Items;
	std::weak_ptr<GameObject> m_pActiveItem;
	Room* m_pCurrentRoom;
	std::weak_ptr<GameObject> m_pWorld;
	BaseEffect* m_pEffect = nullptr;
	float m_EffectTimer;
};
