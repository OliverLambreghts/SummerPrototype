#pragma once
#include <memory>
#include <vector>
class Component;
class GameObject final
{
public:
	GameObject() = default;
	~GameObject() = default;
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	void Update(float elapsedSec);
	void Render() const;

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		for (auto& comp : m_pComponents)
		{
			if (std::dynamic_pointer_cast<T>(comp))
				return std::dynamic_pointer_cast<T>(comp);
		}
		return nullptr;
	}

	void AddComponent(std::shared_ptr<Component> comp);
private:
	// Components
	std::vector<std::shared_ptr<Component>> m_pComponents;
};
