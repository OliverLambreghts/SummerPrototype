#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"
#include "SpriteRenderComponent.h"

class SetPlayerSpriteIdleCommand final : public Command
{
public:
	SetPlayerSpriteIdleCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~SetPlayerSpriteIdleCommand() override = default;
	SetPlayerSpriteIdleCommand(const SetPlayerSpriteIdleCommand& other) = delete;
	SetPlayerSpriteIdleCommand(SetPlayerSpriteIdleCommand&& other) = delete;
	void operator=(const SetPlayerSpriteIdleCommand& rhs) = delete;
	void operator=(const SetPlayerSpriteIdleCommand&& rhs) = delete;

	virtual void Execute() override
	{
		if (m_Player->GetComponent<PlayerMovementComponent>()->GetState() == PlayerMovementComponent::State::idle)
			m_Player->GetComponent<SpriteRenderComponent>()->Idle();
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
