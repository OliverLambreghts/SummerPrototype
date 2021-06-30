#pragma once
#include "MovementComponent.h"

class TransformComponent final : public MovementComponent
{
public:
	TransformComponent() = default;
	virtual ~TransformComponent() override = default;
	TransformComponent(const TransformComponent& other) = delete;
	TransformComponent(TransformComponent&& other) = delete;
	void operator=(const TransformComponent& rhs) = delete;
	void operator=(const TransformComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject& obj) override;
	
	virtual void SetPosition(const Point2f& pos) override;
	virtual const Point2f& GetPosition() const override;
private:
	Point2f m_Position;
};
