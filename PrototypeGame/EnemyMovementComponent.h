#pragma once
#include "MovementComponent.h"
#include "utils.h"

class EnemyMovementComponent final : public MovementComponent
{
public:
	enum class DirectionX
	{
		left = 2,
		right = 0,
		none
	};
	enum class DirectionY
	{
		up = 3,
		down = 1,
		none
	};
	enum class State
	{
		moving,
		idle
	};
	
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
	float m_Speed;
	Point2f m_Position;
	DirectionX m_DirectionX;
	DirectionY m_DirectionY;
	State m_State;
};
