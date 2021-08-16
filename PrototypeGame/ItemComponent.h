#pragma once
#include <memory>
#include "Component.h"

class ItemComponent : public Component
{
public:
	ItemComponent() = default;
	virtual ~ItemComponent() override = default;
	ItemComponent(const ItemComponent& other) = delete;
	ItemComponent(ItemComponent&& other) = delete;
	void operator=(const ItemComponent& rhs) = delete;
	void operator=(const ItemComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() = 0;
	virtual void Update(float elapsedSec, GameObject & obj) override = 0;
	virtual void OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy) = 0;
	virtual void PrintStats() = 0;
	virtual int GetPrice() const = 0;
private:
};
