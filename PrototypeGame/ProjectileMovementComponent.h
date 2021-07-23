#pragma once
#include "MovementComponent.h"
#include "utils.h"

class ProjectileMovementComponent final : public MovementComponent
{
public:
	ProjectileMovementComponent(float speed, const Point2f pos, const Vector2f& fwdVec);
	virtual ~ProjectileMovementComponent() override = default;
	ProjectileMovementComponent(const ProjectileMovementComponent& other) = delete;
	ProjectileMovementComponent(ProjectileMovementComponent&& other) = delete;
	void operator=(const ProjectileMovementComponent& rhs) = delete;
	void operator=(const ProjectileMovementComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	virtual void SetPosition(const Point2f & pos) override;
	virtual const Point2f& GetPosition() const override;
private:
	float m_Speed;
	Point2f m_Position;
	Vector2f m_ForwardVec;
};
