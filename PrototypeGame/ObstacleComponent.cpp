#include "pch.h"
#include "ObstacleComponent.h"
#include "ActivityComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ObstacleTransformComponent.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "utils.h"

ObstacleComponent::ObstacleComponent(bool isBreakable, const std::string& filePath, const Point2f& pos,
	std::shared_ptr<GameObject> player)
	: m_IsBreakable{ isBreakable },
	m_FileName{ filePath },
	m_Position{ pos },
	m_pPlayer{ player }
{
}

void ObstacleComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

std::shared_ptr<GameObject> ObstacleComponent::Clone()
{
	auto clone = std::make_shared<GameObject>();
	clone->AddComponent(std::make_shared<ObstacleComponent>(m_IsBreakable, m_FileName, m_Position, m_pPlayer));
	clone->AddComponent(std::make_shared<ObstacleTransformComponent>());
	clone->GetComponent<ObstacleTransformComponent>()->SetPosition(m_Position);
	clone->AddComponent(std::make_shared<HealthComponent>(1));
	// Using test sprite for debugging purposes
	clone->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/" + m_FileName, 1, 1, 1.f / 10.f, 1));
	clone->AddComponent(std::make_shared<ActivityComponent>());
	return clone;
}

std::shared_ptr<GameObject> ObstacleComponent::GetPlayer() const
{
	return m_pPlayer;
}

