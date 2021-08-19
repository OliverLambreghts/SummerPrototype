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

	std::shared_ptr<GameObject> GetClosestObstacleInFront();
private:
	void UpdateCurrentRoom(GameObject& obj);
	void AddObstacles(GameObject& obj);
	void CheckNeighbors();
	void SpawnTreasureObstacles(GameObject& obj);
	void ActivateObstacles(GameObject& obj);
	void RemoveBrokenObstacles();
	
	void SpawnLeftTreasureObstacles(GameObject& obj);
	void SpawnRightTreasureObstacles(GameObject& obj);
	void SpawnUpTreasureObstacles(GameObject& obj);
	void SpawnDownTreasureObstacles(GameObject& obj);

	void SpawnRandomObstacle(GameObject& obj);
	
	Room* m_pCurrentRoom;
	std::map<Room*, std::vector<std::shared_ptr<GameObject>>> m_pObstacles;
	std::vector<Direction> m_ObstacleDirections;
	std::shared_ptr<GameObject> m_pPlayer;

	bool m_HasSpawnedRandomObstacle = false;
};
