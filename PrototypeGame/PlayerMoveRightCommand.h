#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"

class PlayerMoveRightCommand final : public Command
{
public:
	PlayerMoveRightCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~PlayerMoveRightCommand() override = default;
	PlayerMoveRightCommand(const PlayerMoveRightCommand& other) = delete;
	PlayerMoveRightCommand(PlayerMoveRightCommand&& other) = delete;
	void operator=(const PlayerMoveRightCommand& rhs) = delete;
	void operator=(const PlayerMoveRightCommand&& rhs) = delete;

	virtual void Execute() override
	{
		auto right = PlayerMovementComponent::DirectionX::right;
		m_Player->GetComponent<PlayerMovementComponent>()->Move(&right, nullptr);
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
