#include "pch.h"
#include "HandleCollisionCommand.h"
#include "DoorCollisionComponent.h"
#include "MazeRenderComponent.h"
#include "Sprite.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"

HandleCollisionCommand::HandleCollisionCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> maze)
	: m_pPlayer{ std::move(player) },
	m_pMaze{ std::move(maze) }
{
}

void HandleCollisionCommand::Execute()
{
	const auto playerSprite = Rectf{ m_pPlayer.lock()->GetComponent<PlayerMovementComponent>()->GetPosition().x,
	m_pPlayer.lock()->GetComponent<PlayerMovementComponent>()->GetPosition().y,
		m_pPlayer.lock()->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	m_pPlayer.lock()->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

	auto doors = m_pMaze.lock()->GetComponent<MazeRenderComponent>()->GetActiveDoors();
	for (const auto& door : doors)
	{
		if (utils::IsOverlapping(playerSprite, door.second))
		{
			m_pMaze.lock()->GetComponent<MazeComponent>()->ChangeCurrentRoom(door.first);
			m_pPlayer.lock()->GetComponent<DoorCollisionComponent>()->m_pCurrentRoom = m_pMaze.lock()->GetComponent<MazeComponent>()->GetCurrentRoom();

			Point2f newPos{};
			switch (door.first)
			{
				//left
			case 0:
				newPos = Point2f{ m_pMaze.lock()->GetComponent<MazeRenderComponent>()->GetWindowDimension()
					- m_pPlayer.lock()->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() - door.second.width ,
					door.second.bottom + (doors[door.first].height / 2) };
				break;
				//right
			case 1:
				newPos = Point2f{ m_pPlayer.lock()->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() + door.second.width,
					door.second.bottom + (door.second.height / 2) };
				break;
				//up
			case 2:
				newPos = Point2f{ door.second.left + (door.second.width / 2),
					m_pPlayer.lock()->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() + door.second.height };
				break;
				//down
			case 3:
				newPos = Point2f{ door.second.left + (door.second.width / 2),
					m_pMaze.lock()->GetComponent<MazeRenderComponent>()->GetWindowDimension()
					- m_pPlayer.lock()->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight()
				- door.second.height };
				break;
			}

			m_pPlayer.lock()->GetComponent<PlayerMovementComponent>()->SetPosition(newPos);
		}
	}
}
