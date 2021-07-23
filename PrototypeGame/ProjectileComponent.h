#pragma once
#include <memory>
#include "Component.h"
#include "Vector2f.h"

class ProjectileComponent final : public Component
{
public:
	ProjectileComponent();
	virtual ~ProjectileComponent() override = default;
	ProjectileComponent(const ProjectileComponent& other) = delete;
	ProjectileComponent(ProjectileComponent&& other) = delete;
	void operator=(const ProjectileComponent& rhs) = delete;
	void operator=(const ProjectileComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;
	void Shoot(const Point2f& pos, const Vector2f& dir, float rangeTime);
	void ResetProjectile();
private:
	std::shared_ptr<GameObject> m_pActiveProjectile = nullptr;
	const float m_Speed = 200.f;
	float m_RangeTime, m_CurrentTime;
};
