#pragma once
#include <memory>
#include "Component.h"

class EnemyComponent : public Component
{
public:
	EnemyComponent() = default;
	virtual ~EnemyComponent() override = default;
	EnemyComponent(const EnemyComponent& other) = delete;
	EnemyComponent(EnemyComponent&& other) = delete;
	void operator=(const EnemyComponent& rhs) = delete;
	void operator=(const EnemyComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() = 0;
	virtual void Update(float elapsedSec, GameObject& obj) override = 0;
	virtual std::shared_ptr<GameObject> GetPlayer() const = 0;
	virtual int GetPower() const = 0;
private:
};
