#pragma once
#include <map>
#include <vector>
#include "BaseRenderComponent.h"
#include "MazeComponent.h"

class MazeRenderComponent final : public BaseRenderComponent
{
public:
	MazeRenderComponent(float windowDimension);
	virtual ~MazeRenderComponent() override = default;
	MazeRenderComponent(const MazeRenderComponent& other) = delete;
	MazeRenderComponent(MazeRenderComponent&& other) = delete;
	void operator=(const MazeRenderComponent& rhs) = delete;
	void operator=(const MazeRenderComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;
	virtual void Render() const override;

	std::map<int, Rectf> GetActiveDoors() const;
	float GetWindowDimension() const;
private:
	void RenderCurrentRoom() const;

	Rectf m_Doors[4];
	float m_WindowDimension;
	Room* m_pCurrentRoom;
};
