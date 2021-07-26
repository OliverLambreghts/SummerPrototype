#pragma once
#include "BaseEffect.h"

class HealEffect final : public BaseEffect
{
public:
	HealEffect(float amount);
	virtual ~HealEffect() override = default;
	HealEffect(const HealEffect& other) = delete;
	HealEffect(HealEffect&& other) = delete;
	void operator=(const HealEffect& rhs) = delete;
	void operator=(const HealEffect&& rhs) = delete;

	virtual void ApplyEffect(std::shared_ptr<GameObject> obj) override;
	virtual float GetAmount() const override;
	virtual float GetDuration() const override;
private:
	float m_Amount;
};