#include "pch.h"
#include "EnemyManagerComponent.h"
#include <iostream>

#include "ActivityComponent.h"
#include "DebugEnemyComponent.h"
#include "Game.h"
#include "Scene.h"
#include "SceneManager.h"

EnemyManagerComponent::EnemyManagerComponent()
	: m_pCurrentRoom{ nullptr }
{
}

void EnemyManagerComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	UpdateCurrentRoom(obj);
	AddSpawners(obj);
	SpawnEnemies(obj);
}

void EnemyManagerComponent::UpdateCurrentRoom(GameObject& obj)
{
	if (m_pCurrentRoom == obj.GetComponent<MazeComponent>()->GetCurrentRoom())
		return;

	// Despawn enemies when going to another room
	for(auto& enemy : m_Enemies[m_pCurrentRoom])
	{
		enemy->GetComponent<ActivityComponent>()->Deactivate();
	}
	
	m_pCurrentRoom = obj.GetComponent<MazeComponent>()->GetCurrentRoom();
}

void EnemyManagerComponent::AddSpawners(GameObject& obj)
{
	if (obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
	{
		if (m_pCurrentRoom->isBeginRoom && m_Spawners.find(m_pCurrentRoom) != m_Spawners.end())
			m_Spawners.erase(m_pCurrentRoom);
		return;
	}

	// --- DEBUG CODE ---
	std::vector<Spawner> spawners;

	const int amount = rand() % 4 + 1;

	for (int i{}; i < amount; ++i)
	{
		auto pos = Point2f{ static_cast<float>(rand() % (static_cast<int>(Game::GetWindowDimension()) - 100)),
		static_cast<float>(rand() % (static_cast<int>(Game::GetWindowDimension()) - 100)) };
		auto prototype = std::make_shared<DebugEnemyComponent>(rand() % 100, rand() % 50, rand() % 50, pos);

		Spawner spawner{ prototype };
		spawners.push_back(spawner);
	}

	m_Spawners[m_pCurrentRoom] = spawners;

	// --- DEBUG CODE ---
}

void EnemyManagerComponent::SpawnEnemies(GameObject& obj)
{
	if (!obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
		return;

	if (m_Spawners.find(m_pCurrentRoom) == m_Spawners.end())
	{
		if (m_Enemies.find(m_pCurrentRoom) == m_Enemies.end())
			return;

		for(auto& enemy : m_Enemies[m_pCurrentRoom])
		{
			enemy->GetComponent<ActivityComponent>()->Activate();
		}
		
		return;
	}
	
	for(auto& spawner : m_Spawners[m_pCurrentRoom])
	{
		auto enemy = spawner.Spawn();
		enemy->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(enemy);
		m_Enemies[m_pCurrentRoom].push_back(enemy);
	}

	m_Spawners.erase(m_pCurrentRoom);
}