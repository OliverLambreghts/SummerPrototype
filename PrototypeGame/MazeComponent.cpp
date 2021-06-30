#include "pch.h"
#include "MazeComponent.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

MazeComponent::MazeComponent()
	: m_HasFinishedGenerating{ false }
{
	int idx{};
	for (int row{}; row < m_WorldSize; ++row)
	{
		for (int col{}; col < m_WorldSize; ++col)
		{
			Room room{};
			room.isVisited = false;

			if (col + 1 < m_WorldSize)
				room.right = Neighbor{ &m_Rooms[row][col + 1], false };
			if (col - 1 >= 0)
				room.left = Neighbor{ &m_Rooms[row][col - 1], false };
			if (row + 1 < m_WorldSize)
				room.up = Neighbor{ &m_Rooms[row + 1][col], false };
			if (row - 1 >= 0)
				room.down = Neighbor{ &m_Rooms[row - 1][col], false };

			m_Rooms[row][col] = room;

			++idx;
		}
	}

	// Adjust first cell separately to specify that it's already been visited
	m_Rooms[0][0].isVisited = true;
	m_pCurrentRoom = &m_Rooms[0][0];

	// Generate the world
	while (!m_HasFinishedGenerating)
		GenerateWorld();
}

void MazeComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

void MazeComponent::GenerateWorld()
{
	if (m_HasFinishedGenerating)
		return;

	std::vector<Neighbor*> neighbors;

	m_pCurrentRoom->isVisited = true;
	const int percentage = (rand() % 101);
	if (percentage >= 30)
		m_pCurrentRoom->type = RoomType::normal;
	else if (percentage >= 10)
		m_pCurrentRoom->type = RoomType::treasure;
	else
		m_pCurrentRoom->type = RoomType::boss;

	AddNeighbors(neighbors);

	if (neighbors.empty())
	{
		m_pCurrentRoom = m_Stack.top();
		m_Stack.pop();

		EndWorldGeneration();

		return;
	}

	std::random_shuffle(neighbors.begin(), neighbors.end());
	Room* nextNode = neighbors[0]->neighbor;
	++m_pCurrentRoom->neighborAmount;

	InverseConnectRooms(neighbors, nextNode);

	m_Stack.push(m_pCurrentRoom);
	m_pCurrentRoom = nextNode;
}

void MazeComponent::EndWorldGeneration()
{
	if (m_Stack.empty())
	{
		m_HasFinishedGenerating = true;
		const int beginIdx = rand() % m_WorldSize;
		m_pCurrentRoom = &m_Rooms[beginIdx][beginIdx];
		m_pCurrentRoom->isVisitedByPlayer = true;
		m_pCurrentRoom->isCurrentlyActive = true;

		// Rooms with only 1 connection are treasure rooms
		for (auto& row : m_Rooms)
		{
			for (auto& room : row)
			{
				if (room.neighborAmount == 1)
					room.type = RoomType::treasure;
			}
		}

		// First room is a normal room
		m_pCurrentRoom->type = RoomType::normal;

		std::cout << "World generation finished\n";
	}
}

void MazeComponent::AddNeighbors(std::vector<Neighbor*>& neighbors) const
{
	if (m_pCurrentRoom->down.neighbor && !m_pCurrentRoom->down.neighbor->isVisited)
		neighbors.push_back(&m_pCurrentRoom->down);
	if (m_pCurrentRoom->up.neighbor && !m_pCurrentRoom->up.neighbor->isVisited)
		neighbors.push_back(&m_pCurrentRoom->up);
	if (m_pCurrentRoom->left.neighbor && !m_pCurrentRoom->left.neighbor->isVisited)
		neighbors.push_back(&m_pCurrentRoom->left);
	if (m_pCurrentRoom->right.neighbor && !m_pCurrentRoom->right.neighbor->isVisited)
		neighbors.push_back(&m_pCurrentRoom->right);
}

void MazeComponent::InverseConnectRooms(std::vector<Neighbor*>& neighbors, Room* nextNode) const
{
	neighbors[0]->isConnected = true;
	if (nextNode->down.neighbor == m_pCurrentRoom)
		nextNode->down.isConnected = true;
	else if (nextNode->up.neighbor == m_pCurrentRoom)
		nextNode->up.isConnected = true;
	else if (nextNode->left.neighbor == m_pCurrentRoom)
		nextNode->left.isConnected = true;
	else if (nextNode->right.neighbor == m_pCurrentRoom)
		nextNode->right.isConnected = true;
	++nextNode->neighborAmount;
}

bool MazeComponent::HasFinishedGenerating() const
{
	return m_HasFinishedGenerating;
}

Room* MazeComponent::GetCurrentRoom() const
{
	return m_pCurrentRoom;
}

std::vector<Room> MazeComponent::GetAllRooms() const
{
	std::vector<Room> rooms;

	for(const auto& row : m_Rooms)
	{
		for(const auto& room : row)
		{
			rooms.push_back(room);
		}
	}
	
	return rooms;
}

void MazeComponent::ChangeCurrentRoom(Direction direction)
{
	m_pCurrentRoom->isCurrentlyActive = false;
	
	switch (direction)
	{
	case Direction::left:
		m_pCurrentRoom = m_pCurrentRoom->left.neighbor;
		break;
	case Direction::right:
		m_pCurrentRoom = m_pCurrentRoom->right.neighbor;
		break;
	case Direction::up:
		m_pCurrentRoom = m_pCurrentRoom->up.neighbor;
		break;
	case Direction::down:
		m_pCurrentRoom = m_pCurrentRoom->down.neighbor;
		break;
	}

	assert(m_pCurrentRoom);

	m_pCurrentRoom->isVisitedByPlayer = true;
	m_pCurrentRoom->isCurrentlyActive = true;
}

void MazeComponent::ChangeCurrentRoom(int direction)
{
	ChangeCurrentRoom(static_cast<Direction>(direction));

	std::cout << "Entered ";
	switch (m_pCurrentRoom->type)
	{
	case RoomType::boss:
		std::cout << "boss ";
		break;
	case RoomType::normal:
		std::cout << "normal ";
		break;
	case RoomType::treasure:
		std::cout << "treasure ";
		break;
	}
	std::cout << "room\n";
}

int MazeComponent::GetWorldSize()
{
	return m_WorldSize;
}
