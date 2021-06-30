#pragma once
#include "Component.h"

class MovementComponent : public Component
{
public:
	MovementComponent() = default;
	virtual ~MovementComponent() = default;
	MovementComponent(const MovementComponent& other) = delete;
	MovementComponent(MovementComponent&& other) = delete;
	void operator=(const MovementComponent& rhs) = delete;
	void operator=(const MovementComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) = 0;

	virtual void SetPosition(const Point2f& pos) = 0;
	virtual const Point2f& GetPosition() const = 0;
};