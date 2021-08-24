#include "pch.h"
#include "ObstacleManagerComponent.h"
#include <algorithm>
#include <iostream>
#include "GameObject.h"
#include "ActivityComponent.h"
#include "EnemyManagerComponent.h"
#include "Game.h"
#include "HealthComponent.h"
#include "MazeComponent.h"
#include "ObstacleComponent.h"
#include "ObstacleTransformComponent.h"
#include "PlayerMovementComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

ObstacleManagerComponent::ObstacleManagerComponent(std::shared_ptr<GameObject> player)
	: m_pCurrentRoom{ nullptr },
	m_pPlayer{ player }
{
}

void ObstacleManagerComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	m_ObstacleDirections.clear();
	UpdateCurrentRoom(obj);
	AddObstacles(obj);
	ActivateObstacles(obj);
	RemoveBrokenObstacles();
}

std::shared_ptr<GameObject> ObstacleManagerComponent::GetClosestObstacleInFront()
{
	if (m_pObstacles.find(m_pCurrentRoom) == m_pObstacles.end())
		return nullptr;

	if (m_pObstacles.at(m_pCurrentRoom).empty())
		return nullptr;

	const auto direction = m_pPlayer->GetComponent<SpriteRenderComponent>()->GetDirection();

	std::shared_ptr<GameObject> closestObstacle = nullptr;
	const auto playerPos = m_pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition();
	Vector2f closestDist{ 0, 20000 };
	for (auto& obstacle : m_pObstacles.at(m_pCurrentRoom))
	{
		auto obstaclePos = obstacle->GetComponent<ObstacleTransformComponent>()->GetPosition();
		Vector2f distanceVec{ playerPos, obstaclePos };

		double obstacleAngle = atan2(distanceVec.y, distanceVec.x);
		obstacleAngle = obstacleAngle * 180 / M_PI;
		SpriteRenderComponent::Direction enemyDirection{};

		if (obstacleAngle > 0)
		{
			if (obstacleAngle > 45 && obstacleAngle < 135)
			{
				enemyDirection = SpriteRenderComponent::Direction::up;
			}
			else if (obstacleAngle > 135)
			{
				enemyDirection = SpriteRenderComponent::Direction::left;
			}
			else if (obstacleAngle < 45)
			{
				enemyDirection = SpriteRenderComponent::Direction::right;
			}
		}
		else
		{
			if (obstacleAngle < -45 && obstacleAngle > -135)
			{
				enemyDirection = SpriteRenderComponent::Direction::down;
			}
			else if (obstacleAngle < -135)
			{
				enemyDirection = SpriteRenderComponent::Direction::left;
			}
			else if (obstacleAngle > -45)
			{
				enemyDirection = SpriteRenderComponent::Direction::right;
			}
		}

		if (enemyDirection != direction)
			continue;

		if (distanceVec.Length() < closestDist.Length())
		{
			closestDist = distanceVec;
			closestObstacle = obstacle;
		}
	}

	return closestObstacle;
}

void ObstacleManagerComponent::UpdateCurrentRoom(GameObject& obj)
{
	if (m_pCurrentRoom == obj.GetComponent<MazeComponent>()->GetCurrentRoom())
		return;

	// Despawn obstacles when going to another room (no idea if this works yet)
	if (m_pObstacles.find(m_pCurrentRoom) != m_pObstacles.end() && !m_pObstacles.at(m_pCurrentRoom).empty())
	{
		std::for_each(m_pObstacles.at(m_pCurrentRoom).begin(), m_pObstacles.at(m_pCurrentRoom).end(), [](
			std::shared_ptr<GameObject> obstacle)
			{
				obstacle->GetComponent<ActivityComponent>()->Deactivate();
			});
	}

	m_pCurrentRoom = obj.GetComponent<MazeComponent>()->GetCurrentRoom();
	m_HasSpawnedRandomObstacle = false;
}

void ObstacleManagerComponent::AddObstacles(GameObject& obj)
{
	// Wait for maze generation to finish so that all room types are correct.
	if (!obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
		return;

	// If obstacles already spawned in, no need to do it again.
	if (m_pObstacles.find(m_pCurrentRoom) != m_pObstacles.end())
		return;

	CheckNeighbors();
	SpawnTreasureObstacles(obj);

	if (m_pCurrentRoom->isBeginRoom || m_pCurrentRoom->type == RoomType::treasure || m_pCurrentRoom->type == RoomType::vendor)
		return;

	if (m_HasSpawnedRandomObstacle)
		return;
	
	const auto randNr = rand() % 101;
	constexpr int spawnPercentage = 20;
	if (randNr <= spawnPercentage)
		SpawnRandomObstacle(obj);

	m_HasSpawnedRandomObstacle = true;
}

void ObstacleManagerComponent::CheckNeighbors()
{
	// Collect all directions of rooms that are treasure rooms and have only 1 connection
	if (m_pCurrentRoom->down.isConnected && m_pCurrentRoom->down.neighbor->type == RoomType::treasure &&
		m_pCurrentRoom->down.neighbor->neighborAmount == 1)
		m_ObstacleDirections.push_back(Direction::down);
	else if (m_pCurrentRoom->up.isConnected && m_pCurrentRoom->up.neighbor->type == RoomType::treasure &&
		m_pCurrentRoom->up.neighbor->neighborAmount == 1)
		m_ObstacleDirections.push_back(Direction::up);
	else if (m_pCurrentRoom->left.isConnected && m_pCurrentRoom->left.neighbor->type == RoomType::treasure &&
		m_pCurrentRoom->left.neighbor->neighborAmount == 1)
		m_ObstacleDirections.push_back(Direction::left);
	else if (m_pCurrentRoom->right.isConnected && m_pCurrentRoom->right.neighbor->type == RoomType::treasure &&
		m_pCurrentRoom->right.neighbor->neighborAmount == 1)
		m_ObstacleDirections.push_back(Direction::right);
}

void ObstacleManagerComponent::SpawnTreasureObstacles(GameObject& obj)
{
	for (auto direction : m_ObstacleDirections)
	{
		switch (direction)
		{
		case Direction::down:
			SpawnDownTreasureObstacles(obj);
			break;
		case Direction::left:
			SpawnLeftTreasureObstacles(obj);
			break;
		case Direction::right:
			SpawnRightTreasureObstacles(obj);
			break;
		case Direction::up:
			SpawnUpTreasureObstacles(obj);
			break;
		}
	}
}

void ObstacleManagerComponent::ActivateObstacles(GameObject& obj)
{
	if (!obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
		return;

	if (m_pObstacles.find(m_pCurrentRoom) == m_pObstacles.end())
		return;

	if (m_pObstacles.at(m_pCurrentRoom).empty())
		return;

	for (auto& obstacle : m_pObstacles.at(m_pCurrentRoom))
	{
		obstacle->GetComponent<ActivityComponent>()->Activate();
	}
}

void ObstacleManagerComponent::RemoveBrokenObstacles()
{
	if (m_pObstacles.find(m_pCurrentRoom) == m_pObstacles.end())
		return;

	if (m_pObstacles.at(m_pCurrentRoom).empty())
		return;

	m_pObstacles.at(m_pCurrentRoom).erase(std::remove_if(
		m_pObstacles.at(m_pCurrentRoom).begin(), m_pObstacles.at(m_pCurrentRoom).end(), [](std::shared_ptr<GameObject> obj)
		{
			if (obj->GetComponent<HealthComponent>()->IsDead())
			{
				obj->GetComponent<ActivityComponent>()->Deactivate();
				std::cout << "Enemy killed!\n";
				return true;
			}
			return false;
		}
	), m_pObstacles.at(m_pCurrentRoom).end());
}

void ObstacleManagerComponent::SpawnLeftTreasureObstacles(GameObject& obj)
{
	Point2f pos{ 25.f, Game::GetWindowDimension() / 2.f - 10.f };

	for (int i{}; i < 4; ++i)
	{
		auto obstacle = std::make_shared<ObstacleComponent>(true, "Boulder.png", pos, m_pPlayer,
			obj.GetComponent<EnemyManagerComponent>()->GetEnemiesInCurrentRoom());

		auto boulder = obstacle->Clone();

		const auto width = boulder->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth();

		if (i == 0 || i == 3)
		{
			pos.x = width - 10.f;
		}
		if (i == 1 || i == 2)
		{
			pos.x = width - 10.f + 25.f;
		}

		boulder->GetComponent<ObstacleTransformComponent>()->SetPosition(pos);

		boulder->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(boulder);
		m_pObstacles[m_pCurrentRoom].push_back(boulder);

		pos.y -= width;
	}
}

void ObstacleManagerComponent::SpawnRightTreasureObstacles(GameObject& obj)
{
	Point2f pos{ 0.f, Game::GetWindowDimension() / 2.f - 10.f };

	for (int i{}; i < 4; ++i)
	{
		auto obstacle = std::make_shared<ObstacleComponent>(true, "Boulder.png", pos, m_pPlayer,
			obj.GetComponent<EnemyManagerComponent>()->GetEnemiesInCurrentRoom());

		auto boulder = obstacle->Clone();

		const auto width = boulder->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth();

		if (i == 0 || i == 3)
		{
			pos.x = Game::GetWindowDimension() - width - 25.f;
		}
		if (i == 1 || i == 2)
		{
			pos.x = Game::GetWindowDimension() - width - 50.f;
		}

		boulder->GetComponent<ObstacleTransformComponent>()->SetPosition(pos);

		boulder->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(boulder);
		m_pObstacles[m_pCurrentRoom].push_back(boulder);

		pos.y -= width;
	}
}

void ObstacleManagerComponent::SpawnUpTreasureObstacles(GameObject& obj)
{
	Point2f pos{ Game::GetWindowDimension() / 2.f - 10.f, 0.f };

	for (int i{}; i < 4; ++i)
	{
		auto obstacle = std::make_shared<ObstacleComponent>(true, "Boulder.png", pos, m_pPlayer,
			obj.GetComponent<EnemyManagerComponent>()->GetEnemiesInCurrentRoom());

		auto boulder = obstacle->Clone();

		const auto width = boulder->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth();

		if (i == 0 || i == 3)
		{
			pos.y = Game::GetWindowDimension() - width - 25.f;
		}
		if (i == 1 || i == 2)
		{
			pos.y = Game::GetWindowDimension() - width - 50.f;
		}

		boulder->GetComponent<ObstacleTransformComponent>()->SetPosition(pos);

		boulder->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(boulder);
		m_pObstacles[m_pCurrentRoom].push_back(boulder);

		pos.x -= width;
	}
}

void ObstacleManagerComponent::SpawnDownTreasureObstacles(GameObject& obj)
{
	Point2f pos{ Game::GetWindowDimension() / 2.f - 10.f, 0.f };

	for (int i{}; i < 4; ++i)
	{
		auto obstacle = std::make_shared<ObstacleComponent>(true, "Boulder.png", pos, m_pPlayer,
			obj.GetComponent<EnemyManagerComponent>()->GetEnemiesInCurrentRoom());

		auto boulder = obstacle->Clone();

		const auto width = boulder->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth();

		if (i == 0 || i == 3)
		{
			pos.y = width - 10.f;
		}
		if (i == 1 || i == 2)
		{
			pos.y = width - 10.f + 25.f;
		}

		boulder->GetComponent<ObstacleTransformComponent>()->SetPosition(pos);

		boulder->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(boulder);
		m_pObstacles[m_pCurrentRoom].push_back(boulder);

		pos.x -= width;
	}
}

void ObstacleManagerComponent::SpawnRandomObstacle(GameObject& obj)
{
	auto obstacle = std::make_shared<ObstacleComponent>(true, "Boulder.png", Point2f{ Game::GetWindowDimension() / 2.f,
	Game::GetWindowDimension() / 2.f }, m_pPlayer,
		obj.GetComponent<EnemyManagerComponent>()->GetEnemiesInCurrentRoom());
	auto boulder = obstacle->Clone();
	boulder->GetComponent<ActivityComponent>()->Activate();
	SceneManager::GetInstance().GetCurrentScene()->Add(boulder);
	m_pObstacles[m_pCurrentRoom].push_back(boulder);
}
