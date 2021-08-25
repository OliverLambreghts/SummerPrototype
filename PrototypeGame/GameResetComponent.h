#pragma once
#include <functional>

#include "Component.h"

class GameResetComponent final : public Component
{
public:
	GameResetComponent(std::function<void()> initFcn);
	virtual ~GameResetComponent() override = default;
	GameResetComponent(const GameResetComponent& other) = delete;
	GameResetComponent(GameResetComponent&& other) = delete;
	void operator=(const GameResetComponent& rhs) = delete;
	void operator=(const GameResetComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;
private:
	void Reset() const;
	void UpdateBestStats() const;
	void UpdateTotalStats() const;
	void PrintAllStats() const;
	void ResetEverything() const;
	
	std::function<void()> m_pInitialize;
};
