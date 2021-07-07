#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "BaseRenderComponent.h"
#include "MazeComponent.h"
#include "GetAllRoomsCommand.h"

class MapRenderComponent final : public BaseRenderComponent
{
	friend class GetAllRoomsCommand;
public:
	MapRenderComponent(std::shared_ptr<GetAllRoomsCommand> roomsCmd);
	virtual ~MapRenderComponent() override = default;
	MapRenderComponent(const MapRenderComponent& other) = delete;
	MapRenderComponent(MapRenderComponent&& other) = delete;
	void operator=(const MapRenderComponent& rhs) = delete;
	void operator=(const MapRenderComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject& obj) override;
	virtual void Render() const override;

	void ActivateMap();
	void DeactivateMap();
	bool IsMapActive() const;
private:
	void RenderHalls(std::vector<Room> rooms, size_t idx, const Rectf& room) const;
	void MoveMapToBackOfScene() const;
	
	std::function < const std::vector<Rectf>& ()> m_pGetCells = nullptr;
	std::function<std::vector<Room>()> m_pGetRooms = nullptr;
	std::shared_ptr<GetAllRoomsCommand> m_pGetRoomsCmd = nullptr;

	bool m_IsMapActive = false;
};
