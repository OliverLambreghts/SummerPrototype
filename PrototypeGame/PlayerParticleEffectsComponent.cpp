#include "pch.h"
#include "PlayerParticleEffectsComponent.h"
#include "ActivityComponent.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"
#include "SceneManager.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "Scene.h"

void PlayerParticleEffectsComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	if (!m_pParticleEffect)
		return;
	
	CheckParticleEffect();
	
	if (!m_HasBeenSet)
		return;

	UpdateParticleEffect(obj);
}

void PlayerParticleEffectsComponent::SetParticleEffect(const std::string& filePath, int rows, int cols, float frt, int rowToRender)
{
	m_HasBeenSet = true;
	SceneManager::GetInstance().GetCurrentScene()->RemoveObj(m_pParticleEffect);
	m_pParticleEffect.reset();
	
	m_pParticleEffect = std::make_shared<GameObject>();
	m_pParticleEffect->AddComponent(std::make_shared<SpriteRenderComponent>(filePath, rows, cols, frt, rowToRender));
	m_pParticleEffect->AddComponent(std::make_shared<ActivityComponent>());
	m_pParticleEffect->AddComponent(std::make_shared<TransformComponent>());
	SceneManager::GetInstance().GetCurrentScene()->Add(m_pParticleEffect);
}

void PlayerParticleEffectsComponent::CheckParticleEffect() const
{
	if (m_pParticleEffect->GetComponent<ActivityComponent>()->GetActivity())
	{
		if (m_pParticleEffect->GetComponent<SpriteRenderComponent>()->GetSprite().GetActFrame()
			== m_pParticleEffect->GetComponent<SpriteRenderComponent>()->GetSprite().GetCols() - 1)
		{
			m_pParticleEffect->GetComponent<ActivityComponent>()->Deactivate();
			m_pParticleEffect->GetComponent<SpriteRenderComponent>()->GetSprite().ResetSprite();

			m_pParticleEffect->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::down);
		}
	}
}

void PlayerParticleEffectsComponent::UpdateParticleEffect(GameObject& obj)
{
	auto direction = obj.GetComponent<SpriteRenderComponent>()->GetDirection();
	Point2f renderPos{};

	switch(direction)
	{
	case SpriteRenderComponent::Direction::left:
		renderPos.x = obj.GetComponent<PlayerMovementComponent>()->GetPosition().x - 50.f;
		renderPos.y = obj.GetComponent<PlayerMovementComponent>()->GetPosition().y;
		m_pParticleEffect->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::left);
		break;
	case SpriteRenderComponent::Direction::right:
		renderPos.x = obj.GetComponent<PlayerMovementComponent>()->GetPosition().x + 
			obj.GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth() + 10.f;
		renderPos.y = obj.GetComponent<PlayerMovementComponent>()->GetPosition().y;
		break;
	case SpriteRenderComponent::Direction::down:
		renderPos.y = obj.GetComponent<PlayerMovementComponent>()->GetPosition().y - 50.f;
		renderPos.x = obj.GetComponent<PlayerMovementComponent>()->GetPosition().x;
		break;
	case SpriteRenderComponent::Direction::up:
		renderPos.y = obj.GetComponent<PlayerMovementComponent>()->GetPosition().y + obj.GetComponent<SpriteRenderComponent>()
			->GetSprite().GetFrameHeight() + 10.f;
		renderPos.x = obj.GetComponent<PlayerMovementComponent>()->GetPosition().x;
		break;
	}

	m_pParticleEffect->GetComponent<TransformComponent>()->SetPosition(renderPos);
	m_pParticleEffect->GetComponent<SpriteRenderComponent>()->Move();
	m_pParticleEffect->GetComponent<ActivityComponent>()->Activate();

	//if (direction == SpriteRenderComponent::Direction::left)
		

	m_HasBeenSet = false;
}
