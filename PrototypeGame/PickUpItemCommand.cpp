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
