#pragma once
#include "MovementComponent.h"
#include "utils.h"

class EnemyMovementComponent final : public MovementComponent
{
public:
	EnemyMovementComponent(float speed, const Point2f& pos);
	virtual ~EnemyMovementComponent() override = default;
	EnemyMovementComponent(const EnemyMovementComponent& other) = delete;
	EnemyMovementComponent(EnemyMovementComponent&& other) = delete;
	void operator=(const EnemyMovementComponent& rhs) = delete;
	void operator=(const EnemyMovementComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	virtual void SetPosition(const Point2f & pos) override;
	virtual const Point2f& GetPosition() const override;

	void ActivateKnockBack(const Point2f& playerPos);

	void ActivateInvertVelocityFlag();
	const Vector2f& GetCurrentVelocity() const;
private:
	void DetermineDirection(const Vector2f& velocity, GameObject& obj) const;
	bool ApplyKnockBack(float elapsedSec, GameObject& obj);
	bool CanSeePlayer(float range, const Vector2f& velocity, GameObject& obj);
	Vector2f DetermineBehavior(const Point2f& playerPos, float elapsedSec, GameObject& obj);
	void UpdateWanderBehavior(Vector2f& velocity, float elapsedSec, GameObject& obj);
	void UpdateSeekBehavior(Vector2f& velocity, float elapsedSec);
	void DetermineForwardVector();
	void CalculateRandomVelocity();
	void CheckWallCollision(GameObject& obj);
	bool IsAgainstWall(GameObject& obj) const;
	void UpdateObstacleEscape(float elapsedSec);
	
	float m_Speed;
	Point2f m_Position;
	bool m_IsKnockedBack;
	const float m_KnockBackTimer = 0.15f, m_KBSpeed = 400.f;
	float m_ActiveKBTimer;
	Vector2f m_KBVelocity;
	bool m_HasSeenPlayer;
	bool m_IsAgainstObstacle;
	bool m_IsEscapingObstacle;
	Vector2f m_CurrentVelocity;
	float m_ObstacleCollisionTimer;

	// WANDER DATA
	Vector2f m_WanderFwdVector;
	float m_WanderAngle;
	float m_WanderTimer = 0.f;
};
