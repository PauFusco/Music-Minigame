#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
//#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
//#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib")

#include "Entity.h"

#define WINDOW_WIDTH	1920
#define WINDOW_HEIGHT	1080
#define MAX_KEYS		256
#define MAX_SHOTS		32
#define MAX_ENEMIES		64
#define MAX_HP			3

class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool LoadImages();
	void Release();
	
	bool Input();
	bool Update();
	void Draw();

private:
	SDL_Window *Window;
	SDL_Renderer *Renderer;
	SDL_Texture *img_background, *img_player, * img_enemy, *img_enemyEmp, *img_boss, *img_silence, *img_shot, *img_HP;

	Entity Player, Boss, Shots[MAX_SHOTS], Enemy[MAX_ENEMIES], Scene, Silence, HP[MAX_HP];
	
	int idx_shot;
	int idx_enemy;
	
	bool god_mode;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP	};
	KEY_STATE keys[MAX_KEYS]; 
};
