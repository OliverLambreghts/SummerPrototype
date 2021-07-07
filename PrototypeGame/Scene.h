#pragma once
#include <memory>
#include <vector>
#include <string>
#include "SceneManager.h"
class GameObject;
class Scene final
{
	friend std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name);
public:
	~Scene() = default;
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void Add(const std::shared_ptr<GameObject>& object);

	void Update(float elapsedSec);
	void Render() const;

	void Activate();
	void Deactivate();

	void MoveObjToBack(std::shared_ptr<GameObject> obj);
private:
	explicit Scene(const std::string& name);
	
	std::vector<std::shared_ptr<GameObject>> m_Objects, m_NewObjects;
	std::string m_Name;
	bool m_IsActive;
};