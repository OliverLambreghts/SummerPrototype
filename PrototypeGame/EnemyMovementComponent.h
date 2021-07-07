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
private:
	void DetermineDirection(const Vector2f& velocity, GameObject& obj) const;
	
	float m_Speed;
	Point2f m_Position;
};
