#include "pch.h"
#include "StatManager.h"
#include <iostream>
#include <fstream>

StatManager::StatManager()
{
	ParseTotalStats();
	ParseBestStats();
}

void StatManager::ParseTotalStats()
{
	// Parse saved data
	std::ifstream file{ "../Data/Stats/TotalStats.txt" };
	std::string line{};
	std::getline(file, line, ':');
	file >> line;
	m_TotalStats.itemsFound = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_TotalStats.coinsCollected = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_TotalStats.enemiesKilled = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_TotalStats.roomsDiscovered = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_TotalStats.timeSurvived = std::stof(line);
}

void StatManager::ParseBestStats()
{
	// Parse saved data
	std::ifstream file{ "../Data/Stats/BestStats.txt" };
	std::string line{};
	std::getline(file, line, ':');
	file >> line;
	m_BestStats.itemsFound = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_BestStats.coinsCollected = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_BestStats.enemiesKilled = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_BestStats.roomsDiscovered = std::stoi(line);
	std::getline(file, line, ':');
	file >> line;
	m_BestStats.timeSurvived = std::stof(line);
}

Stats& StatManager::GetCurrentStats()
{
	return m_CurrentStats;
}

Stats& StatManager::GetTotalStats()
{
	return m_TotalStats;
}

Stats& StatManager::GetBestStats()
{
	return m_BestStats;
}

void StatManager::PrintCurrentStats() const
{
	std::cout << "--- Stats this run ---\n";
	
	std::cout << "Enemies killed this run: " << m_CurrentStats.enemiesKilled
		<< "\nCoins collected this run: " << m_CurrentStats.coinsCollected
		<< "\nItems found this run: " << m_CurrentStats.itemsFound
		<< "\nRooms discovered this run: " << m_CurrentStats.roomsDiscovered
		<< "\nTime survived this run: " << m_CurrentStats.timeSurvived << " seconds\n";
}

void StatManager::PrintTotalStats() const
{
	std::cout << "--- Total stats ---\n";
	
	std::cout << "Total enemies killed: " << m_TotalStats.enemiesKilled
		<< "\nTotal coins collected: " << m_TotalStats.coinsCollected
		<< "\nTotal items found: " << m_TotalStats.itemsFound
		<< "\nTotal rooms discovered: " << m_TotalStats.roomsDiscovered
		<< "\nTotal time played: " << m_TotalStats.timeSurvived << " seconds\n";
}

void StatManager::PrintBestStats() const
{
	std::cout << "--- Best stats ---\n";
	
	std::cout << "Most enemies killed in one run: " << m_BestStats.enemiesKilled
		<< "\nMost coins collected in one run: " << m_BestStats.coinsCollected
		<< "\nMost items found in one run: " << m_BestStats.itemsFound
		<< "\nMost rooms discovered in one run: " << m_BestStats.roomsDiscovered
		<< "\nLongest time survived in one run: " << m_BestStats.timeSurvived << " seconds\n";
}

void StatManager::ResetCurrentStats()
{
	m_CurrentStats = Stats{};
}
