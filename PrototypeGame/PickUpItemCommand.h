#pragma once
#include "Command.h"
#include "GameObject.h"

class PickUpItemCommand final : public Command
{
public:
	PickUpItemCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> world);
	virtual ~PickUpItemCommand() override = default;
	PickUpItemCommand(const PickUpItemCommand& other) = delete;
	PickUpItemCommand(PickUpItemCommand&& other) = delete;
	void operator=(const PickUpItemCommand& rhs) = delete;
	void operator=(const PickUpItemCommand&& rhs) = delete;

	virtual void Execute() override;
private:
	std::weak_ptr<GameObject> m_pPlayer, m_pWorld;
};
