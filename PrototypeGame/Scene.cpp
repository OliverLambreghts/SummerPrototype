#include "pch.h"
#include "Scene.h"
#include <algorithm>
#include <iostream>
#include "ActivityComponent.h"
#include "GameObject.h"
#include "ItemComponent.h"
#include "MazeRenderComponent.h"
#include "MovementComponent.h"
#include "SpriteRenderComponent.h"

Scene::Scene(const std::string& name)
	: m_Name{ name },
	m_IsActive{ false }
{

}

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	if (!m_IsActive)
		m_Objects.push_back(object);
	else
		m_NewObjects.push_back(object);
}

void Scene::Update(float elapsedSec)
{
	// Check if the next buffer is empty. If not, then current and next buffer are swapped.
	if (!m_NextBuffer.empty())
	{
		m_Objects = m_NextBuffer;
		m_NextBuffer.clear();
	}

	if (!m_NewObjects.empty())
	{
		for (auto& obj : m_NewObjects)
		{
			m_Objects.push_back(obj);
		}
		m_NewObjects.clear();
	}

	std::for_each(m_Objects.begin(), m_Objects.end(), [elapsedSec](auto e)
		{
			if (e->GetComponent<ActivityComponent>() && !e->GetComponent<ActivityComponent>()->GetActivity())
				return;

			e->Update(elapsedSec);
		});

	// Sort all objects based on their y pos
	std::sort(m_Objects.begin(), m_Objects.end(), [](std::shared_ptr<GameObject>& lhs, std::shared_ptr<GameObject>& rhs)
		{
			if (lhs->GetComponent<SpriteRenderComponent>() && !rhs->GetComponent<SpriteRenderComponent>())
				return true;

			if (!lhs->GetComponent<SpriteRenderComponent>())
				return false;
		
			return lhs->GetComponent<MovementComponent>()->GetPosition().y > rhs->GetComponent<MovementComponent>()->GetPosition().y;
		});

	// Put the maze renderer at the beginning of the container so that the ground and wall textures are rendered first
	if (m_Objects[0]->GetComponent<MazeRenderComponent>())
		return;
	
	std::iter_swap(m_Objects.begin(), std::find_if(m_Objects.begin(), m_Objects.end(), [](std::shared_ptr<GameObject> obj)
		{
			return obj->GetComponent<MazeRenderComponent>();
		}));
}

void Scene::Render() const
{
	std::for_each(m_Objects.begin(), m_Objects.end(), [](auto e)
		{
			if (e->GetComponent<ActivityComponent>() && !e->GetComponent<ActivityComponent>()->GetActivity())
				return;

			e->Render();
		});
}

void Scene::Activate()
{
	m_IsActive = true;
}

void Scene::Deactivate()
{
	m_IsActive = false;
}

void Scene::MoveObjToBack(std::shared_ptr<GameObject> obj)
{
	if (std::find(m_Objects.begin(), m_Objects.end(), obj) == m_Objects.end())
		return;

	if(m_NextBuffer.empty())
		m_NextBuffer = m_Objects;

	const auto it = std::find(m_NextBuffer.begin(), m_NextBuffer.end(), obj);
	m_NextBuffer.erase(it);

	m_NextBuffer.push_back(obj);
}

std::shared_ptr<GameObject> Scene::GetLastObj() const
{
	return m_Objects.back();
}

void Scene::RemoveObj(std::shared_ptr<GameObject> obj)
{
	if (std::find(m_Objects.begin(), m_Objects.end(), obj) == m_Objects.end())
		return;

	if (m_NextBuffer.empty())
		m_NextBuffer = m_Objects;

	auto it = std::find(m_NextBuffer.begin(), m_NextBuffer.end(), obj);
	m_NextBuffer.erase(it);
}
