#include "pch.h"
#include "MazeRenderComponent.h"

#include <map>

#include "utils.h"
#include "GameObject.h"

MazeRenderComponent::MazeRenderComponent(float windowDimension)
	: m_WindowDimension{ windowDimension },
	m_pCurrentRoom{ nullptr }
{
	const float doorWidth{ 100.f }, doorDepth{ 25.f };
	m_Doors[0] = Rectf{ 0.f, (m_WindowDimension / 2.f) - doorWidth, doorDepth, doorWidth };
	m_Doors[1] = Rectf{ m_WindowDimension - doorDepth, (m_WindowDimension / 2.f) - doorWidth, doorDepth, doorWidth };
	m_Doors[2] = Rectf{ (m_WindowDimension / 2.f) - doorWidth, m_WindowDimension - doorDepth, doorWidth, doorDepth };
	m_Doors[3] = Rectf{ (m_WindowDimension / 2.f) - doorWidth, 0.f, doorWidth, doorDepth };
}

void MazeRenderComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	if (m_pCurrentRoom == obj.GetComponent<MazeComponent>()->GetCurrentRoom())
		return;

	if (obj.GetComponent<MazeComponent>()->HasFinishedGenerating() &&
		obj.GetComponent<MazeComponent>()->GetCurrentRoom())
	{
		m_pCurrentRoom = obj.GetComponent<MazeComponent>()->GetCurrentRoom();
	}
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
	
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for(auto& door : GetActiveDoors())
	{
		utils::FillRect(door.second);
	}
}
