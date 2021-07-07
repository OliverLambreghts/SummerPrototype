#pragma once
#include <map>

#include "Component.h"
#include "MazeComponent.h"
#include "Spawner.h"

class EnemyManagerComponent final : public Component
{
public:
	EnemyManagerComponent();
	virtual ~EnemyManagerComponent() override = default;
	EnemyManagerComponent(const EnemyManagerComponent& other) = delete;
	EnemyManagerComponent(EnemyManagerComponent&& other) = delete;
	void operator=(const EnemyManagerComponent& rhs) = delete;
	void operator=(const EnemyManagerComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject& obj) override;
private:
	void UpdateCurrentRoom(GameObject& obj);
	void AddSpawners(GameObject& obj);
	void SpawnEnemies(GameObject& obj);
	
	std::map<Room*, std::vector<Spawner>> m_Spawners;
	std::map<Room*, std::vector<std::shared_ptr<GameObject>>> m_Enemies;
	Room* m_pCurrentRoom;
};
