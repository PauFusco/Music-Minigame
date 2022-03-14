#include "Game.h"

int main(int argc, char* args[])
{
	Game game;
	if (!game.Init()) return -1;
	
	game.Start();
	
	bool end = false;
	while (!end)
	{
		end = game.Update();
		game.Draw();
	}
	game.End();
	game.Release();
	return 0;
}