#include "pch.h"
#include "ConsumableComponent.h"
#include <iostream>
#include "ActivityComponent.h"
#include "Game.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

ConsumableComponent::ConsumableComponent(const std::string& name, std::vector<std::shared_ptr<BaseEffect>>& effects,
                                         const std::string& fileName)
	: m_Name{ name },
	m_pEffects{ effects },
	m_FileName{ fileName }
{
}

std::shared_ptr<GameObject> ConsumableComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<ConsumableComponent>(m_Name, m_pEffects, m_FileName));
	clone->AddComponent(std::make_shared<TransformComponent>());
	clone->GetComponent<TransformComponent>()->SetPosition(Point2f{ Game::GetWindowDimension() / 2, Game::GetWindowDimension() / 2 });
	// Using test sprite for debugging purposes
	clone->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/" + m_FileName, 1, 1, 1.f / 10.f, 1));
	clone->AddComponent(std::make_shared<ActivityComponent>());
	return clone;
}

void ConsumableComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

void ConsumableComponent::OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> /*enemy*/)
{
	for(auto& effect : m_pEffects)
	{
		effect->ApplyEffect(player);
		std::cout << "Activating consumable " << m_Name << '\n';
	}
}

void ConsumableComponent::PrintStats()
{
	std::cout << m_Name << '\n';
}
