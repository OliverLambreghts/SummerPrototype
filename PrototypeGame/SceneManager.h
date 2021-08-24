#pragma once
#include <functional>
#include "Singleton.h"
#include <string>
#include <map>
#include <memory>
class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	std::shared_ptr<Scene> CreateScene(const std::string& name);

	void Update(float elapsedSec);
	void Render() const;

	void ActivateScene(const std::string& name);

	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator=(const SceneManager& other) = delete;
	SceneManager(SceneManager&& other) = delete;
	SceneManager& operator=(SceneManager&& other) = delete;

	std::shared_ptr<Scene> GetCurrentScene() const;
	void Reset(std::function<void()> initFcn);
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	~SceneManager() = default;
	bool CanResetGame();
	
	std::map<std::string, std::shared_ptr<Scene>> m_Scenes;
	std::shared_ptr<Scene> m_pCurrentActiveScene;
	std::function<void()> m_pInit = nullptr;
};