#pragma once

class Game
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	static float GetWindowDimension();
	static void SetWindowDimension(float dimension);
private:
	// DATA MEMBERS
	const Window m_Window;
	static float m_WindowDimension;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
};