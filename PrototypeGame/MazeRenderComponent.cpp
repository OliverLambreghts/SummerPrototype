#include "pch.h"
#include "MazeRenderComponent.h"
#include <map>


#include "EnemyManagerComponent.h"
#include "Game.h"
#include "utils.h"
#include "GameObject.h"

MazeRenderComponent::MazeRenderComponent(float windowDimension)
	: m_WindowDimension{ windowDimension },
	m_pCurrentRoom{ nullptr },
	m_FloorTexture{ "../Data/Sprites/Floor.png" },
	m_WorldSize{},
	m_WallTexture{ "../Data/Sprites/Wall.png" },
	m_OpenDoorTexture{ "../Data/Sprites/OpenDoor.png" },
	m_ClosedDoorTexture{ "../Data/Sprites/ClosedDoor.png" },
	m_AreAllEnemiesDead{ false }
{
	const float doorWidth{ 100.f }, doorDepth{ 25.f };
	m_Doors[0] = Rectf{ 0.f, (m_WindowDimension / 2.f) - doorWidth, doorDepth, doorWidth };
	m_Doors[1] = Rectf{ m_WindowDimension - doorDepth, (m_WindowDimension / 2.f) - doorWidth, doorDepth, doorWidth };
	m_Doors[2] = Rectf{ (m_WindowDimension / 2.f) - doorWidth, m_WindowDimension - doorDepth, doorWidth, doorDepth };
	m_Doors[3] = Rectf{ (m_WindowDimension / 2.f) - doorWidth, 0.f, doorWidth, doorDepth };
}

void MazeRenderComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	if (obj.GetComponent<EnemyManagerComponent>()->GetEnemiesInCurrentRoom().empty())
		m_AreAllEnemiesDead = true;
	else
		m_AreAllEnemiesDead = false;

	if (m_pCurrentRoom == obj.GetComponent<MazeComponent>()->GetCurrentRoom())
		return;

	if (obj.GetComponent<MazeComponent>()->HasFinishedGenerating() &&
		obj.GetComponent<MazeComponent>()->GetCurrentRoom())
	{
		m_pCurrentRoom = obj.GetComponent<MazeComponent>()->GetCurrentRoom();
	}

	if (m_WorldSize == 0)
		m_WorldSize = obj.GetComponent<MazeComponent>()->GetWorldSize();
}

void MazeRenderComponent::Render() const
{
	RenderCurrentRoom();
}

std::map<int, Rectf> MazeRenderComponent::GetActiveDoors() const
{
	std::map<int, Rectf> doors;

	if (!m_pCurrentRoom)
		return doors;

	if (m_pCurrentRoom->left.isConnected)
	{
		doors[0] = (m_Doors[0]);
	}
	if (m_pCurrentRoom->right.isConnected)
	{
		doors[1] = (m_Doors[1]);
	}
	if (m_pCurrentRoom->up.isConnected)
	{
		doors[2] = (m_Doors[2]);
	}
	if (m_pCurrentRoom->down.isConnected)
	{
		doors[3] = (m_Doors[3]);
	}

	return doors;
}

float MazeRenderComponent::GetWindowDimension() const
{
	return m_WindowDimension;
}

void MazeRenderComponent::RenderCurrentRoom() const
{
	if (!m_pCurrentRoom)
		return;

	// --- Render the floor ---
	Point2f renderPos{ 0.f, 0.f };
	for (int i{}; i < m_WorldSize; ++i)
	{
		for (int j{}; j < m_WorldSize; ++j)
		{
			m_FloorTexture.Draw(Rectf{ renderPos.x, renderPos.y,
				Game::GetWindowDimension() / (m_WorldSize - 3.5f), Game::GetWindowDimension() / (m_WorldSize - 3.5f) });

			renderPos.x += m_FloorTexture.GetWidth();
		}
		renderPos.y += m_FloorTexture.GetHeight();
		renderPos.x = 0.f;
	}

	// --- Render walls ---
	Point2f wallPos{ 0.f, Game::GetWindowDimension() - m_WallTexture.GetHeight() + 1.f };
	// Top wall
	for (int i{}; i < m_WorldSize + 5; ++i)
	{
		m_WallTexture.Draw(Rectf{ wallPos.x, wallPos.y,
			Game::GetWindowDimension() / m_WorldSize, Game::GetWindowDimension() / m_WorldSize });
		wallPos.x += m_WallTexture.GetWidth();
	}
	// Bottom wall
	wallPos = Point2f{ 0.f, -60.f };
	for (int i{}; i < m_WorldSize + 5; ++i)
	{
		m_WallTexture.Draw(Rectf{ wallPos.x, wallPos.y,
			Game::GetWindowDimension() / m_WorldSize, Game::GetWindowDimension() / m_WorldSize });
		wallPos.x += m_WallTexture.GetWidth();
	}
	// Left wall
	wallPos = Point2f{ 0.f, 0.f };
	for (int i{}; i < m_WorldSize + 5; ++i)
	{
		glPushMatrix();
		glTranslatef(wallPos.x + 20.f, wallPos.y, 0.f);
		glRotatef(90.f, 0.f, 0.f, 1.f);

		m_WallTexture.Draw(Rectf{ 0.f, 0.f,
			Game::GetWindowDimension() / m_WorldSize, Game::GetWindowDimension() / m_WorldSize });
		wallPos.y += m_WallTexture.GetHeight();
		glPopMatrix();
	}
	// Right wall
	wallPos = Point2f{ Game::GetWindowDimension() - m_WallTexture.GetWidth(), 0.f };
	for (int i{}; i < m_WorldSize + 5; ++i)
	{
		glPushMatrix();
		glTranslatef(wallPos.x + 30.f, wallPos.y, 0.f);
		glRotatef(-90.f, 0.f, 0.f, 1.f);

		m_WallTexture.Draw(Rectf{ 0.f, 0.f,
			Game::GetWindowDimension() / m_WorldSize, Game::GetWindowDimension() / m_WorldSize });
		wallPos.y += m_WallTexture.GetHeight();
		glPopMatrix();
	}

	// Render doors
	//utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for (auto& door : GetActiveDoors())
	{
		//utils::FillRect(door.second);
		// If there are still enemies in this room, then render the closed doors, otherwise render the open doors

		glPushMatrix();

		// Als het de rechtse deur is
		if (door.second.left > Game::GetWindowDimension() / 1.5f)
		{
			glTranslatef(door.second.left, door.second.bottom + 100.f, 0.f);
			glRotatef(-90.f, 0.f, 0.f, 1.f);
		}
		else if (door.second.left <= 20.f)
		{
			glTranslatef(door.second.left + 25.f, door.second.bottom, 0.f);
			glRotatef(90.f, 0.f, 0.f, 1.f);
		}
		else if (door.second.bottom > Game::GetWindowDimension() - 100.f)
		{
			glTranslatef(door.second.left, door.second.bottom - 25.f, 0.f);
		}
		else if (door.second.bottom < 100.f)
		{
			glTranslatef(door.second.left + 100.f, door.second.bottom + 10.f, 0.f);
			glRotatef(180.f, 0.f, 0.f, 1.f);
		}

		glScalef(2.f, 1.f, 0.f);
		if (m_AreAllEnemiesDead)
			m_OpenDoorTexture.Draw();
		else
			m_ClosedDoorTexture.Draw();
		glPopMatrix();
	}
}
