#include "pch.h"
#include "CoinComponent.h"
#include "ActivityComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

CoinComponent::CoinComponent(const std::string& fileName)
	: m_FileName{ fileName }
{
}

std::shared_ptr<GameObject> CoinComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<CoinComponent>(m_FileName));
	clone->AddComponent(std::make_shared<TransformComponent>());
	clone->GetComponent<TransformComponent>()->SetPosition(Point2f{ Game::GetWindowDimension() / 2, Game::GetWindowDimension() / 2 });
	// Using test sprite for debugging purposes
	clone->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/" + m_FileName, 1, 1, 1.f / 10.f, 1));
	clone->AddComponent(std::make_shared<ActivityComponent>());
	return clone;
}

void CoinComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

void CoinComponent::OnUse(std::shared_ptr<GameObject> /*player*/, std::shared_ptr<GameObject> /*enemy*/)
{
}

void CoinComponent::PrintStats()
{
}

int CoinComponent::GetPrice() const
{
	return 0;
}
