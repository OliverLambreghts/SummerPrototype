#pragma once
#include <iostream>
#include "BaseProc.h"
#include "ItemComponent.h"
#include <string>
#include "WeaponQuality.h"

class RangedKeyComponent final : public ItemComponent
{
public:
	RangedKeyComponent(const std::string& name, int damage, float cooldown, std::shared_ptr<BaseProc> proc,
		std::shared_ptr<WeaponQuality> quality, const std::string& fileName, float rangeTime);
	virtual ~RangedKeyComponent() override = default;
	RangedKeyComponent(const RangedKeyComponent& other) = delete;
	RangedKeyComponent(RangedKeyComponent&& other) = delete;
	void operator=(const RangedKeyComponent& rhs) = delete;
	void operator=(const RangedKeyComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() override;
	virtual void Update(float elapsedSec, GameObject& obj) override;
	virtual void OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy) override;

	virtual void PrintStats() override
	{
		std::cout << m_Name << ". It does " << m_Damage << " damage and has a " <<
			m_Cooldown << " second cooldown.\n";

		if (!m_pProc)
			return;

		std::cout << "It has a " << m_pProc->GetChance() << " chance to proc\n";
	}
private:
	std::string m_Name;
	int m_Damage;
	float m_Cooldown, m_CDTimer, m_RangeTime;
	std::shared_ptr<BaseProc> m_pProc;
	std::shared_ptr<WeaponQuality> m_pWeaponQuality;
	std::string m_FileName;
	bool m_IsCDActive, m_HasShot;
	Point2f m_ProjectilePos;
	Vector2f m_ProjectileDir;
};
