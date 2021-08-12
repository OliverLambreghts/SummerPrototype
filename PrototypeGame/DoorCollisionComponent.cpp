#include "pch.h"
#include "DoorCollisionComponent.h"
#include "GameObject.h"
#include "MazeRenderComponent.h"
#include "PlayerMovementComponent.h"
#include "Sprite.h"
#include "SpriteRenderComponent.h"

DoorCollisionComponent::DoorCollisionComponent(std::shared_ptr<UpdateCurrentRoomCommand> updateRoomCmd,
	std::shared_ptr<HandleCollisionCommand> collisionCmd)
	: m_UpdateRoomCommand{ updateRoomCmd },
	m_HandleCollisionCommand{ collisionCmd }
{
}

void DoorCollisionComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
	m_UpdateRoomCommand->Execute();

	
	HandleCollision();
}

void DoorCollisionComponent::HandleCollision() const
{
	if (!m_pCurrentRoom)
		return;

	m_HandleCollisionCommand->Execute();
}
