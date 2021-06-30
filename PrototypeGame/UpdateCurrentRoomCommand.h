#pragma once
#include "Command.h"
#include "DoorCollisionComponent.h"
#include "GameObject.h"

class UpdateCurrentRoomCommand final : public Command
{
public:
	UpdateCurrentRoomCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> maze);
	virtual ~UpdateCurrentRoomCommand() override = default;
	UpdateCurrentRoomCommand(const UpdateCurrentRoomCommand& other) = delete;
	UpdateCurrentRoomCommand(UpdateCurrentRoomCommand&& other) = delete;
	void operator=(const UpdateCurrentRoomCommand& rhs) = delete;
	void operator=(const UpdateCurrentRoomCommand&& rhs) = delete;

	virtual void Execute() override;
private:
	std::weak_ptr<GameObject> m_pPlayer, m_pMaze;
};
