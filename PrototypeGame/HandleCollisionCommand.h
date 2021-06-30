#pragma once
#include <memory>
#include "Command.h"
#include "GameObject.h"

class HandleCollisionCommand final : public Command
{
public:
	HandleCollisionCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> maze);
	virtual ~HandleCollisionCommand() override = default;
	HandleCollisionCommand(const HandleCollisionCommand& other) = delete;
	HandleCollisionCommand(HandleCollisionCommand&& other) = delete;
	void operator=(const HandleCollisionCommand& rhs) = delete;
	void operator=(const HandleCollisionCommand&& rhs) = delete;
	
	virtual void Execute() override;
private:
	std::weak_ptr<GameObject> m_pPlayer, m_pMaze;
};
