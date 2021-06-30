#pragma once
#include "Command.h"
#include "GameObject.h"

class GetAllRoomsCommand final : public Command
{
public:
	GetAllRoomsCommand(std::shared_ptr<GameObject> map, std::shared_ptr<GameObject> world);
	virtual ~GetAllRoomsCommand() override = default;
	GetAllRoomsCommand(const GetAllRoomsCommand& other) = delete;
	GetAllRoomsCommand(GetAllRoomsCommand&& other) = delete;
	void operator=(const GetAllRoomsCommand& rhs) = delete;
	void operator=(const GetAllRoomsCommand&& rhs) = delete;
	
	virtual void Execute() override;
private:
	std::weak_ptr<GameObject> m_pMap, m_pWorld;
};
