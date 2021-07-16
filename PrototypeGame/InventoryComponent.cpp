#include "pch.h"
#include "InventoryComponent.h"
#include <cassert>
#include <iostream>
#include "ActivityComponent.h"
#include "GameObject.h"
#include "ItemComponent.h"

void InventoryComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

void InventoryComponent::AddItem(std::shared_ptr<GameObject> item)
{
	if (m_Items.size() == m_MaxCapacity)
		return;

	item->GetComponent<ActivityComponent>()->Deactivate();
	m_Items.push_back(item);
}

void InventoryComponent::SwitchItem(int idx)
{
	/*if (idx > static_cast<int>(GetCurrentIdx()))
		std::cout << "Scrolled up\n";
	else if (idx < static_cast<int>(GetCurrentIdx()))
		std::cout << "Scrolled down\n";*/

	if (idx >= m_MaxCapacity)
		return;
	
	assert(idx < m_MaxCapacity);
	
	if (idx >= m_Items.size())
		return;

	m_pActiveItem = m_Items[idx];
	m_pActiveItem.lock()->GetComponent<ItemComponent>()->PrintStats();
}

void InventoryComponent::OnUse(std::shared_ptr<GameObject> player) const
{
	if (!m_pActiveItem.lock())
		return;

	m_pActiveItem.lock()->GetComponent<ItemComponent>()->OnUse(player);
}

size_t InventoryComponent::GetCurrentIdx() const
{
	for (size_t i{}; i < m_Items.size(); ++i)
	{
		if(m_pActiveItem.lock() == m_Items[i].lock())
		{
			return i;
		}
	}
	return 0;
}
