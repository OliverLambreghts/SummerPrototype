#pragma once
#include "Command.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

class SetPlayerSpriteDownCommand final : public Command
{
public:
	SetPlayerSpriteDownCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~SetPlayerSpriteDownCommand() override = default;
	SetPlayerSpriteDownCommand(const SetPlayerSpriteDownCommand& other) = delete;
	SetPlayerSpriteDownCommand(SetPlayerSpriteDownCommand&& other) = delete;
	void operator=(const SetPlayerSpriteDownCommand& rhs) = delete;
	void operator=(const SetPlayerSpriteDownCommand&& rhs) = delete;

	virtual void Execute() override
	{
		m_Player->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::down);
		m_Player->GetComponent<SpriteRenderComponent>()->Move();
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
