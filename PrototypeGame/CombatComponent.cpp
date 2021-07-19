#include "pch.h"
#include "CombatComponent.h"
#include "EnemyComponent.h"
#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"

CombatComponent::CombatComponent()
	: m_Timer{},
	m_IsCDActive{ false }
{
}

void CombatComponent::Update(float elapsedSec, GameObject& obj)
{
	if(m_IsCDActive)
	{
		m_Timer += elapsedSec;

		if(m_Timer >= m_Cooldown)
		{
			m_Timer = 0.f;
			m_IsCDActive = false;
		}
		return;
	}
	
	if (!CheckCollision(obj))
		return;

	Attack(obj);
}

bool CombatComponent::CheckCollision(GameObject& obj) const
{
	auto player = obj.GetComponent<EnemyComponent>()->GetPlayer();

	const Rectf playerHitbox{ player->GetComponent<PlayerMovementComponent>()->GetPosition().x,
	player->GetComponent<PlayerMovementComponent>()->GetPosition().y ,
	player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

	const Rectf enemyHitbox{ obj.GetComponent<EnemyMovementComponent>()->GetPosition().x,
	obj.GetComponent<EnemyMovementComponent>()->GetPosition().y ,
	obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

	return utils::IsOverlapping(playerHitbox, enemyHitbox);
}

void CombatComponent::Attack(GameObject& obj)
{
	auto player = obj.GetComponent<EnemyComponent>()->GetPlayer();
	const auto damage = obj.GetComponent<EnemyComponent>()->GetPower();

	player->GetComponent<HealthComponent>()->AddHealth(-damage);
	player->GetComponent<PlayerMovementComponent>()->ActivateKnockBack(obj.GetComponent<EnemyMovementComponent>()->GetPosition());

	m_IsCDActive = true;
}
