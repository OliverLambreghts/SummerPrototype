#include "pch.h"
#include "GridComponent.h"

GridComponent::GridComponent(int rows, int cols, float windowWidth, float windowHeight)
	: m_Rows{ rows },
	m_Cols{ cols },
	m_WindowWidth{ windowWidth },
	m_WindowHeight{ windowHeight },
	m_CellWidth{ windowWidth / static_cast<float>(cols) },
	m_CellHeight{ windowHeight / static_cast<float>(rows) }
{
	// Initialize all the cells inside the grid
	for (int row{}; row < rows; ++row)
	{
		for (int col{}; col < cols; ++col)
		{
			m_Cells.push_back(Rectf{ col * m_CellWidth, row * m_CellHeight, m_CellWidth, m_CellHeight });
		}
	}
}

void GridComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

int GridComponent::GetIndexFromPos(const Point2f& pos) const
{
	const int col = static_cast<int>(pos.x / m_CellWidth);
	const int row = static_cast<int>(pos.y / m_CellHeight);

	const int idx = col + (row * m_Cols);

	return idx;
}

Point2f GridComponent::GetPosFromIdx(int idx) const
{
	return Point2f{ m_Cells[idx].left , m_Cells[idx].bottom };;
}

float GridComponent::GetCellWidth() const
{
	return m_CellWidth;
}

float GridComponent::GetSpaceHeight() const
{
	return m_WindowHeight;
}

std::pair<int, int> GridComponent::GetRowCol(const Point2f& pos) const
{
	int col = static_cast<int>(pos.x / m_CellWidth);
	int row = static_cast<int>(pos.y / m_CellHeight);

	return std::pair<int, int>{row, col};
}

int GridComponent::GetCellAmount() const
{
	return static_cast<int>(m_Cells.size());
}

const std::vector<Rectf>& GridComponent::GetCells() const
{
	return m_Cells;
}