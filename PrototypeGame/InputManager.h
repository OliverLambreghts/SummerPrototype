#pragma once
#include <map>
#include <memory>
#include <SDL_events.h>
#include <vector>
#include "Singleton.h"

class GameObject;
class Command;
class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput() const;

	template<typename T>
	void AddCommand(SDL_Keycode kKey, Uint32 stroke, std::shared_ptr<GameObject>& obj)
	{		
		auto cmd = std::make_shared<T>(obj);
		auto pair = std::make_pair(stroke, cmd);
		m_pKeyboardCommands.emplace(std::make_pair(kKey, pair));
	}

	template<typename T>
	void AddCommand(SDL_Keycode kKey, Uint32 stroke, std::shared_ptr<GameObject>& obj1, std::shared_ptr<GameObject>& obj2)
	{
		auto cmd = std::make_shared<T>(obj1, obj2);
		auto pair = std::make_pair(stroke, cmd);
		m_pKeyboardCommands.emplace(std::make_pair(kKey, pair));
	}

	template<typename T>
	void AddWheelCommand(Uint32 wheel, int direction, std::shared_ptr<GameObject>& obj1)
	{
		auto cmd = std::make_shared<T>(obj1);
		auto pair = std::make_pair(direction, cmd);
		m_pWheelCommands.emplace(std::make_pair(wheel, pair));
	}

	InputManager(const InputManager& other) = delete;
	InputManager& operator=(const InputManager& other) = delete;
	InputManager(InputManager&& other) = delete;
	InputManager& operator=(InputManager&& other) = delete;

	void ClearInputs();
protected:
	friend class Singleton<InputManager>;
	InputManager() = default;
	~InputManager() = default;
	
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

	std::multimap<SDL_Keycode, std::pair<Uint32, std::shared_ptr<Command>>> m_pKeyboardCommands;
	std::multimap<Uint32, std::pair<int, std::shared_ptr<Command>>> m_pWheelCommands;
};