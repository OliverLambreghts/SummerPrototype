#pragma once
#include "Command.h"
#include "GameObject.h"
#include "MapRenderComponent.h"
#include "Scene.h"
#include "SceneManager.h"

class ActivateMapCommand final : public Command
{
public:
	ActivateMapCommand(std::shared_ptr<GameObject> map)
		: m_pMap{ std::move(map) }
	{

	}

	virtual ~ActivateMapCommand() override = default;
	ActivateMapCommand(const ActivateMapCommand& other) = delete;
	ActivateMapCommand(ActivateMapCommand&& other) = delete;
	void operator=(const ActivateMapCommand& rhs) = delete;
	void operator=(const ActivateMapCommand&& rhs) = delete;

	virtual void Execute() override
	{
		SceneManager::GetInstance().GetCurrentScene()->MoveObjToBack(m_pMap);
		
		if (m_pMap->GetComponent<MapRenderComponent>()->IsMapActive())
			m_pMap->GetComponent<MapRenderComponent>()->DeactivateMap();
		else
			m_pMap->GetComponent<MapRenderComponent>()->ActivateMap();
	}
private:
	std::shared_ptr<GameObject> m_pMap;
};
