#pragma once
#include "MovementComponent.h"
#include "utils.h"

class ObstacleTransformComponent final : public MovementComponent
{
public:
	ObstacleTransformComponent() = default;
	virtual ~ObstacleTransformComponent() override = default;
	ObstacleTransformComponent(const ObstacleTransformComponent& other) = delete;
	ObstacleTransformComponent(ObstacleTransformComponent&& other) = delete;
	void operator=(const ObstacleTransformComponent& rhs) = delete;
	void operator=(const ObstacleTransformComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	virtual void SetPosition(const Point2f & pos) override;
	virtual const Point2f& GetPosition() const override;
private:
	void CheckPlayerCollision(GameObject& obj) const;
	void CheckEnemyCollision(GameObject& obj) const;
	
	Point2f m_Position;
};
