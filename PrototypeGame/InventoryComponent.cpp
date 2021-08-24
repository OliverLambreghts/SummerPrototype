#include "pch.h"
#include "InventoryComponent.h"
#include <cassert>
#include <iostream>
#include "ActivityComponent.h"
#include "GameObject.h"
#include "ItemComponent.h"
#include "MeleeKeyComponent.h"
#include "ProjectileComponent.h"
#include "RangedKeyComponent.h"
#include "StatManager.h"

InventoryComponent::InventoryComponent(std::shared_ptr<GameObject> world)
	: m_pWorld{ std::move(world) },
	m_pCurrentRoom{ nullptr },
	m_EffectTimer{},
	m_pActiveItem{},
	m_Coins{}
{
}

void InventoryComponent::Update(float elapsedSec, GameObject& /*obj*/)
{
	for (auto& item : m_Items)
	{
		item.lock()->GetComponent<ItemComponent>()->Update(elapsedSec, *item.lock());
	}

	// Reset an active projectile when the player changes rooms
	ResetActiveProjectile();

	UpdateEffectTimer(elapsedSec);
}

void InventoryComponent::UpdateEffectTimer(float elapsedSec)
{
	if (m_pEffect)
	{
		m_EffectTimer += elapsedSec;

		if (m_EffectTimer >= m_pEffect->GetDuration())
		{
			m_EffectTimer = 0.f;
			m_pEffect = nullptr;
		}
	}
}

void InventoryComponent::ResetActiveProjectile()
{
	if (m_pCurrentRoom != m_pWorld.lock()->GetComponent<MazeComponent>()->GetCurrentRoom())
	{
		m_pCurrentRoom = m_pWorld.lock()->GetComponent<MazeComponent>()->GetCurrentRoom();
		if (m_pActiveItem.lock() && m_pActiveItem.lock()->GetComponent<RangedKeyComponent>())
		{
			m_pActiveItem.lock()->GetComponent<ProjectileComponent>()->ResetProjectile();
		}
	}
}

void InventoryComponent::AddItem(std::shared_ptr<GameObject> item)
{
	if (m_Items.size() == m_MaxCapacity)
		return;

	//item->GetComponent<ActivityComponent>()->Deactivate();
	m_Items.push_back(item);
	m_pActiveItem = item;
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

void InventoryComponent::OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy) const
{
	if (!m_pActiveItem.lock())
		return;

	m_pActiveItem.lock()->GetComponent<ItemComponent>()->OnUse(player, enemy);
}

size_t InventoryComponent::GetCurrentIdx() const
{
	for (size_t i{}; i < m_Items.size(); ++i)
	{
		if (m_pActiveItem.lock() == m_Items[i].lock())
		{
			return i;
		}
	}
	return 0;
}

bool InventoryComponent::CanPickUpItem() const
{
	return m_Items.size() != m_MaxCapacity;
}

InventoryComponent::ItemType InventoryComponent::GetCurrentItemType() const
{
	if (!m_pActiveItem.lock())
		return ItemType::none;

	if (m_pActiveItem.lock()->GetComponent<MeleeKeyComponent>())
	{
		return ItemType::MeleeKey;
	}
	if (m_pActiveItem.lock()->GetComponent<RangedKeyComponent>())
	{
		return ItemType::RangedKey;
	}
	return ItemType::Consumable;
}

void InventoryComponent::RemoveCurrentItem()
{
	m_Items.erase(std::remove_if(m_Items.begin(), m_Items.end(), [this](std::weak_ptr<GameObject> item)
		{
			return m_pActiveItem.lock() == item.lock();
		}), m_Items.end());

	m_pActiveItem.reset();

	if (!m_Items.empty())
	{
		m_pActiveItem = m_Items.front();
		m_pActiveItem.lock()->GetComponent<ItemComponent>()->PrintStats();
	}
}

void InventoryComponent::SetEffect(BaseEffect* pEffect)
{
	m_pEffect = pEffect;
}

BaseEffect* InventoryComponent::GetActiveEffect() const
{
	return m_pEffect;
}

void InventoryComponent::AddCoin()
{
	++m_Coins;
	std::cout << "You now have " << m_Coins << " coins\n";
	++StatManager::GetInstance().GetCurrentStats().coinsCollected;
}

int InventoryComponent::GetBalance() const
{
	return m_Coins;
}

void InventoryComponent::AddCoinAmount(int amount)
{
	m_Coins += amount;
}
