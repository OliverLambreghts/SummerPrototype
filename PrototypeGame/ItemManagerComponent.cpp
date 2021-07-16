#include "pch.h"
#include "ItemManagerComponent.h"
#include <algorithm>
#include "GameObject.h"
#include "ActivityComponent.h"
#include "MazeComponent.h"
#include <fstream>
#include "FireProc.h"
#include "MeleeKeyComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include <sstream>
#include "PlayerMovementComponent.h"
#include "Sprite.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"

ItemManagerComponent::ItemManagerComponent(std::shared_ptr<GameObject> player)
	: m_pPlayer{ player },
	m_pCurrentRoom{ nullptr }
{
}

void ItemManagerComponent::Update(float /*elapsedSec*/, GameObject& obj)
{
	UpdateCurrentRoom(obj);
	AddItems(obj);
	SpawnItem(obj);
}

bool ItemManagerComponent::IsPlayerCloseToItem()
{
	if (m_Items.find(m_pCurrentRoom) == m_Items.end() || !m_Items.at(m_pCurrentRoom))
		return false;
	
	const Rectf player{ m_pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition().x,
	m_pPlayer->GetComponent<PlayerMovementComponent>()->GetPosition().y ,
	m_pPlayer->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	m_pPlayer->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

	const Rectf item{ m_Items[m_pCurrentRoom]->GetComponent<TransformComponent>()->GetPosition().x,
	m_Items[m_pCurrentRoom]->GetComponent<TransformComponent>()->GetPosition().y,
	m_Items[m_pCurrentRoom]->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameWidth(),
	m_Items[m_pCurrentRoom]->GetComponent<SpriteRenderComponent>()->GetSprite().GetFrameHeight() };

	if (utils::IsOverlapping(player, item))
		return true;

	return false;
}

std::shared_ptr<GameObject> ItemManagerComponent::GetItemInCurrentRoom()
{
	if (m_Items.find(m_pCurrentRoom) == m_Items.end())
		return nullptr;

	return m_Items[m_pCurrentRoom];
}

void ItemManagerComponent::RemoveItem()
{
	m_Items.erase(m_pCurrentRoom);
	m_Items[m_pCurrentRoom] = nullptr;
}

void ItemManagerComponent::UpdateCurrentRoom(GameObject& obj)
{
	if (m_pCurrentRoom == obj.GetComponent<MazeComponent>()->GetCurrentRoom())
		return;

	// Despawn item when going to another room
	if (m_Items.find(m_pCurrentRoom) != m_Items.end() && m_Items.at(m_pCurrentRoom))
		m_Items[m_pCurrentRoom]->GetComponent<ActivityComponent>()->Deactivate();

	m_pCurrentRoom = obj.GetComponent<MazeComponent>()->GetCurrentRoom();
}

void ItemManagerComponent::AddItems(GameObject& obj)
{
	if (!obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
		return;

	if (m_pCurrentRoom->type != RoomType::treasure)
		return;

	if (m_Items.find(m_pCurrentRoom) != m_Items.end())
		return;

	const auto type = static_cast<ItemType>(rand() % 3);
	
	switch (type)
	{
	case ItemType::Consumable:
		break;
	case ItemType::MeleeKey:
		SpawnMeleeKey();
		break;
	case ItemType::RangedKey:
		break;
	}
}

void ItemManagerComponent::ParseData(const std::string& line, std::vector<std::string>& data) const
{
	std::stringstream ss{ line };
	std::string name{};
	std::getline(ss, name, ':');
	std::string damage{};
	std::getline(ss, damage, ':');
	std::string cooldown{};
	std::getline(ss, cooldown, ':');
	std::string proc{};
	std::getline(ss, proc, ':');
	std::string quality{};
	std::getline(ss, quality);

	data.push_back(name);
	data.push_back(damage);
	data.push_back(cooldown);
	data.push_back(proc);
	data.push_back(quality);
}

void ItemManagerComponent::SpawnMeleeKey()
{
	std::string line{};
	std::vector<std::string> lines;
	std::vector<std::string> weaponData;
	std::shared_ptr<BaseProc> proc;

	std::ifstream file{ "../Data/ItemData/MeleeList.txt" };

	while (!file.eof())
	{
		std::getline(file, line);
		lines.push_back(line);
	}

	std::random_shuffle(lines.begin(), lines.end());
	const std::string weapon = lines.front();

	ParseData(weapon, weaponData);
	SetProc(weaponData, proc);

	const auto quality = std::make_shared<WeaponQuality>(weaponData[4]);

	auto meleeKey = std::make_shared<MeleeKeyComponent>( weaponData[0],
	std::stoi(weaponData[1]), std::stof(weaponData[2]), proc,
	*quality );

	auto item = meleeKey->Clone();
	item->GetComponent<ActivityComponent>()->Activate();
	SceneManager::GetInstance().GetCurrentScene()->Add(item);
	m_Items[m_pCurrentRoom] = item;
}

void ItemManagerComponent::SetProc(std::vector<std::string>& data, std::shared_ptr<BaseProc>& proc) const
{
	if (data[3] == "fireproc")
		proc = std::make_shared<FireProc>(static_cast<float>(rand() % 15), m_pPlayer);
	else if (data[3] == "none")
		proc = nullptr;
}

void ItemManagerComponent::SpawnItem(GameObject& obj)
{
	if (!obj.GetComponent<MazeComponent>()->HasFinishedGenerating())
		return;

	if (m_Items.find(m_pCurrentRoom) == m_Items.end())
		return;

	if (!m_Items.at(m_pCurrentRoom))
		return;
	
	m_Items[m_pCurrentRoom]->GetComponent<ActivityComponent>()->Activate();
}