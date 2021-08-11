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
#include "ConsumableComponent.h"
#include "CritProc.h"
#include "DamageEffect.h"
#include "HealEffect.h"
#include "PlayerMovementComponent.h"
#include "RangedKeyComponent.h"
#include "SpeedEffect.h"
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

	// If the room isn't a treasure room, return out. If the room is the starter room, spawn a melee or ranged weapon.
	if (m_pCurrentRoom->type != RoomType::treasure)
	{
		if (m_pCurrentRoom->type != RoomType::starter)
			return;

		if (m_Items.find(m_pCurrentRoom) != m_Items.end())
			return;

		const auto type = static_cast<ItemType>(rand() % 2);

		switch (type)
		{
		case ItemType::MeleeKey:
			SpawnMeleeKey();
			break;
		case ItemType::RangedKey:
			SpawnRangedKey();
			break;
		}
		
		return;
	}

	if (m_Items.find(m_pCurrentRoom) != m_Items.end())
		return;

	const auto type = static_cast<ItemType>(rand() % 3);

	switch (type)
	{
	case ItemType::Consumable:
		SpawnConsumable();
		break;
	case ItemType::MeleeKey:
		SpawnMeleeKey();
		break;
	case ItemType::RangedKey:
		SpawnRangedKey();
		break;
	}
}

void ItemManagerComponent::ParseMeleeData(const std::string& line, std::vector<std::string>& data) const
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
	std::getline(ss, quality, ':');
	std::string fileName{};
	std::getline(ss, fileName);

	data.push_back(name);
	data.push_back(damage);
	data.push_back(cooldown);
	data.push_back(proc);
	data.push_back(quality);
	data.push_back(fileName);
}

void ItemManagerComponent::ParseRangedData(const std::string& line, std::vector<std::string>& data) const
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
	std::getline(ss, quality, ':');
	std::string fileName{};
	std::getline(ss, fileName, ':');
	std::string rangeTime{};
	std::getline(ss, rangeTime, ':');
	std::string projSpeed{};
	std::getline(ss, projSpeed);

	data.push_back(name);
	data.push_back(damage);
	data.push_back(cooldown);
	data.push_back(proc);
	data.push_back(quality);
	data.push_back(fileName);
	data.push_back(rangeTime);
	data.push_back(projSpeed);
}

void ItemManagerComponent::ParseConsumableData(const std::string& line, std::vector<std::string>& data) const
{
	std::stringstream ss{ line };
	std::string name{};
	std::getline(ss, name, ':');

	std::string effectsArray{};
	std::getline(ss, effectsArray, ']');

	std::string fileName{};
	std::getline(ss, fileName);
	fileName.erase(std::remove(fileName.begin(), fileName.end(), ':'), fileName.end());

	data.push_back(name);
	data.push_back(effectsArray);
	data.push_back(fileName);
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

	ParseMeleeData(weapon, weaponData);
	SetProc(weaponData, proc);

	auto quality = std::make_shared<WeaponQuality>(weaponData[4]);

	// Make a copy of a new weapon quality so that each item with that quality uses the same copy (flyweight)
	if (std::find_if(m_Qualities.begin(), m_Qualities.end(), [quality](std::shared_ptr<WeaponQuality> type)
		{
			return type->GetName() == quality->GetName();
		}) == m_Qualities.end())
	{
		m_Qualities.push_back(quality);
	}

		auto meleeKey = std::make_shared<MeleeKeyComponent>(weaponData[0],
			std::stoi(weaponData[1]), std::stof(weaponData[2]), proc,
			quality, weaponData[5]);

		auto item = meleeKey->Clone();
		item->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(item);
		m_Items[m_pCurrentRoom] = item;
}

void ItemManagerComponent::SpawnRangedKey()
{
	std::string line{};
	std::vector<std::string> lines;
	std::vector<std::string> weaponData;
	std::shared_ptr<BaseProc> proc;

	std::ifstream file{ "../Data/ItemData/RangedList.txt" };

	while (!file.eof())
	{
		std::getline(file, line);
		lines.push_back(line);
	}

	std::random_shuffle(lines.begin(), lines.end());
	const std::string weapon = lines.front();

	ParseRangedData(weapon, weaponData);
	SetProc(weaponData, proc);

	auto quality = std::make_shared<WeaponQuality>(weaponData[4]);

	// Make a copy of a new weapon quality so that each item with that quality uses the same copy (flyweight)
	if (std::find_if(m_Qualities.begin(), m_Qualities.end(), [quality](std::shared_ptr<WeaponQuality> type)
		{
			return type->GetName() == quality->GetName();
		}) == m_Qualities.end())
	{
		m_Qualities.push_back(quality);
	}

		auto rangedKey = std::make_shared<RangedKeyComponent>(weaponData[0],
			std::stoi(weaponData[1]), std::stof(weaponData[2]), proc,
			quality, weaponData[5], std::stof(weaponData[6]), std::stof(weaponData[7]));

		auto item = rangedKey->Clone();
		item->GetComponent<ActivityComponent>()->Activate();
		SceneManager::GetInstance().GetCurrentScene()->Add(item);
		m_Items[m_pCurrentRoom] = item;
}

void ItemManagerComponent::SpawnConsumable()
{
	std::string line{};
	std::vector<std::string> lines;
	std::vector<std::string> consumableData;
	std::vector<std::shared_ptr<BaseEffect>> effects;

	std::ifstream file{ "../Data/ItemData/ConsumableList.txt" };

	while (!file.eof())
	{
		std::getline(file, line);
		lines.push_back(line);
	}

	std::random_shuffle(lines.begin(), lines.end());
	const std::string consumable = lines.front();

	ParseConsumableData(consumable, consumableData);

	SetEffect(consumableData, effects);

	auto consumableItem = std::make_shared<ConsumableComponent>(consumableData[0],
		effects, consumableData[2]);

	auto item = consumableItem->Clone();
	item->GetComponent<ActivityComponent>()->Activate();
	SceneManager::GetInstance().GetCurrentScene()->Add(item);
	m_Items[m_pCurrentRoom] = item;
}

void ItemManagerComponent::SetProc(std::vector<std::string>& data, std::shared_ptr<BaseProc>& proc) const
{
	if (data[3] == "fireproc")
		proc = std::make_shared<FireProc>(static_cast<float>(rand() % 10 + 6), m_pPlayer);
	else if (data[3] == "critproc")
		proc = std::make_shared<CritProc>(static_cast<float>(rand() % 10 + 6), m_pPlayer);
	else
		proc = nullptr;
}

void ItemManagerComponent::SetEffect(std::vector<std::string>& data, std::vector<std::shared_ptr<BaseEffect>>& effects) const
{
	std::stringstream ss2{ data[1] };
	std::string effect{};

	while (!ss2.eof())
	{
		std::vector<std::string> currentEffect;

		std::getline(ss2, effect, '(');
		std::getline(ss2, effect, ')');
		std::stringstream ss3{ effect };

		if (effect == "]" || effect.empty())
			break;

		std::string effectName{};
		std::getline(ss3, effectName, ':');
		std::string effectAmount{};
		std::getline(ss3, effectAmount, ':');
		std::string effectDuration{};
		std::getline(ss3, effectDuration);

		currentEffect.push_back(effectName);
		currentEffect.push_back(effectAmount);
		currentEffect.push_back(effectDuration);

		if (effectName == "healeffect")
		{
			effects.push_back(std::make_shared<HealEffect>(std::stof(effectAmount)));
		}
		else if (effectName == "damageeffect")
		{
			effects.push_back(std::make_shared<DamageEffect>(std::stof(effectAmount), std::stof(effectDuration)));
		}
		else if (effectName == "speedeffect")
		{
			effects.push_back(std::make_shared<SpeedEffect>(std::stof(effectAmount), std::stof(effectDuration)));
		}
	}
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