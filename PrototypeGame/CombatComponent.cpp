#include "pch.h"
#include "CombatComponent.h"
#include "ActivityComponent.h"
#include "EnemyComponent.h"
#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PlayerMovementComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

CombatComponent::CombatComponent()
	: m_Timer{},
	m_IsCDActive{ false },
	m_pSlashEffect{ std::make_shared<GameObject>() }
{
	// Slash effect
	m_pSlashEffect->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/Slash.png", 5, 2, 1.f / 20.f, 1));
	m_pSlashEffect->AddComponent(std::make_shared<ActivityComponent>());
	m_pSlashEffect->AddComponent(std::make_shared<TransformComponent>());
	SceneManager::GetInstance().GetCurrentScene()->Add(m_pSlashEffect);
}

void CombatComponent::Update(float elapsedSec, GameObject& obj)
{
	CheckSlashEffect();
	
	if (m_IsCDActive)
	{
		m_Timer += elapsedSec;

		if (m_Timer >= m_Cooldown)
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

	// Activate an attack animation here: cloud of air wooshing towards the player for example
	m_pSlashEffect->GetComponent<TransformComponent>()->SetPosition(obj.GetComponent<EnemyMovementComponent>()->GetPosition()
	+ obj.GetComponent<EnemyMovementComponent>()->GetCurrentVelocity());
	m_pSlashEffect->GetComponent<SpriteRenderComponent>()->Move();
	m_pSlashEffect->GetComponent<ActivityComponent>()->Activate();
	// Nog instellen dat je de sprite van de slash kan mirroren als de enemy's velocity naar links gericht is
	if (obj.GetComponent<EnemyMovementComponent>()->GetCurrentVelocity().x < 0.f)
		m_pSlashEffect->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::left);

	m_IsCDActive = true;
}

void CombatComponent::CheckSlashEffect() const
{
	if (m_pSlashEffect->GetComponent<ActivityComponent>()->GetActivity())
	{
		if (m_pSlashEffect->GetComponent<SpriteRenderComponent>()->GetSprite().GetActFrame()
			== m_pSlashEffect->GetComponent<SpriteRenderComponent>()->GetSprite().GetCols() - 1)
		{
			m_pSlashEffect->GetComponent<ActivityComponent>()->Deactivate();
			m_pSlashEffect->GetComponent<SpriteRenderComponent>()->GetSprite().ResetSprite();

			m_pSlashEffect->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::down);
		}
	}
}
