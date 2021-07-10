#pragma once
#include <string>
#include "utils.h"

class WeaponQuality final
{
public:
	WeaponQuality(const std::string& name)
		: m_Name{ name },
		m_TextColor{  },
		m_DamageMultiplier{  },
		m_CooldownMultiplier{  }
	{
		if(name == "basic")
		{
			m_TextColor = Color4f{ 1.f, 1.f, 1.f, 1.f };
			m_DamageMultiplier = 1.f;
			m_CooldownMultiplier = 1.f;
		}
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	const Color4f& GetColor() const
	{
		return m_TextColor;
	}

	float GetDmgMultiplier() const
	{
		return m_DamageMultiplier;
	}

	float GetCDMultiplier() const
	{
		return m_CooldownMultiplier;
	}
private:
	std::string m_Name;
	Color4f m_TextColor;
	float m_DamageMultiplier;
	float m_CooldownMultiplier;
};