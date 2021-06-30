#pragma once
#include "Command.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

class SetPlayerSpriteLeftCommand final : public Command
{
public:
	SetPlayerSpriteLeftCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~SetPlayerSpriteLeftCommand() override = default;
	SetPlayerSpriteLeftCommand(const SetPlayerSpriteLeftCommand& other) = delete;
	SetPlayerSpriteLeftCommand(SetPlayerSpriteLeftCommand&& other) = delete;
	void operator=(const SetPlayerSpriteLeftCommand& rhs) = delete;
	void operator=(const SetPlayerSpriteLeftCommand&& rhs) = delete;

	virtual void Execute() override
	{
		m_Player->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::left);
		m_Player->GetComponent<SpriteRenderComponent>()->Move();
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
