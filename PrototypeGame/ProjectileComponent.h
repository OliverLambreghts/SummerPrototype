#pragma once
#include <functional>
#include <memory>
#include <vector>
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
	void Shoot(const Point2f& pos, const Vector2f& dir, float rangeTime, float projSpeed, std::vector<std::shared_ptr<GameObject>> enemies);
	void ResetProjectile();
private:
	bool CheckCollision();
	void HitEnemy(GameObject& obj);
	
	std::shared_ptr<GameObject> m_pActiveProjectile = nullptr, m_pHitEnemy = nullptr;
	float m_RangeTime, m_CurrentTime, m_Speed;
	std::vector<std::shared_ptr<GameObject>> m_Enemies;
};
