#pragma once
#include "BaseEffect.h"

class SpeedEffect final : public BaseEffect
{
public:
	SpeedEffect(float amount, float duration);
	virtual ~SpeedEffect() override = default;
	SpeedEffect(const SpeedEffect& other) = delete;
	SpeedEffect(SpeedEffect&& other) = delete;
	void operator=(const SpeedEffect& rhs) = delete;
	void operator=(const SpeedEffect&& rhs) = delete;

	virtual void ApplyEffect(std::shared_ptr<GameObject> obj) override;
	virtual float GetAmount() const override;
	virtual float GetDuration() const override;
private:
	float m_Amount;
	float m_Duration;
};