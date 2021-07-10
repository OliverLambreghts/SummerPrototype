#include "pch.h"
#include "MeleeKeyComponent.h"
#include "ActivityComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

MeleeKeyComponent::MeleeKeyComponent(const std::string& name, int damage, float cooldown, std::shared_ptr<BaseProc> proc, WeaponQuality& quality)
	: m_Name{ name },
	m_Damage{ damage },
	m_Cooldown{ cooldown },
	m_pProc{ proc },
	m_WeaponQuality{ quality }
{
}

std::shared_ptr<GameObject> MeleeKeyComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<MeleeKeyComponent>(m_Name, m_Damage, m_Cooldown, m_pProc, m_WeaponQuality));
	clone->AddComponent(std::make_shared<TransformComponent>());
	clone->GetComponent<TransformComponent>()->SetPosition(Point2f{Game::GetWindowDimension() / 2, Game::GetWindowDimension() / 2});
	// Using test sprite for debugging purposes
	clone->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/TestKey.png", 1, 1, 1.f / 10.f, 1));
	clone->AddComponent(std::make_shared<ActivityComponent>());
	return clone;
}

void MeleeKeyComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}
