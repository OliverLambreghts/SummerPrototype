#pragma once
#include <map>
#include <memory>
#include "BaseEffect.h"
#include "BaseProc.h"
#include "Component.h"
#include "MazeComponent.h"
#include "WeaponQuality.h"

class ItemManagerComponent final : public Component
{
public:
	enum class ItemType
	{
		MeleeKey,
		RangedKey,
		Consumable
	};
	
	ItemManagerComponent(std::shared_ptr<GameObject> player);
	virtual ~ItemManagerComponent() override = default;
	ItemManagerComponent(const ItemManagerComponent& other) = delete;
	ItemManagerComponent(ItemManagerComponent&& other) = delete;
	void operator=(const ItemManagerComponent& rhs) = delete;
	void operator=(const ItemManagerComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	bool IsPlayerCloseToItem();
	std::shared_ptr<GameObject> GetItemInCurrentRoom();
	void RemoveItem();
private:
	void UpdateCurrentRoom(GameObject& obj);
	void AddItems(GameObject& obj);
	void ParseMeleeData(const std::string& line, std::vector<std::string>& data) const;
	void ParseRangedData(const std::string& line, std::vector<std::string>& data) const;
	void ParseConsumableData(const std::string& line, std::vector<std::string>& data) const;
	void SpawnMeleeKey();
	void SpawnRangedKey();
	void SpawnConsumable();
	void SetProc(std::vector<std::string>& data, std::shared_ptr<BaseProc>& proc) const;
	void SetEffect(std::vector<std::string>& data, std::vector<std::shared_ptr<BaseEffect>>& effects) const;
	void SpawnItem(GameObject& obj);
	
	Room* m_pCurrentRoom;
	std::map<Room*, std::shared_ptr<GameObject>> m_Items;
	std::shared_ptr<GameObject> m_pPlayer;
	std::vector<std::shared_ptr<WeaponQuality>> m_Qualities;
};
