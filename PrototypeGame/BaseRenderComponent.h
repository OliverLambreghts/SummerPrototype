#pragma once
#include "Component.h"

class BaseRenderComponent : public Component
{
public:
	BaseRenderComponent() = default;
	virtual ~BaseRenderComponent() = default;
	BaseRenderComponent(const BaseRenderComponent& other) = delete;
	BaseRenderComponent(BaseRenderComponent&& other) = delete;
	void operator=(const BaseRenderComponent& rhs) = delete;
	void operator=(const BaseRenderComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject& obj) = 0;
	virtual void Render() const = 0;
};
