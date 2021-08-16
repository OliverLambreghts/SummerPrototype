#include "pch.h"
#include "PickUpItemCommand.h"
#include <iostream>


#include "ActivityComponent.h"
#include "CoinComponent.h"
#include "InventoryComponent.h"
#include "ItemManagerComponent.h"

PickUpItemCommand::PickUpItemCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> world)
	: m_pPlayer{ player },
	m_pWorld{ world }
{
}

void PickUpItemCommand::Execute()
{
	// Handle items in vendor rooms differently
	if (m_pWorld.lock()->GetComponent<MazeComponent>()->GetCurrentRoom()->type == RoomType::vendor)
	{
		if (!m_pWorld.lock()->GetComponent<ItemManagerComponent>()->GetClosestItemInCurrentRoom() ||
			!m_pPlayer.lock()->GetComponent<InventoryComponent>()->CanPickUpItem())
		{
			std::cout << "Can't pick up item\n";
			return;
		}

		auto item = m_pWorld.lock()->GetComponent<ItemManagerComponent>()->GetClosestItemInCurrentRoom();
		std::cout << "Item costs " << item->GetComponent<ItemComponent>()->GetPrice() << " coins\n";
		
		// If player can't afford an item
		if (m_pPlayer.lock()->GetComponent<InventoryComponent>()->GetBalance() < item->GetComponent<ItemComponent>()->GetPrice())
		{
			std::cout << "Can't pick up item, can't afford\n";
			return;
		}

		m_pWorld.lock()->GetComponent<ItemManagerComponent>()->RemoveItem(item);
		m_pPlayer.lock()->GetComponent<InventoryComponent>()->AddItem(item);
		m_pPlayer.lock()->GetComponent<InventoryComponent>()->AddCoinAmount(-item->GetComponent<ItemComponent>()->GetPrice());
		item->GetComponent<ActivityComponent>()->Deactivate();
		return;
	}

	if (!m_pWorld.lock()->GetComponent<ItemManagerComponent>()->GetClosestItemInCurrentRoom() ||
		!m_pPlayer.lock()->GetComponent<InventoryComponent>()->CanPickUpItem())
	{
		std::cout << "Can't pick up item\n";
		return;
	}

	std::cout << "Can pick up item\n";

	auto item = m_pWorld.lock()->GetComponent<ItemManagerComponent>()->GetClosestItemInCurrentRoom();
	m_pWorld.lock()->GetComponent<ItemManagerComponent>()->RemoveItem(item);

	if (!item->GetComponent<CoinComponent>())
		m_pPlayer.lock()->GetComponent<InventoryComponent>()->AddItem(
			item);
	else
		m_pPlayer.lock()->GetComponent<InventoryComponent>()->AddCoin();

	item->GetComponent<ActivityComponent>()->Deactivate();
}
