#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"

class PlayerMoveLeftCommand final : public Command
{
public:
	PlayerMoveLeftCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~PlayerMoveLeftCommand() override = default;
	PlayerMoveLeftCommand(const PlayerMoveLeftCommand& other) = delete;
	PlayerMoveLeftCommand(PlayerMoveLeftCommand&& other) = delete;
	void operator=(const PlayerMoveLeftCommand& rhs) = delete;
	void operator=(const PlayerMoveLeftCommand&& rhs) = delete;

	virtual void Execute() override
	{
		auto left = PlayerMovementComponent::DirectionX::left;
		m_Player->GetComponent<PlayerMovementComponent>()->Move(&left, nullptr);
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
