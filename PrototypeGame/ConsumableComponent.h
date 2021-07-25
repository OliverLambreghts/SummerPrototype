#pragma once
#include "ItemComponent.h"

class ConsumableComponent final : public ItemComponent
{
public:
	ConsumableComponent() = default;
	virtual ~ConsumableComponent() override = default;
	ConsumableComponent(const ConsumableComponent& other) = delete;
	ConsumableComponent(ConsumableComponent&& other) = delete;
	void operator=(const ConsumableComponent& rhs) = delete;
	void operator=(const ConsumableComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() override;
	virtual void Update(float elapsedSec, GameObject & obj) override;
	virtual void OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy) override;
	virtual void PrintStats() override;
private:
	
};
