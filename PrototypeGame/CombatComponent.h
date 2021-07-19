#pragma once
#include "Component.h"

class CombatComponent final : public Component
{
public:
	CombatComponent();
	virtual ~CombatComponent() override = default;
	CombatComponent(const CombatComponent& other) = delete;
	CombatComponent(CombatComponent&& other) = delete;
	void operator=(const CombatComponent& rhs) = delete;
	void operator=(const CombatComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;
private:
	bool CheckCollision(GameObject& obj) const;
	void Attack(GameObject& obj);

	const float m_Cooldown = 1.f;
	float m_Timer;
	bool m_IsCDActive;
};
