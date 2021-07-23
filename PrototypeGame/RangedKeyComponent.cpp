#include "pch.h"
#include "RangedKeyComponent.h"
#include "ActivityComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"
#include "ProjectileComponent.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

RangedKeyComponent::RangedKeyComponent(const std::string& name, int damage, float cooldown, std::shared_ptr<BaseProc> proc, std::shared_ptr<WeaponQuality> quality, const std::string& fileName,
	float rangeTime)
	: m_Name{ name },
	m_Damage{ damage },
	m_Cooldown{ cooldown },
	m_pProc{ proc },
	m_pWeaponQuality{ quality },
	m_FileName{ fileName },
	m_IsCDActive{ false },
	m_CDTimer{},
	m_HasShot{ false },
	m_RangeTime{ rangeTime }
{
}

std::shared_ptr<GameObject> RangedKeyComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<RangedKeyComponent>(m_Name, m_Damage, m_Cooldown, m_pProc, m_pWeaponQuality, m_FileName, m_RangeTime));
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
	// Projectiel ook updaten
	obj.GetComponent<ProjectileComponent>()->Update(elapsedSec, obj);
	
	if (!m_IsCDActive)
		return;

	if (!m_HasShot)
	{
		m_HasShot = true;
		obj.GetComponent<ProjectileComponent>()->Shoot(m_ProjectilePos, m_ProjectileDir, m_RangeTime);
	}

	// Cooldown
	m_CDTimer += elapsedSec;

	if (m_CDTimer >= m_Cooldown)
	{
		m_CDTimer = 0.f;
		m_IsCDActive = false;
		m_HasShot = false;
	}
}

void RangedKeyComponent::OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> /*enemy*/)
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
		break;
	case SpriteRenderComponent::Direction::down:
		m_ProjectileDir = Vector2f{ 0.f, -1.f };
		break;
	case SpriteRenderComponent::Direction::left:
		m_ProjectileDir = Vector2f{ -1.f, 0.f };
		break;
	case SpriteRenderComponent::Direction::up:
		m_ProjectileDir = Vector2f{ 0.f, 1.f };
		break;
	}
}
