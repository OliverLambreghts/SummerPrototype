#pragma once
#include "Command.h"
#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include "InventoryComponent.h"
#include "Sprite.h"

class UseItemCommand final : public Command
{
public:
	UseItemCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> world)
		: m_pPlayer{ std::move(player) },
		m_pWorld{ std::move(world) }
	{

	}

	virtual ~UseItemCommand() override = default;
	UseItemCommand(const UseItemCommand& other) = delete;
	UseItemCommand(UseItemCommand&& other) = delete;
	void operator=(const UseItemCommand& rhs) = delete;
	void operator=(const UseItemCommand&& rhs) = delete;

	virtual void Execute() override
	{
		const auto type = m_pPlayer->GetComponent<InventoryComponent>()->GetCurrentItemType();
		
		if (type == InventoryComponent::ItemType::MeleeKey)
		{
			auto enemy = m_pWorld->GetComponent<EnemyManagerComponent>()->GetClosestEnemyInFront();

			if (!enemy)
				return;

			const auto playerPos = m_pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition();
			const auto enemyPos = enemy->GetComponent<EnemyMovementComponent>()->GetPosition();
			const Vector2f distanceVec{ playerPos, enemyPos };
			const auto attackRange = m_pPlayer->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() + m_RangeOffset;

			if (distanceVec.Length() <= attackRange)
				m_pPlayer->GetComponent<InventoryComponent>()->OnUse(m_pPlayer, enemy);
		}
		else if(type == InventoryComponent::ItemType::RangedKey)
		{
			m_pPlayer->GetComponent<InventoryComponent>()->OnUse(m_pPlayer, m_pWorld);
		}
		else if(type == InventoryComponent::ItemType::Consumable)
		{
			m_pPlayer->GetComponent<InventoryComponent>()->OnUse(m_pPlayer, nullptr);
			// Remove consumable after using
			m_pPlayer->GetComponent<InventoryComponent>()->RemoveCurrentItem();
		}
	}
private:
	std::shared_ptr<GameObject> m_pPlayer, m_pWorld;
	const float m_RangeOffset = 20.f;
};
