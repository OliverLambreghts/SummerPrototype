#pragma once
#include "Singleton.h"

struct Stats
{
	int itemsFound{};
	int coinsCollected{};
	int enemiesKilled{};
	int roomsDiscovered{};
	float timeSurvived{};
};

class StatManager final : public Singleton<StatManager>
{
public:
	StatManager(const StatManager& other) = delete;
	StatManager& operator=(const StatManager& other) = delete;
	StatManager(StatManager&& other) = delete;
	StatManager& operator=(StatManager&& other) = delete;

	Stats& GetCurrentStats();
	Stats& GetTotalStats();
	Stats& GetBestStats();

	void PrintCurrentStats() const;
	void PrintTotalStats() const;
	void PrintBestStats() const;
	void ResetCurrentStats();
private:
	friend class Singleton<StatManager>;
	StatManager() = default;
	~StatManager() = default;

	Stats m_CurrentStats, m_BestStats, m_TotalStats;
};
