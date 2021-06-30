#include "pch.h"
#include "Scene.h"
#include <algorithm>
#include "GameObject.h"

Scene::Scene(const std::string& name)
	: m_Name{ name },
	m_IsActive{ false }
{

}

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float elapsedSec)
{
	std::for_each(m_Objects.begin(), m_Objects.end(), [elapsedSec](auto e)
		{
			e->Update(elapsedSec);
		});
}

void Scene::Render() const
{
	std::for_each(m_Objects.begin(), m_Objects.end(), [](auto e)
		{
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