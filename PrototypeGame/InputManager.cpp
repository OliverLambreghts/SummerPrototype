#include "pch.h"
#include "InputManager.h"

#include <iostream>

#include "Command.h"

bool InputManager::ProcessInput() const
{
	SDL_Event e{};

	// Poll next event from queue in event loop
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
			return true;
		
		// Handle the polled event
		for (auto& pair : m_pKeyboardCommands)
		{
			if (e.type == pair.second.first)
			{
				if (e.key.keysym.sym == pair.first)
				{
					pair.second.second->Execute();
				}
			}
		}

		// Handle wheel events
		for(auto& pair : m_pWheelCommands)
		{
			if(e.type == pair.first)
			{
				if(pair.second.first > 0 && e.wheel.y > 0)
				{
					pair.second.second->Execute();
				}
				else if(pair.second.first < 0 && e.wheel.y < 0)
				{
					pair.second.second->Execute();
				}
			}
		}
	}
	return false;
}

void InputManager::ProcessKeyDownEvent(const SDL_KeyboardEvent& /*e*/)
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void InputManager::ProcessKeyUpEvent(const SDL_KeyboardEvent& /*e*/)
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void InputManager::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& /*e*/)
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void InputManager::ProcessMouseDownEvent(const SDL_MouseButtonEvent& /*e*/)
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void InputManager::ProcessMouseUpEvent(const SDL_MouseButtonEvent& /*e*/)
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}