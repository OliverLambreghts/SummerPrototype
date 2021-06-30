#include "pch.h"
#include "UpdateCurrentRoomCommand.h"

UpdateCurrentRoomCommand::UpdateCurrentRoomCommand(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> maze)
	: m_pPlayer{ std::move(player) },
	m_pMaze{ std::move(maze) }
{

}

void UpdateCurrentRoomCommand::Execute()
{

	if (m_pPlayer.lock()->GetComponent<DoorCollisionComponent>()->m_pCurrentRoom == m_pMaze.lock()->GetComponent<MazeComponent>()->GetCurrentRoom())
		return;

	if (m_pMaze.lock()->GetComponent<MazeComponent>()->HasFinishedGenerating() &&
		m_pMaze.lock()->GetComponent<MazeComponent>()->GetCurrentRoom())
	{
		m_pPlayer.lock()->GetComponent<DoorCollisionComponent>()->m_pCurrentRoom = m_pMaze.lock()->GetComponent<MazeComponent>()->GetCurrentRoom();
	}

}
