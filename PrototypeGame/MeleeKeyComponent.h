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
		std::shared_ptr<WeaponQuality> quality, const std::string& fileName);
	virtual ~MeleeKeyComponent() override = default;
	MeleeKeyComponent(const MeleeKeyComponent& other) = delete;
	MeleeKeyComponent(MeleeKeyComponent&& other) = delete;
	void operator=(const MeleeKeyComponent& rhs) = delete;
	void operator=(const MeleeKeyComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() override;
	virtual void Update(float elapsedSec, GameObject & obj) override;
	virtual void OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy) override;
	virtual int GetPrice() const override;

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
	float m_Cooldown, m_CDTimer;
	std::shared_ptr<BaseProc> m_pProc;
	std::shared_ptr<WeaponQuality> m_pWeaponQuality;
	std::string m_FileName;
	bool m_IsCDActive;

	int m_Price;
};
