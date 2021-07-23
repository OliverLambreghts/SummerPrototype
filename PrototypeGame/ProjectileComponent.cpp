#include "pch.h"
#include "ProjectileComponent.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"
#include "ProjectileMovementComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SpriteRenderComponent.h"

ProjectileComponent::ProjectileComponent()
	: m_RangeTime{},
	m_CurrentTime{}
{
}

void ProjectileComponent::Update(float elapsedSec, GameObject& /*obj*/)
{
	if (!m_pActiveProjectile)
		return;

	m_CurrentTime += elapsedSec;

	if(m_CurrentTime >= m_RangeTime)
	{
		SceneManager::GetInstance().GetCurrentScene()->RemoveObj(m_pActiveProjectile);
		m_pActiveProjectile.reset();
		m_CurrentTime = 0.f;
	}
}

void ProjectileComponent::Shoot(const Point2f& pos, const Vector2f& dir, float rangeTime)
{
	if (m_pActiveProjectile)
		return;

	m_RangeTime = rangeTime;
	m_pActiveProjectile = std::make_shared<GameObject>();
	m_pActiveProjectile->AddComponent(std::make_shared<ProjectileMovementComponent>(m_Speed, pos, dir));
	m_pActiveProjectile->GetComponent<ProjectileMovementComponent>()->SetPosition(pos);
	m_pActiveProjectile->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/Projectile.png", 1, 1, 1.f / 10.f, 1));

	SceneManager::GetInstance().GetCurrentScene()->Add(m_pActiveProjectile);
}

void ProjectileComponent::ResetProjectile()
{
	SceneManager::GetInstance().GetCurrentScene()->RemoveObj(m_pActiveProjectile);
	m_pActiveProjectile.reset();
	m_CurrentTime = 0.f;
}
