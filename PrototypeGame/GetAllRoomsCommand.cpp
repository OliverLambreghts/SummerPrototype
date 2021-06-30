#include "pch.h"
#include "GetAllRoomsCommand.h"
#include "MapRenderComponent.h"

GetAllRoomsCommand::GetAllRoomsCommand(std::shared_ptr<GameObject> map, std::shared_ptr<GameObject> world)
	: m_pMap{ map },
	m_pWorld{ world }
{

}

void GetAllRoomsCommand::Execute()
{
	if (!m_pMap.lock()->GetComponent<MapRenderComponent>()->m_pGetRooms)
	{
		m_pMap.lock()->GetComponent<MapRenderComponent>()->m_pGetRooms =
			std::bind(&MazeComponent::GetAllRooms, m_pWorld.lock()->GetComponent<MazeComponent>());
	}
}
