#pragma once
#include <memory>
#include "BaseProc.h"
#include "GameObject.h"

class CritProc final : public BaseProc
{
public:
	CritProc(float chance, std::shared_ptr<GameObject> player);
	virtual ~CritProc() override = default;
	CritProc(const CritProc& other) = delete;
	CritProc(CritProc&& other) = delete;
	void operator=(const CritProc& rhs) = delete;
	void operator=(const CritProc&& rhs) = delete;

	virtual bool IsProcActive() override;
	virtual void Trigger(int& health) override;
	virtual float GetDuration() const override;
	virtual float GetChance() const override;
private:
	float m_Chance, m_Duration;
	std::shared_ptr<GameObject> m_pPlayer;
	int m_Damage;
};
