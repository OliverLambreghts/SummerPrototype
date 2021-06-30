#include "pch.h"
#include "MapRenderComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "utils.h"

MapRenderComponent::MapRenderComponent(std::shared_ptr<GetAllRoomsCommand> roomsCmd)
	: m_pGetRoomsCmd{ roomsCmd }
{

}

void MapRenderComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	if (!m_pGetCells)
		m_pGetCells = std::bind(&GridComponent::GetCells, obj.GetComponent<GridComponent>());

	m_pGetRoomsCmd->Execute();
}

void MapRenderComponent::Render() const
{
	if (!m_pGetCells || !m_IsMapActive)
		return;

	const auto cells = m_pGetCells();
	const auto rooms = m_pGetRooms();

	for (size_t i{}; i < cells.size(); ++i)
	{
		if (rooms[i].isVisitedByPlayer)
			utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 100.f / 255.f });
		else
			continue;

		if (rooms[i].isCurrentlyActive)
			utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 100.f / 255.f });

		Rectf room = Rectf{ cells[i].left + 2.f, cells[i].bottom + 2.f, cells[i].width - 4.f,
		cells[i].height - 4.f };
		
		utils::FillRect(room);

		// Render the connections to other rooms
		RenderHalls(rooms, i, room);
	}
}

void MapRenderComponent::RenderHalls(std::vector<Room> rooms, size_t idx, const Rectf& room) const
{
	// Render the connections to other rooms
	const float hallDimension{ room.width / 3.f };
	const float smallHallDim{ room.width / 10.f };

	const Rectf topHall{ room.left + (room.width / 2.f) - (hallDimension / 2.f), room.bottom + room.height, hallDimension, smallHallDim }
		, botHall{ room.left + (room.width / 2.f) - (hallDimension / 2.f) , room.bottom - smallHallDim, hallDimension, smallHallDim }
		, rightHall{ room.left + room.width, room.bottom + (room.width / 2.f) - (hallDimension / 2.f), smallHallDim, hallDimension }
	, leftHall{ room.left - smallHallDim, room.bottom + (room.width / 2.f) - (hallDimension / 2.f) , smallHallDim, hallDimension };
	
	if (rooms[idx].up.isConnected)
		utils::FillRect(topHall);
	if (rooms[idx].down.isConnected)
		utils::FillRect(botHall);
	if (rooms[idx].left.isConnected)
		utils::FillRect(leftHall);
	if (rooms[idx].right.isConnected)
		utils::FillRect(rightHall);
}

void MapRenderComponent::ActivateMap()
{
	m_IsMapActive = true;
}

void MapRenderComponent::DeactivateMap()
{
	m_IsMapActive = false;
}

bool MapRenderComponent::IsMapActive() const
{
	return m_IsMapActive;
}