#pragma once
#include "Command.h"
#include "GameObject.h"
#include "InventoryComponent.h"

class UseItemCommand final : public Command
{
public:
	UseItemCommand(std::shared_ptr<GameObject> player)
		: m_pPlayer{ std::move(player) }
	{

	}

	virtual ~UseItemCommand() override = default;
	UseItemCommand(const UseItemCommand& other) = delete;
	UseItemCommand(UseItemCommand&& other) = delete;
	void operator=(const UseItemCommand& rhs) = delete;
	void operator=(const UseItemCommand&& rhs) = delete;

	virtual void Execute() override
	{
		m_pPlayer->GetComponent<InventoryComponent>()->OnUse(m_pPlayer);
	}
private:
	std::shared_ptr<GameObject> m_pPlayer;
};
