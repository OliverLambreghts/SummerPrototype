#include "pch.h"
#include "RangedKeyComponent.h"

#include <functional>

#include "ActivityComponent.h"
#include "EnemyManagerComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "InventoryComponent.h"
#include "PlayerMovementComponent.h"
#include "ProjectileComponent.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

RangedKeyComponent::RangedKeyComponent(const std::string& name, int damage, float cooldown, std::shared_ptr<BaseProc> proc, std::shared_ptr<WeaponQuality> quality, const std::string& fileName,
	float rangeTime, float projSpeed)
	: m_Name{ name },
	m_Damage{ damage },
	m_Cooldown{ cooldown },
	m_pProc{ proc },
	m_pWeaponQuality{ quality },
	m_FileName{ fileName },
	m_IsCDActive{ false },
	m_CDTimer{},
	m_HasShot{ false },
	m_RangeTime{ rangeTime },
	m_ProjectileSpeed{ projSpeed },
	m_EffectTimer{}
{
}

std::shared_ptr<GameObject> RangedKeyComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<RangedKeyComponent>(m_Name, m_Damage, m_Cooldown, m_pProc, m_pWeaponQuality, m_FileName, m_RangeTime,
		m_ProjectileSpeed));
	clone->AddComponent(std::make_shared<TransformComponent>());
	clone->GetComponent<TransformComponent>()->SetPosition(Point2f{ Game::GetWindowDimension() / 2, Game::GetWindowDimension() / 2 });
	// Using test sprite for debugging purposes
	clone->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/" + m_FileName, 1, 1, 1.f / 10.f, 1));
	clone->AddComponent(std::make_shared<ProjectileComponent>());
	clone->AddComponent(std::make_shared<ActivityComponent>());
	return clone;
}

void RangedKeyComponent::Update(float elapsedSec, GameObject& obj)
{
	// Update effect based on its duration
	UpdateEffectTimer(elapsedSec);
	
	// Projectiel ook updaten
	obj.GetComponent<ProjectileComponent>()->Update(elapsedSec, obj);

	if (!m_IsCDActive)
		return;

	ActivateProjectile(obj);
	UpdateCDTimer(elapsedSec);
}

void RangedKeyComponent::ActivateProjectile(GameObject& obj)
{
	if (!m_HasShot)
	{
		m_HasShot = true;
		obj.GetComponent<ProjectileComponent>()->Shoot(m_ProjectilePos, m_ProjectileDir, m_RangeTime, m_ProjectileSpeed,
			m_Enemies);
	}
}

void RangedKeyComponent::UpdateCDTimer(float elapsedSec)
{
	// Cooldown
	m_CDTimer += elapsedSec;

	if (m_CDTimer >= m_Cooldown)
	{
		m_CDTimer = 0.f;
		m_IsCDActive = false;
		m_HasShot = false;
	}
}

void RangedKeyComponent::UpdateEffectTimer(float elapsedSec)
{
	if (m_pEffect)
	{
		m_EffectTimer += elapsedSec;

		if (m_EffectTimer >= m_pEffect->GetDuration())
		{
			m_EffectTimer = 0.f;
			m_pEffect = nullptr;
		}
	}
}

void RangedKeyComponent::OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> world)
{
	// Hier moet de cooldown gechecked worden en als die niet actief is wordt een projectile afgeschoten via de
	// ProjectileComponent.
	if (m_IsCDActive)
		return;

	m_IsCDActive = true;

	m_ProjectilePos = player->GetComponent<PlayerMovementComponent>()->GetPosition();

	const auto dir = player->GetComponent<SpriteRenderComponent>()->GetDirection();

	switch (dir)
	{
	case SpriteRenderComponent::Direction::right:
		m_ProjectileDir = Vector2f{ 1.f, 0.f };
		m_ProjectilePos.y += (player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() / 2);
		m_ProjectilePos.x += (player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() / 2);
		break;
	case SpriteRenderComponent::Direction::down:
		m_ProjectileDir = Vector2f{ 0.f, -1.f };
		m_ProjectilePos.x += (player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() / 3);
		break;
	case SpriteRenderComponent::Direction::left:
		m_ProjectileDir = Vector2f{ -1.f, 0.f };
		m_ProjectilePos.y += (player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() / 2);
		break;
	case SpriteRenderComponent::Direction::up:
		m_ProjectileDir = Vector2f{ 0.f, 1.f };
		m_ProjectilePos.x += (player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() / 3);
		m_ProjectilePos.y += (player->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() / 2);
		break;
	}

	m_Enemies = world->GetComponent<EnemyManagerComponent>()->GetEnemiesInCurrentRoom();

	const auto effect = player->GetComponent<InventoryComponent>()->GetActiveEffect();
	if (effect)
	{
		m_pEffect = effect;
	}
}