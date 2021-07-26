#pragma once
#include <memory>
#include "GameObject.h"

class BaseEffect
{
public:
	BaseEffect() = default;
	virtual ~BaseEffect() = default;
	BaseEffect(const BaseEffect& other) = delete;
	BaseEffect(BaseEffect&& other) = delete;
	void operator=(const BaseEffect& rhs) = delete;
	void operator=(const BaseEffect&& rhs) = delete;

	virtual void ApplyEffect(std::shared_ptr<GameObject> obj) = 0;
	virtual float GetAmount() const = 0;
	virtual float GetDuration() const = 0;
private:
	
};
