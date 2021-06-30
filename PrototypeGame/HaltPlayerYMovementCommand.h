#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"

class HaltPlayerYMovementCommand final : public Command
{
public:
	HaltPlayerYMovementCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~HaltPlayerYMovementCommand() override = default;
	HaltPlayerYMovementCommand(const HaltPlayerYMovementCommand& other) = delete;
	HaltPlayerYMovementCommand(HaltPlayerYMovementCommand&& other) = delete;
	void operator=(const HaltPlayerYMovementCommand& rhs) = delete;
	void operator=(const HaltPlayerYMovementCommand&& rhs) = delete;

	virtual void Execute() override
	{
		auto idle = PlayerMovementComponent::DirectionY::none;
		m_Player->GetComponent<PlayerMovementComponent>()->Move(nullptr, &idle);
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
