#include "pch.h"
#include "Scene.h"
#include <algorithm>
#include <iostream>
#include "ActivityComponent.h"
#include "GameObject.h"

Scene::Scene(const std::string& name)
	: m_Name{ name },
	m_IsActive{ false }
{

}

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	if(!m_IsActive)
		m_Objects.push_back(object);
	else
		m_NewObjects.push_back(object);
}

void Scene::Update(float elapsedSec)
{
	if(!m_NewObjects.empty())
	{
		for(auto& obj : m_NewObjects)
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
	auto it = std::find(m_Objects.begin(), m_Objects.end(), obj);
	m_Objects.erase(it);

	m_Objects.push_back(obj);
}

std::shared_ptr<GameObject> Scene::GetLastObj() const
{
	return m_Objects.back();
}
