#pragma once
#include <memory>
#include "Command.h"
#include "GameObject.h"
#include "InventoryComponent.h"

class SwitchItemDownCommand final : public Command
{
public:
	SwitchItemDownCommand(std::shared_ptr<GameObject> player)
		: m_pPlayer{ std::move(player) }
	{

	}
	
	virtual ~SwitchItemDownCommand() override = default;
	SwitchItemDownCommand(const SwitchItemDownCommand& other) = delete;
	SwitchItemDownCommand(SwitchItemDownCommand&& other) = delete;
	void operator=(const SwitchItemDownCommand& rhs) = delete;
	void operator=(const SwitchItemDownCommand&& rhs) = delete;

	virtual void Execute() override
	{
		m_pPlayer->GetComponent<InventoryComponent>()->SwitchItem(
			static_cast<int>(m_pPlayer->GetComponent<InventoryComponent>()->GetCurrentIdx()) - 1);
	}
private:
	std::shared_ptr<GameObject> m_pPlayer;
};
