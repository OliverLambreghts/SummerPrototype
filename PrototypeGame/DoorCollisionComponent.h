#pragma once
#include <memory>

#include "Component.h"
#include "MazeComponent.h"
#include "UpdateCurrentRoomCommand.h"
#include "HandleCollisionCommand.h"

class DoorCollisionComponent final : public Component
{
	friend class UpdateCurrentRoomCommand;
	friend class HandleCollisionCommand;
public:
	DoorCollisionComponent(std::shared_ptr<UpdateCurrentRoomCommand> updateRoomCmd,
		std::shared_ptr<HandleCollisionCommand> collisionCmd);
	virtual ~DoorCollisionComponent() override = default;
	DoorCollisionComponent(const DoorCollisionComponent& other) = delete;
	DoorCollisionComponent(DoorCollisionComponent&& other) = delete;
	void operator=(const DoorCollisionComponent& rhs) = delete;
	void operator=(const DoorCollisionComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;
private:
	void HandleCollision() const;
	
	Room* m_pCurrentRoom = nullptr;
	std::shared_ptr<UpdateCurrentRoomCommand> m_UpdateRoomCommand;
	std::shared_ptr<HandleCollisionCommand> m_HandleCollisionCommand;
};
