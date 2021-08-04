#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Component.h"

struct Room;

class ObstacleManagerComponent final : public Component
{
public:
	enum class Direction
	{
		left,
		right,
		up,
		down
	};
	
	ObstacleManagerComponent(std::shared_ptr<GameObject> player);
	virtual ~ObstacleManagerComponent() override = default;
	ObstacleManagerComponent(const ObstacleManagerComponent& other) = delete;
	ObstacleManagerComponent(ObstacleManagerComponent&& other) = delete;
	void operator=(const ObstacleManagerComponent& rhs) = delete;
	void operator=(const ObstacleManagerComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject& obj) override;
private:
	void UpdateCurrentRoom(GameObject& obj);
	void AddObstacles(GameObject& obj);
	void CheckNeighbors();
	void SpawnTreasureObstacles();
	void ActivateObstacles(GameObject& obj);
	
	void SpawnLeftTreasureObstacles();
	void SpawnRightTreasureObstacles();
	void SpawnUpTreasureObstacles();
	void SpawnDownTreasureObstacles();
	
	Room* m_pCurrentRoom;
	std::map<Room*, std::vector<std::shared_ptr<GameObject>>> m_pObstacles;
	std::vector<Direction> m_ObstacleDirections;
	std::shared_ptr<GameObject> m_pPlayer;
};
