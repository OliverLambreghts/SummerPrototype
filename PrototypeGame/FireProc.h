#pragma once
#include "BaseProc.h"
#include "GameObject.h"

class FireProc final : public BaseProc
{
public:
	FireProc(float chance, std::shared_ptr<GameObject> player);
	virtual ~FireProc() override = default;
	FireProc(const FireProc& other) = delete;
	FireProc(FireProc&& other) = delete;
	void operator=(const FireProc& rhs) = delete;
	void operator=(const FireProc&& rhs) = delete;

	virtual bool IsProcActive() override;
	virtual void Trigger() override;
private:
	float m_Chance;
	std::shared_ptr<GameObject> m_pPlayer;
};
