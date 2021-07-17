#pragma once
#include "Command.h"
#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include "InventoryComponent.h"

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
		switch (m_pPlayer->GetComponent<InventoryComponent>()->GetCurrentItemType())
		{
		case InventoryComponent::ItemType::MeleeKey:
			auto enemy = m_pWorld->GetComponent<EnemyManagerComponent>()->GetClosestEnemy();
			const auto playerPos = m_pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition();
			const auto enemyPos = enemy->GetComponent<EnemyMovementComponent>()->GetPosition();
			const Vector2f distanceVec{ playerPos, enemyPos };
			if (distanceVec.Length() <= m_AttackRange)
				m_pPlayer->GetComponent<InventoryComponent>()->OnUse(m_pPlayer, enemy);
			break;
		}
	}
private:
	std::shared_ptr<GameObject> m_pPlayer, m_pWorld;
	const float m_AttackRange = 50.f;
};
