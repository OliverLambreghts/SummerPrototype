#include "pch.h"
#include "Game.h"
#include <iostream>


#include "ActivateMapCommand.h"
#include "DoorCollisionComponent.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "HaltPlayerXMovementCommand.h"
#include "HaltPlayerYMovementCommand.h"
#include "InputManager.h"
#include "MapRenderComponent.h"
#include "MazeComponent.h"
#include "MazeRenderComponent.h"
#include "PlayerMoveDownCommand.h"
#include "PlayerMoveLeftCommand.h"
#include "PlayerMoveRightCommand.h"
#include "PlayerMoveUpCommand.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SetPlayerSpriteDownCommand.h"
#include "SetPlayerSpriteIdleCommand.h"
#include "SetPlayerSpriteLeftCommand.h"
#include "SetPlayerSpriteRightCommand.h"
#include "SetPlayerSpriteUpCommand.h"
#include "SpriteRenderComponent.h"
#include "TransformComponent.h"
#include "vld.h"

Game::Game(const Window& window)
	:m_Window{ window }
{
	Initialize();
};

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	auto testScene = SceneManager::GetInstance().CreateScene("Test");

	auto testPlayer = std::make_shared<GameObject>();
	auto world = std::make_shared<GameObject>();
	auto map = std::make_shared<GameObject>();
	
	// Test Player
	testPlayer->AddComponent(std::make_shared<PlayerMovementComponent>());
	testPlayer->GetComponent<PlayerMovementComponent>()->SetPosition(Point2f{ m_Window.width / 2.f, m_Window.height / 2.f });
	testPlayer->AddComponent(std::make_shared<SpriteRenderComponent>("../Data/Sprites/TestSheet.png", 9, 4, 1.f / 10.f, 1));
	auto updateRoomCmd = std::make_shared<UpdateCurrentRoomCommand>(testPlayer, world);
	auto collisionCmd = std::make_shared<HandleCollisionCommand>(testPlayer, world);
	testPlayer->AddComponent(std::make_shared<DoorCollisionComponent>(updateRoomCmd, collisionCmd));
	testScene->Add(testPlayer);
	
	InputManager::GetInstance().AddCommand<PlayerMoveRightCommand>(SDLK_RIGHT, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteRightCommand>(SDLK_RIGHT, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<HaltPlayerXMovementCommand>(SDLK_RIGHT, SDL_KEYUP, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteIdleCommand>(SDLK_RIGHT, SDL_KEYUP, testPlayer);

	InputManager::GetInstance().AddCommand<PlayerMoveLeftCommand>(SDLK_LEFT, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteLeftCommand>(SDLK_LEFT, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<HaltPlayerXMovementCommand>(SDLK_LEFT, SDL_KEYUP, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteIdleCommand>(SDLK_LEFT, SDL_KEYUP, testPlayer);

	InputManager::GetInstance().AddCommand<PlayerMoveUpCommand>(SDLK_UP, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteUpCommand>(SDLK_UP, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<HaltPlayerYMovementCommand>(SDLK_UP, SDL_KEYUP, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteIdleCommand>(SDLK_UP, SDL_KEYUP, testPlayer);

	InputManager::GetInstance().AddCommand<PlayerMoveDownCommand>(SDLK_DOWN, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteDownCommand>(SDLK_DOWN, SDL_KEYDOWN, testPlayer);
	InputManager::GetInstance().AddCommand<HaltPlayerYMovementCommand>(SDLK_DOWN, SDL_KEYUP, testPlayer);
	InputManager::GetInstance().AddCommand<SetPlayerSpriteIdleCommand>(SDLK_DOWN, SDL_KEYUP, testPlayer);

	// World
	world->AddComponent(std::make_shared<MazeComponent>());
	world->AddComponent(std::make_shared<MazeRenderComponent>(m_Window.height));
	testScene->Add(world);

	// Map
	auto getRoomsCmd = std::make_shared<GetAllRoomsCommand>(map, world);
	map->AddComponent(std::make_shared<GridComponent>(MazeComponent::GetWorldSize(), 
		MazeComponent::GetWorldSize(), m_Window.width, m_Window.height));
	map->AddComponent(std::make_shared<MapRenderComponent>(getRoomsCmd));
	testScene->Add(map);

	InputManager::GetInstance().AddCommand<ActivateMapCommand>(SDLK_m, SDL_KEYUP, map);
	
	SceneManager::GetInstance().ActivateScene("Test");
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
	SceneManager::GetInstance().Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );
	SceneManager::GetInstance().Render();
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
