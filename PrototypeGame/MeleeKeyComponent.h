#pragma once
#include <iostream>

#include "BaseProc.h"
#include "ItemComponent.h"
#include <string>
#include "WeaponQuality.h"

class MeleeKeyComponent final : public ItemComponent
{
public:
	MeleeKeyComponent(const std::string& name, int damage, float cooldown, std::shared_ptr<BaseProc> proc,
		WeaponQuality& quality);
	virtual ~MeleeKeyComponent() override = default;
	MeleeKeyComponent(const MeleeKeyComponent& other) = delete;
	MeleeKeyComponent(MeleeKeyComponent&& other) = delete;
	void operator=(const MeleeKeyComponent& rhs) = delete;
	void operator=(const MeleeKeyComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() override;
	virtual void Update(float elapsedSec, GameObject & obj) override;

	void PrintStats() const
	{
		std::cout << "This room contains: " << m_Name << ". It does " << m_Damage << " damage and has a " <<
			m_Cooldown << " second cooldown.\n";
	}
private:
	std::string m_Name;
	int m_Damage;
	float m_Cooldown;
	std::shared_ptr<BaseProc> m_pProc;
	WeaponQuality& m_WeaponQuality;
};
