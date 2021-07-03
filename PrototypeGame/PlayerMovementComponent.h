#pragma once
#include "MovementComponent.h"
#include "utils.h"

class PlayerMovementComponent final : public MovementComponent
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

	PlayerMovementComponent();
	virtual ~PlayerMovementComponent() = default;
	PlayerMovementComponent(const PlayerMovementComponent& other) = delete;
	PlayerMovementComponent(PlayerMovementComponent&& other) = delete;
	void operator=(const PlayerMovementComponent& rhs) = delete;
	void operator=(const PlayerMovementComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	virtual void SetPosition(const Point2f & pos) override;
	virtual const Point2f& GetPosition() const override;

	void Move(DirectionX* directionX, DirectionY* directionY);
	void SetSpeed(float speed);
	void SetState(State state);

	State GetState() const;
	std::pair<DirectionX, DirectionY> GetDirections() const;
private:
	void HandleXMovement(float elapsedSec);
	void HandleYMovement(float elapsedSec);
	
	float m_Speed;
	Point2f m_Position;
	DirectionX m_DirectionX;
	DirectionY m_DirectionY;
	State m_State;
};
