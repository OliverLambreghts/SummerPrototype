#pragma once
#include <memory>
#include "BaseProc.h"
#include "Component.h"

class HealthComponent final : public Component
{
public:
	HealthComponent(int health);
	virtual ~HealthComponent() override = default;
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	void operator=(const HealthComponent& rhs) = delete;
	void operator=(const HealthComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	void AddHealth(int health);
	bool IsDead() const;
	void SetProc(std::shared_ptr<BaseProc> proc);
private:
	int m_Health, m_CurrentProcAmount, m_MaxHealth;
	std::shared_ptr<BaseProc> m_pProc = nullptr;
	float m_ProcTimer;
};
