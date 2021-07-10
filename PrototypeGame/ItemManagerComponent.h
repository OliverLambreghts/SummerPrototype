#pragma once
#include <map>
#include <memory>

#include "BaseProc.h"
#include "Component.h"
#include "MazeComponent.h"

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
private:
	void UpdateCurrentRoom(GameObject& obj);
	void AddItems(GameObject& obj);
	void ParseData(const std::string& line, std::vector<std::string>& data) const;
	void SpawnMeleeKey();
	void SetProc(std::vector<std::string>& data, std::shared_ptr<BaseProc> proc) const;
	void SpawnItem(GameObject& obj);
	
	Room* m_pCurrentRoom;
	std::map<Room*, std::shared_ptr<GameObject>> m_Items;
	std::shared_ptr<GameObject> m_pPlayer;
};
