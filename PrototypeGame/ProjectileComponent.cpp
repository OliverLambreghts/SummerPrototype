#include "pch.h"
#include "ProjectileComponent.h"

#include <iostream>


#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PlayerMovementComponent.h"
#include "ProjectileMovementComponent.h"
#include "RangedKeyComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SpriteRenderComponent.h"

ProjectileComponent::ProjectileComponent()
	: m_RangeTime{},
	m_CurrentTime{},
	m_Speed{}
{
}

void ProjectileComponent::Update(float elapsedSec, GameObject& obj)
{
	if (!m_pActiveProjectile)
		return;

	m_CurrentTime += elapsedSec;

	if (m_CurrentTime >= m_RangeTime)
	{
		ResetProjectile();
		return;
	}

	if (!CheckCollision())
		return;

	HitEnemy(obj);
}

bool ProjectileComponent::CheckCollision()
{
	for (auto& enemy : m_Enemies)
	{
		const Rectf enemyHitbox{ enemy->GetComponent<EnemyMovementComponent>()->GetPosition().x,
	enemy->GetComponent<EnemyMovementComponent>()->GetPosition().y ,
	enemy->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	enemy->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

		const Rectf projHitbox{ m_pActiveProjectile->GetComponent<ProjectileMovementComponent>()->GetPosition().x,
	m_pActiveProjectile->GetComponent<ProjectileMovementComponent>()->GetPosition().y ,
	m_pActiveProjectile->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	m_pActiveProjectile->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

		if (utils::IsOverlapping(enemyHitbox, projHitbox))
		{
			m_pHitEnemy = enemy;
			return true;
		}
	}

	return false;
}

void ProjectileComponent::HitEnemy(GameObject& obj)
{
	auto rangedKeyComp = obj.GetComponent<RangedKeyComponent>();
	
	std::cout << "Attacking with " << rangedKeyComp->m_pWeaponQuality->GetName() << ' ' << rangedKeyComp->m_Name << '\n';
	if (rangedKeyComp->m_pProc && rangedKeyComp->m_pProc->IsProcActive())
	{
		std::cout << "Proc activates!\n";
		m_pHitEnemy->GetComponent<HealthComponent>()->SetProc(rangedKeyComp->m_pProc);
	}

	const auto damage = static_cast<float>(rangedKeyComp->m_Damage) * rangedKeyComp->m_pWeaponQuality->GetDmgMultiplier();
	std::cout << "Hitting enemy for " << damage << " damage!\n";
	m_pHitEnemy->GetComponent<HealthComponent>()->AddHealth(static_cast<int>(-damage));
	
	m_pHitEnemy->GetComponent<EnemyMovementComponent>()->ActivateKnockBack(
		m_pActiveProjectile->GetComponent<ProjectileMovementComponent>()->GetPosition());

	m_pHitEnemy.reset();
	ResetProjectile();
}

void ProjectileComponent::Shoot(const Point2f& pos, const Vector2f& dir, float rangeTime, float projSpeed,
	std::vector<std::shared_ptr<GameObject>> enemies)
{
	if (m_pActiveProjectile)
		return;

	m_Enemies = enemies;
	m_Speed = projSpeed;
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