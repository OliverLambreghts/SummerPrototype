#include "pch.h"
#include "SceneManager.h"
#include <algorithm>
#include <cassert>
#include "Scene.h"

std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name)
{
	std::shared_ptr<Scene> scene{ new Scene{name} };
	m_Scenes[name] = scene;
	return scene;
}

void SceneManager::Update(float elapsedSec)
{
	if(m_pCurrentActiveScene)
		m_pCurrentActiveScene->Update(elapsedSec);
}

void SceneManager::Render() const
{
	if (m_pCurrentActiveScene)
		m_pCurrentActiveScene->Render();
}

void SceneManager::ActivateScene(const std::string& name)
{
	assert(m_Scenes.find(name) != m_Scenes.end());
	
	if (m_pCurrentActiveScene)
		m_pCurrentActiveScene->Deactivate();
	m_pCurrentActiveScene = m_Scenes[name];
	m_pCurrentActiveScene->Activate();
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene() const
{
	return m_pCurrentActiveScene;
}
