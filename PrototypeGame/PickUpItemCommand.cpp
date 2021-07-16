#include "pch.h"
#include "PickUpItemCommand.h"

#include <iostream>



#include "InventoryComponent.h"
#include "ItemManagerComponent.h"

PickUpItemCommand::PickUpItemCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> world)
	: m_pPlayer{ player },
	m_pWorld{ world }
{
}

void PickUpItemCommand::Execute()
{
	if (!m_pWorld.lock()->GetComponent<ItemManagerComponent>()->IsPlayerCloseToItem())
	{
		std::cout << "Can't pick up item\n";
		return;
	}

	std::cout << "Can pick up item\n";

	auto item = m_pWorld.lock()->GetComponent<ItemManagerComponent>()->GetItemInCurrentRoom();
	m_pWorld.lock()->GetComponent<ItemManagerComponent>()->RemoveItem();
	m_pPlayer.lock()->GetComponent<InventoryComponent>()->AddItem(
		item);
}
