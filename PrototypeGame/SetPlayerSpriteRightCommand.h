#pragma once
#include "Command.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

class SetPlayerSpriteRightCommand final : public Command
{
public:
	SetPlayerSpriteRightCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~SetPlayerSpriteRightCommand() override = default;
	SetPlayerSpriteRightCommand(const SetPlayerSpriteRightCommand& other) = delete;
	SetPlayerSpriteRightCommand(SetPlayerSpriteRightCommand&& other) = delete;
	void operator=(const SetPlayerSpriteRightCommand& rhs) = delete;
	void operator=(const SetPlayerSpriteRightCommand&& rhs) = delete;

	virtual void Execute() override
	{
		m_Player->GetComponent<SpriteRenderComponent>()->ChangeRenderDirection(SpriteRenderComponent::Direction::right);
		m_Player->GetComponent<SpriteRenderComponent>()->Move();
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
