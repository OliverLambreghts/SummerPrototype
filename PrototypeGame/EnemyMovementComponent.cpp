#include "pch.h"
#include "EnemyMovementComponent.h"
#include <iostream>
#include "ActivityComponent.h"
#include "DebugEnemyComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"
#include "Sprite.h"
#include "SpriteRenderComponent.h"

EnemyMovementComponent::EnemyMovementComponent(float speed, const Point2f& pos)
	: m_Position{ pos },
	m_Speed{ speed },
	m_IsKnockedBack{ false },
	m_ActiveKBTimer{},
	m_KBVelocity{},
	m_HasSeenPlayer{ false },
	m_WanderFwdVector{ 1.f, 0.f },
	m_WanderAngle{},
	m_IsAgainstObstacle{ false },
	m_CurrentVelocity{}
{
}

void EnemyMovementComponent::Update(float elapsedSec, GameObject& obj)
{
	if (!obj.GetComponent<ActivityComponent>()->GetActivity())
		return;

	auto player = obj.GetComponent<DebugEnemyComponent>()->GetPlayer();
	const auto playerPos = obj.GetComponent<DebugEnemyComponent>()->GetPlayer()->GetComponent<PlayerMovementComponent>()->GetPosition();

	const float width = obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth();
	const float height = obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight();

	// Apply knockback when hit by player
	if (ApplyKnockBack(elapsedSec, obj))
		return;

	// Idle enemy when he reaches the player
	if (utils::IsOverlapping(Rectf{ playerPos.x, playerPos.y,
		player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
		player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() },
		Rectf{ m_Position.x, m_Position.y, width, height }))
	{
		obj.GetComponent<SpriteRenderComponent>()->Idle();
		return;
	}

	obj.GetComponent<SpriteRenderComponent>()->Move();

	const auto velocity = DetermineBehavior(playerPos, elapsedSec, obj);

	DetermineDirection(velocity, obj);
}

bool EnemyMovementComponent::CanSeePlayer(float range, const Vector2f& velocity, GameObject& /*obj*/)
{
	if (velocity.Length() > range && !m_HasSeenPlayer)
	{
		//obj.GetComponent<SpriteRenderComponent>()->Idle();
		return false;
	}
	if (!m_HasSeenPlayer)
		m_HasSeenPlayer = true;

	return true;
}

Vector2f EnemyMovementComponent::DetermineBehavior(const Point2f& playerPos, float elapsedSec, GameObject& obj)
{
	// Calculate desired velocity (for the direction)
	Vector2f velocity = playerPos - m_Position;
	constexpr float attentionRange = 250.f;

	// Wander when the enemy hasn't seen the player yet
	if (!CanSeePlayer(attentionRange, velocity, obj))
		UpdateWanderBehavior(velocity, elapsedSec, obj);
	else
		UpdateSeekBehavior(velocity, elapsedSec);

	m_CurrentVelocity = velocity;
	
	return velocity;
}

void EnemyMovementComponent::UpdateSeekBehavior(Vector2f& velocity, float elapsedSec)
{
	velocity = velocity.Normalized();
	velocity *= m_Speed;

	if (m_IsAgainstObstacle)
	{
		m_IsAgainstObstacle = false;
		return;
	}

	m_Position += velocity * elapsedSec;
}

void EnemyMovementComponent::UpdateWanderBehavior(Vector2f& velocity, float elapsedSec, GameObject& obj)
{
	// Determine the forward vector for the first time
	if (m_WanderFwdVector.Length() == 1.f)
		DetermineForwardVector();

	CalculateRandomVelocity();
	CheckWallCollision(obj);
	
	if (m_IsAgainstObstacle)
	{
		m_IsAgainstObstacle = false;
		return;
	}
	
	m_Position += m_WanderFwdVector * elapsedSec;
	velocity = m_WanderFwdVector;
	m_WanderTimer += elapsedSec;

	const float wanderThreshold = 2.f;
	if (m_WanderTimer >= wanderThreshold)
	{
		m_WanderFwdVector = m_WanderFwdVector.Normalized();
		m_WanderTimer = 0.f;
	}
}

void EnemyMovementComponent::DetermineForwardVector()
{
	// Determine random forward vector
	const Point2f toPos = Point2f{ static_cast<float>(rand() % static_cast<int>(Game::GetWindowDimension())),
		static_cast<float>(rand() % static_cast<int>(Game::GetWindowDimension())) };

	// Adjust to given speed
	m_WanderFwdVector = Vector2f{ m_Position, toPos };
	m_WanderFwdVector = m_WanderFwdVector.Normalized();
	m_WanderFwdVector *= m_Speed;
}

void EnemyMovementComponent::CalculateRandomVelocity()
{
	const auto randAngle = static_cast<float>(rand() % 181);
	m_WanderAngle += randAngle;

	const auto target = Vector2f{ cos(m_WanderAngle), sin(m_WanderAngle) };

	m_WanderFwdVector += target;

	m_WanderFwdVector = m_WanderFwdVector.Normalized();
	m_WanderFwdVector *= m_Speed;
}

void EnemyMovementComponent::CheckWallCollision(GameObject& obj)
{
	if (m_Position.x + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() >= Game::GetWindowDimension()
		|| m_Position.x <= 0.f ||
		m_Position.y + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() >= Game::GetWindowDimension() ||
		m_Position.y <= 0.f)
		m_WanderFwdVector *= -1;
}

bool EnemyMovementComponent::IsAgainstWall(GameObject& obj) const
{
	return (m_Position.x + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() >= Game::GetWindowDimension()
		|| m_Position.x <= 0.f ||
		m_Position.y + obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() >= Game::GetWindowDimension() ||
		m_Position.y <= 0.f);
}

void EnemyMovementComponent::DetermineDirection(const Vector2f& velocity, GameObject& obj) const
{
	double angle = atan2(velocity.y, velocity.x);
	angle = angle * 180 / M_PI;

	if (angle > 0)
	{
		if (angle > 45 && angle < 135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::up);
		}
		else if (angle > 135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::left);
		}
		else if (angle < 45)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::right);
		}
	}
	else
	{
		if (angle < -45 && angle > -135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::down);
		}
		else if (angle < -135)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::left);
		}
		else if (angle > -45)
		{
			obj.GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::right);
		}
	}
}

bool EnemyMovementComponent::ApplyKnockBack(float elapsedSec, GameObject& obj)
{
	if (IsAgainstWall(obj) || m_IsAgainstObstacle)
	{
		m_IsAgainstObstacle = false;
		return false;
	}
	
	if (m_IsKnockedBack)
	{
		m_ActiveKBTimer += elapsedSec;

		auto KBVel = m_KBVelocity.Normalized();
		KBVel *= m_KBSpeed;
		
		m_Position += KBVel * elapsedSec;

		if (m_ActiveKBTimer >= m_KnockBackTimer)
		{
			m_ActiveKBTimer = 0.f;
			m_IsKnockedBack = false;
		}
		return true;
	}
	return false;
}

void EnemyMovementComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& EnemyMovementComponent::GetPosition() const
{
	return m_Position;
}

void EnemyMovementComponent::ActivateKnockBack(const Point2f& playerPos)
{
	m_IsKnockedBack = true;

	m_KBVelocity = Vector2f{ playerPos, m_Position };
}

void EnemyMovementComponent::ActivateInvertVelocityFlag()
{
	m_IsAgainstObstacle = true;
}

const Vector2f& EnemyMovementComponent::GetCurrentVelocity() const
{
	return m_CurrentVelocity;
}

