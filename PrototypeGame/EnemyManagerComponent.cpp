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
#include "LootComponent.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"
#include "StatManager.h"

EnemyManagerComponent::EnemyManagerComponent(std::shared_ptr<GameObject> player)
	: m_pCurrentRoom{ nullptr },
	m_pPlayer{ player },
	m_SpawnDelay{},
	m_CanStartDelay{ false }
{
}

void EnemyManagerComponent::Update(float elapsedSec, GameObject& obj)
{
	UpdateCurrentRoom(obj);
	AddSpawners(obj);
	SpawnEnemies(elapsedSec, obj);
	RemoveDeadEnemies();
}

std::shared_ptr<GameObject> EnemyManagerComponent::GetClosestEnemyInFront()
{
	if (m_Enemies.find(m_pCurrentRoom) == m_Enemies.end() || m_Enemies.at(m_pCurrentRoom).empty())
		return nullptr;

	const auto direction = m_pPlayer->GetComponent<SpriteRenderComponent>()->GetDirection();

	std::shared_ptr<GameObject> closestEnemy = nullptr;
	const auto playerPos = m_pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition();
	Vector2f closestDist{ 0, 20000 };
	for (auto& enemy : m_Enemies.at(m_pCurrentRoom))
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

std::vector<std::shared_ptr<GameObject>> EnemyManagerComponent::GetEnemiesInCurrentRoom() const
{
	if (m_Enemies.find(m_pCurrentRoom) == m_Enemies.end())
		return std::vector<std::shared_ptr<GameObject>>{};
	
	return m_Enemies.at(m_pCurrentRoom);
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
			if (obj->GetComponent<HealthComponent>()->IsDead() && obj->GetComponent<LootComponent>()->HasSpawnedItem())
			{
				obj->GetComponent<ActivityComponent>()->Deactivate();
				std::cout << "Enemy killed!\n";
				++StatManager::GetInstance().GetCurrentStats().enemiesKilled;
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
	//m_CanStartDelay = true;
}

void EnemyManagerComponent::AddSpawners(GameObject& obj)
{
	if (obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
	{
		if ((m_pCurrentRoom->isBeginRoom || 
			m_pCurrentRoom->type == RoomType::boss) ||
			m_pCurrentRoom->type == RoomType::vendor &&
			m_Spawners.find(m_pCurrentRoom) != m_Spawners.end())
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
		auto prototype = std::make_shared<DebugEnemyComponent>(rand() % 50 + 50, rand() % 20, rand() % 50 + 50, pos, m_pPlayer);

		Spawner spawner{ prototype };
		spawners.push_back(spawner);
	}

	m_Spawners[m_pCurrentRoom] = spawners;

	// --- DEBUG CODE ---
}

void EnemyManagerComponent::SpawnEnemies(float /*elapsedSec*/, GameObject& obj)
{
	if (!obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
		return;

	// --- SPAWN DELAY ---
	/*if (!m_CanStartDelay)
		return;

	if (!UpdateDelayTimer(elapsedSec))
		return;*/
	// --- SPAWN DELAY ---

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

bool EnemyManagerComponent::UpdateDelayTimer(float elapsedSec)
{
	m_SpawnDelay += elapsedSec;

	const float delayThreshold = 0.2f;

	if (m_SpawnDelay < delayThreshold)
		return false;

	m_CanStartDelay = false;
	m_SpawnDelay = 0.f;

	return true;
}