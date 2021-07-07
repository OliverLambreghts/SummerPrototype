#pragma once
#include <functional>
#include "EnemyComponent.h"

class DebugEnemyComponent final : public EnemyComponent
{
public:
	DebugEnemyComponent(int health, int power, int speed, const Point2f& pos, std::shared_ptr<GameObject> player);
	virtual ~DebugEnemyComponent() override = default;
	DebugEnemyComponent(const DebugEnemyComponent& other) = delete;
	DebugEnemyComponent(DebugEnemyComponent&& other) = delete;
	void operator=(const DebugEnemyComponent& rhs) = delete;
	void operator=(const DebugEnemyComponent&& rhs) = delete;

	virtual std::shared_ptr<GameObject> Clone() override;
	virtual void Update(float elapsedSec, GameObject & obj) override;

	std::shared_ptr<GameObject> GetPlayer() const;
private:
	int m_Health, m_Power, m_Speed;
	Point2f m_Position;
	std::shared_ptr<GameObject> m_pPlayer;
};
