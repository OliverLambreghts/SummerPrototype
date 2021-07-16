#pragma once
#include <stack>
#include <vector>
#include "Component.h"

struct Room;

enum class RoomType
{
	normal,
	treasure,
	boss
};
struct Neighbor
{
	Room* neighbor = nullptr;
	bool isConnected = false;
};
struct Room
{
	Neighbor left;
	Neighbor right;
	Neighbor up;
	Neighbor down;
	bool isVisited = false;
	bool isVisitedByPlayer = false;
	bool isCurrentlyActive = false;
	bool isBeginRoom = false;
	RoomType type{};
	int neighborAmount{};
};

class MazeComponent final : public Component
{
	enum class Direction
	{
		left,
		right,
		up,
		down
	};
public:
	MazeComponent();
	virtual ~MazeComponent() override = default;
	MazeComponent(const MazeComponent& other) = delete;
	MazeComponent(MazeComponent&& other) = delete;
	void operator=(const MazeComponent& rhs) = delete;
	void operator=(const MazeComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	bool HasFinishedGenerating() const;
	Room* GetCurrentRoom() const;
	std::vector<Room> GetAllRooms() const;
	void ChangeCurrentRoom(Direction direction);
	void ChangeCurrentRoom(int direction);

	static int GetWorldSize();
private:
	void GenerateWorld();
	void AddNeighbors(std::vector<Neighbor*>& neighbors) const;
	void InverseConnectRooms(std::vector<Neighbor*>& neighbors, Room* nextNode) const;
	void EndWorldGeneration();
	
	static const int m_WorldSize = 10;

	Room m_Rooms[m_WorldSize][m_WorldSize];
	std::stack<Room*> m_Stack;
	Room* m_pCurrentRoom;

	bool m_HasFinishedGenerating;
};
