#include "pch.h"
#include "ObstacleTransformComponent.h"
#include <iostream>
#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include "ObstacleComponent.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"

void ObstacleTransformComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	CheckPlayerCollision(obj);
	CheckEnemyCollision(obj);
}

void ObstacleTransformComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& ObstacleTransformComponent::GetPosition() const
{
	return m_Position;
}

void ObstacleTransformComponent::CheckPlayerCollision(GameObject& obj) const
{
	const auto obstacleHitbox = Rectf{ m_Position.x,
	m_Position.y,
	obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

	auto pPlayer = obj.GetComponent<ObstacleComponent>()->GetPlayer();

	const auto playerHitbox = Rectf{ pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition().x,
	pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition().y,
	pPlayer->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	pPlayer->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

	if (utils::IsOverlapping(obstacleHitbox, playerHitbox))
	{
		const auto directionVector = Vector2f{ pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition(), m_Position };

		double angle = atan2(directionVector.y, directionVector.x);
		angle = angle * 180 / M_PI;

		auto directionX = PlayerMovementComponent::DirectionX::none;
		auto directionY = PlayerMovementComponent::DirectionY::none;

		if (angle > 0)
		{
			if (angle > 45 && angle < 135)
			{
				directionY = PlayerMovementComponent::DirectionY::up;
			}
			else if (angle > 135)
			{
				directionX = PlayerMovementComponent::DirectionX::left;
			}
			else if (angle < 45)
			{
				directionX = PlayerMovementComponent::DirectionX::right;
			}
		}
		else
		{
			if (angle < -45 && angle > -135)
			{
				directionY = PlayerMovementComponent::DirectionY::down;
			}
			else if (angle < -135)
			{
				directionX = PlayerMovementComponent::DirectionX::left;
			}
			else if (angle > -45)
			{
				directionX = PlayerMovementComponent::DirectionX::right;
			}
		}
		pPlayer->GetComponent<PlayerMovementComponent>()->SetObstacleCollisionData(directionX, directionY);
	}
}

void ObstacleTransformComponent::CheckEnemyCollision(GameObject& obj) const
{
	for (auto& enemy : obj.GetComponent<ObstacleComponent>()->GetEnemies())
	{
		const auto obstacleHitbox = Rectf{ m_Position.x,
		m_Position.y,
		obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
		obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

		const auto enemyHitbox = Rectf{ enemy->GetComponent<EnemyMovementComponent>()->GetPosition().x,
		enemy->GetComponent<EnemyMovementComponent>()->GetPosition().y,
		enemy->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
		enemy->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

		if (utils::IsOverlapping(obstacleHitbox, enemyHitbox))
		{
			const auto directionVector = Vector2f{ enemy->GetComponent<EnemyMovementComponent>()->GetPosition(), m_Position };
			const auto enemyVelocity = enemy->GetComponent<EnemyMovementComponent>()->GetCurrentVelocity();

			// Angle between vectors acquired through dot product
			const auto angle = acos((directionVector.x * enemyVelocity.x + directionVector.y * enemyVelocity.y)/
				(directionVector.Length() * enemyVelocity.Length()));

			std::cout << angle << '\n';

			if(angle < 1.57f)
				enemy->GetComponent<EnemyMovementComponent>()->ActivateInvertVelocityFlag();
		}
	}
}
