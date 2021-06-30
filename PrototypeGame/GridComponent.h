#pragma once
#include <vector>
#include "Component.h"

class GridComponent final : public Component
{
public:
	GridComponent(int rows, int cols, float windowWidth, float windowHeight);
	virtual ~GridComponent() override = default;
	GridComponent(const GridComponent& other) = delete;
	GridComponent(GridComponent&& other) = delete;
	void operator=(const GridComponent& rhs) = delete;
	void operator=(const GridComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	int GetIndexFromPos(const Point2f& pos) const;

	Point2f GetPosFromIdx(int idx) const;

	float GetCellWidth() const;
	float GetSpaceHeight() const;
	std::pair<int, int> GetRowCol(const Point2f& pos) const;

	int GetCellAmount() const;

	const std::vector<Rectf>& GetCells() const;
private:
	int m_Rows, m_Cols;
	std::vector<Rectf> m_Cells;
	float m_WindowWidth, m_WindowHeight, m_CellWidth, m_CellHeight;
};
