#pragma once
#include <iostream>
#include <memory>
#include "Command.h"
#include "GameObject.h"
#include "InventoryComponent.h"

class SwitchItemUpCommand final : public Command
{
public:
	SwitchItemUpCommand(std::shared_ptr<GameObject> player)
		: m_pPlayer{ std::move(player) }
	{

	}

	virtual ~SwitchItemUpCommand() override = default;
	SwitchItemUpCommand(const SwitchItemUpCommand& other) = delete;
	SwitchItemUpCommand(SwitchItemUpCommand&& other) = delete;
	void operator=(const SwitchItemUpCommand& rhs) = delete;
	void operator=(const SwitchItemUpCommand&& rhs) = delete;

	virtual void Execute() override
	{
		m_pPlayer->GetComponent<InventoryComponent>()->SwitchItem(
			static_cast<int>(m_pPlayer->GetComponent<InventoryComponent>()->GetCurrentIdx()) + 1);
	}
private:
	std::shared_ptr<GameObject> m_pPlayer;
};
