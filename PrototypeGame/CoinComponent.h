#pragma once
#include "ItemComponent.h"

class CoinComponent final : public ItemComponent
{
public:
	CoinComponent(const std::string& fileName);
	virtual ~CoinComponent() override = default;
	CoinComponent(const CoinComponent& other) = delete;
	CoinComponent(CoinComponent&& other) = delete;
	void operator=(const CoinComponent& rhs) = delete;
	void operator=(const CoinComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() override;
	virtual void Update(float elapsedSec, GameObject & obj) override;
	virtual void OnUse(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> enemy) override;
	virtual void PrintStats() override;
private:
	std::string m_FileName;
};
