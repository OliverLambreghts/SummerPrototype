#pragma once
#include "Component.h"

class ActivityComponent final : public Component
{
public:
	ActivityComponent();
	virtual ~ActivityComponent() = default;
	ActivityComponent(const ActivityComponent& other) = delete;
	ActivityComponent(ActivityComponent&& other) = delete;
	void operator=(const ActivityComponent& rhs) = delete;
	void operator=(const ActivityComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	bool GetActivity() const;
	void Activate();
	void Deactivate();

	// --- DIT WEGHALEN VOLGENDE KEER ---
	void DebugDeactivate();
	// --- DIT WEGHALEN VOLGENDE KEER ---
private:
	bool m_IsActive, m_IsDebugActive;
};
