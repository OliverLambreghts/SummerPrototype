#pragma once
#include "BaseEffect.h"

class DamageEffect final : public BaseEffect
{
public:
	DamageEffect(float amount, float duration);
	virtual ~DamageEffect() override = default;
	DamageEffect(const DamageEffect& other) = delete;
	DamageEffect(DamageEffect&& other) = delete;
	void operator=(const DamageEffect& rhs) = delete;
	void operator=(const DamageEffect&& rhs) = delete;

	virtual void ApplyEffect(std::shared_ptr<GameObject> obj) override;
	virtual float GetAmount() const override;
	virtual float GetDuration() const override;
private:
	float m_Amount;
	float m_Duration;
};