#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"

class PlayerMoveDownCommand final : public Command
{
public:
	PlayerMoveDownCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~PlayerMoveDownCommand() override = default;
	PlayerMoveDownCommand(const PlayerMoveDownCommand& other) = delete;
	PlayerMoveDownCommand(PlayerMoveDownCommand&& other) = delete;
	void operator=(const PlayerMoveDownCommand& rhs) = delete;
	void operator=(const PlayerMoveDownCommand&& rhs) = delete;

	virtual void Execute() override
	{
		auto down = PlayerMovementComponent::DirectionY::down;
		m_Player->GetComponent<PlayerMovementComponent>()->Move(nullptr, &down);
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
