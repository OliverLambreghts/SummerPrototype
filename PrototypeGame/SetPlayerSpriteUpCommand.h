#pragma once
#include "Command.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

class SetPlayerSpriteUpCommand final : public Command
{
public:
	SetPlayerSpriteUpCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~SetPlayerSpriteUpCommand() override = default;
	SetPlayerSpriteUpCommand(const SetPlayerSpriteUpCommand& other) = delete;
	SetPlayerSpriteUpCommand(SetPlayerSpriteUpCommand&& other) = delete;
	void operator=(const SetPlayerSpriteUpCommand& rhs) = delete;
	void operator=(const SetPlayerSpriteUpCommand&& rhs) = delete;

	virtual void Execute() override
	{
		m_Player->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::up);
		m_Player->GetComponent<SpriteRenderComponent>()->Move();
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
