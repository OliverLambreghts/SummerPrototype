#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PlayerMovementComponent.h"

class HaltPlayerXMovementCommand final : public Command
{
public:
	HaltPlayerXMovementCommand(std::shared_ptr<GameObject> obj)
		: m_Player{ std::move(obj) }
	{

	}

	virtual ~HaltPlayerXMovementCommand() override = default;
	HaltPlayerXMovementCommand(const HaltPlayerXMovementCommand& other) = delete;
	HaltPlayerXMovementCommand(HaltPlayerXMovementCommand&& other) = delete;
	void operator=(const HaltPlayerXMovementCommand& rhs) = delete;
	void operator=(const HaltPlayerXMovementCommand&& rhs) = delete;

	virtual void Execute() override
	{
		auto idle = PlayerMovementComponent::DirectionX::none;
		m_Player->GetComponent<PlayerMovementComponent>()->Move(&idle, nullptr);
	}
private:
	std::shared_ptr<GameObject> m_Player;
};
