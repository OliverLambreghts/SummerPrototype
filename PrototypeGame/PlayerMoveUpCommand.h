#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"

class PlayerMoveUpCommand final : public Command
{
public:
	PlayerMoveUpCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~PlayerMoveUpCommand() override = default;
	PlayerMoveUpCommand(const PlayerMoveUpCommand& other) = delete;
	PlayerMoveUpCommand(PlayerMoveUpCommand&& other) = delete;
	void operator=(const PlayerMoveUpCommand& rhs) = delete;
	void operator=(const PlayerMoveUpCommand&& rhs) = delete;

	virtual void Execute() override
	{
		auto up = PlayerMovementComponent::DirectionY::up;
		m_Player->GetComponent<PlayerMovementComponent>()->Move(nullptr, &up);
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
