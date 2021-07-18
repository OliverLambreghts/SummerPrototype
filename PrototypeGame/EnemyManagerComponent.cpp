#include "pch.h"
#include "EnemyManagerComponent.h"
#include <iostream>
#include "ActivityComponent.h"
#include "DebugEnemyComponent.h"
#include "Game.h"
#include "Scene.h"
#include "SceneManager.h"
#include <algorithm>
#include "EnemyMovementComponent.h"
#include "HealthComponent.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"

EnemyManagerComponent::EnemyManagerComponent(std::shared_ptr<GameObject> player)
	: m_pCurrentRoom{ nullptr },
	m_pPlayer{ player }
{
}

void EnemyManagerComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	UpdateCurrentRoom(obj);
	AddSpawners(obj);
	SpawnEnemies(obj);
	SortEnemiesByPos();
	RemoveDeadEnemies();
}

std::shared_ptr<GameObject> EnemyManagerComponent::GetClosestEnemyInFront()
{
	if (m_Enemies.at(m_pCurrentRoom).empty())
		return nullptr;

	const auto direction = m_pPlayer->GetComponent<SpriteRenderComponent>()->GetDirection();
	
	std::shared_ptr<GameObject> closestEnemy = nullptr;
	const auto playerPos = m_pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition();
	Vector2f closestDist{0, 20000};
	for(auto& enemy : m_Enemies.at(m_pCurrentRoom))
	{
		auto enemyPos = enemy->GetComponent<EnemyMovementComponent>()->GetPosition();
		Vector2f distanceVec{ playerPos, enemyPos };

		double enemyAngle = atan2(distanceVec.y, distanceVec.x);
		enemyAngle = enemyAngle * 180 / M_PI;
		SpriteRenderComponent::Direction enemyDirection{};

		if (enemyAngle > 0)
		{
			if (enemyAngle > 45 && enemyAngle < 135)
			{
				enemyDirection = SpriteRenderComponent::Direction::up;
			}
			else if (enemyAngle > 135)
			{
				enemyDirection = SpriteRenderComponent::Direction::left;
			}
			else if (enemyAngle < 45)
			{
				enemyDirection = SpriteRenderComponent::Direction::right;
			}
		}
		else
		{
			if (enemyAngle < -45 && enemyAngle > -135)
			{
				enemyDirection = SpriteRenderComponent::Direction::down;
			}
			else if (enemyAngle < -135)
			{
				enemyDirection = SpriteRenderComponent::Direction::left;
			}
			else if (enemyAngle > -45)
			{
				enemyDirection = SpriteRenderComponent::Direction::right;
			}
		}
		
		if (enemyDirection != direction)
			continue;
		
		if (distanceVec.Length() < closestDist.Length())
		{
			closestDist = distanceVec;
			closestEnemy = enemy;
		}
	}
	
	return closestEnemy;
}

void EnemyManagerComponent::SortEnemiesByPos()
{
	auto enemies = m_Enemies[m_pCurrentRoom];

	if (enemies.empty())
		return;

	std::sort(enemies.begin(), enemies.end(), [](std::shared_ptr<GameObject>& lhs, std::shared_ptr<GameObject>& rhs)
		{
			return lhs->GetComponent<EnemyMovementComponent>()->GetPosition().y > rhs->GetComponent<EnemyMovementComponent>()->GetPosition().y;
		});

	for (auto& enemy : enemies)
	{
		SceneManager::GetInstance().GetCurrentScene()->MoveObjToBack(enemy);
	}
}

void EnemyManagerComponent::RemoveDeadEnemies()
{
	if (m_Enemies.find(m_pCurrentRoom) == m_Enemies.end())
		return;
	
	if (m_Enemies.at(m_pCurrentRoom).empty())
		return;

	m_Enemies.at(m_pCurrentRoom).erase(std::remove_if(
		m_Enemies.at(m_pCurrentRoom).begin(), m_Enemies.at(m_pCurrentRoom).end(), [](std::shared_ptr<GameObject> obj)
		{
			if(obj->GetComponent<HealthComponent>()->IsDead())
			{
				obj->GetComponent<ActivityComponent>()->Deactivate();
				std::cout << "Enemy killed!\n";
				return true;
			}
			return false;
		}
	), m_Enemies.at(m_pCurrentRoom).end());
}

void EnemyManagerComponent::UpdateCurrentRoom(GameObject& obj)
{
	if (m_pCurrentRoom == obj.GetComponent<MazeComponent>()->GetCurrentRoom())
		return;

	// Despawn enemies when going to another room
	for (auto& enemy : m_Enemies[m_pCurrentRoom])
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
		auto prototype = std::make_shared<DebugEnemyComponent>(rand() % 50 + 50, rand() % 50, rand() % 50 + 50, pos, m_pPlayer);

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

	// Als de spawner niet meer in de spawners container zit, betekent dat dat de player al in de kamer geweest is
	if (m_Spawners.find(m_pCurrentRoom) == m_Spawners.end())
	{
		if (m_Enemies.find(m_pCurrentRoom) == m_Enemies.end())
			return;

		for (auto& enemy : m_Enemies[m_pCurrentRoom])
		{
			enemy->GetComponent<ActivityComponent>()->Activate();
		}

		return;
	}

	for (auto& spawner : m_Spawners[m_pCurrentRoom])
	{
		auto enemy = spawner.Spawn();
		enemy->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(enemy);
		m_Enemies[m_pCurrentRoom].push_back(enemy);
	}

	m_Spawners.erase(m_pCurrentRoom);
}
