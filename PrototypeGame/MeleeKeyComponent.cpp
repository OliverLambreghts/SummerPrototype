#include "pch.h"
#include "MeleeKeyComponent.h"
#include "ActivityComponent.h"
#include "EnemyMovementComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

MeleeKeyComponent::MeleeKeyComponent(const std::string& name, int damage, float cooldown, std::shared_ptr<BaseProc> proc,
	std::shared_ptr<WeaponQuality> quality,
	const std::string& fileName)
	: m_Name{ name },
	m_Damage{ damage },
	m_Cooldown{ cooldown },
	m_pProc{ proc },
	m_pWeaponQuality{ quality },
	m_FileName{ fileName },
	m_IsCDActive{ false },
	m_CDTimer{}
{
}

std::shared_ptr<GameObject> MeleeKeyComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<MeleeKeyComponent>(m_Name, m_Damage, m_Cooldown, m_pProc, m_pWeaponQuality, m_FileName));
	clone->AddComponent(std::make_shared<TransformComponent>());
	clone->GetComponent<TransformComponent>()->SetPosition(Point2f{ Game::GetWindowDimension() / 2, Game::GetWindowDimension() / 2 });
	// Using test sprite for debugging purposes
	clone->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/" + m_FileName, 1, 1, 1.f / 10.f, 1));
	clone->AddComponent(std::make_shared<ActivityComponent>());
	return clone;
}

void MeleeKeyComponent::Update(float elapsedSec, GameObject& /*obj*/)
{
	if (!m_IsCDActive)
		return;

	// Cooldown
	m_CDTimer += elapsedSec;

	if(m_CDTimer >= m_Cooldown)
	{
		m_CDTimer = 0.f;
		m_IsCDActive = false;
	}
}

void MeleeKeyComponent::OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy)
{
	if (m_IsCDActive)
		return;
	
	std::cout << "Attacking with " << m_pWeaponQuality->GetName() << ' ' << m_Name << '\n';
	if (m_pProc && m_pProc->IsProcActive())
	{
		std::cout << "Proc activates!\n";
		enemy->GetComponent<HealthComponent>()->SetProc(m_pProc);
	}

	// --- DEBUG COMBAT CODE ---
	//enemy->GetComponent<ActivityComponent>()->DebugDeactivate();
	// --- DEBUG COMBAT CODE ---

	const auto damage = static_cast<float>(m_Damage) * m_pWeaponQuality->GetDmgMultiplier();
	std::cout << "Hitting enemy for " << damage << " damage!\n";
	enemy->GetComponent<HealthComponent>()->AddHealth(static_cast<int>(-damage));

	// ACTIVATE COOLDOWN HERE
	m_IsCDActive = true;

	// APPLY KNOCKBACK HERE
	enemy->GetComponent<EnemyMovementComponent>()->ActivateKnockBack(
		player->GetComponent<PlayerMovementComponent>()->GetPosition());
}
