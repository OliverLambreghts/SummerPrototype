#pragma once
#include "EnemyComponent.h"
#include "GameObject.h"

class Spawner final
{
public:
	explicit Spawner(std::shared_ptr<EnemyComponent> obj)
		: m_Prototype{ std::move(obj) },
		m_HasSpawned{ false }{}

	std::shared_ptr<GameObject> Spawn()
	{
		if (m_HasSpawned)
			return nullptr;

		m_HasSpawned = true;
		return m_Prototype->Clone();
	}
private:
	std::shared_ptr<EnemyComponent> m_Prototype;
	bool m_HasSpawned;
};
