#pragma once
#include <memory>
#include "Component.h"

class ObstacleComponent final : public Component
{
public:
	ObstacleComponent(bool isBreakable, const std::string& filePath, const Point2f& pos,
		std::shared_ptr<GameObject> player);
	virtual ~ObstacleComponent() override = default;
	ObstacleComponent(const ObstacleComponent& other) = delete;
	ObstacleComponent(ObstacleComponent&& other) = delete;
	void operator=(const ObstacleComponent& rhs) = delete;
	void operator=(const ObstacleComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;
	std::shared_ptr<GameObject> Clone();

	std::shared_ptr<GameObject> GetPlayer() const;
private:
	bool m_IsBreakable;
	std::string m_FileName;
	Point2f m_Position;
	std::shared_ptr<GameObject> m_pPlayer;
};
