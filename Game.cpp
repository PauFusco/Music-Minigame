#include "Game.h"
#include <math.h>
#include <cstdlib>


Game::Game() {}
Game::~Game(){}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space, f1: god mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}

	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Load images
	if (!LoadImages())
		return false;
	
	int w;
	SDL_QueryTexture(img_start, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	god_mode = false;
	
	//Start();
	
	//Init variables
	Player.Init(WINDOW_WIDTH >> 3, WINDOW_HEIGHT/(2) - 52, 82, 104, 5);
	idx_shot = 0;
	//Silence.Init(WINDOW_WIDTH >> 3, WINDOW_HEIGHT >> 1, 104, 82, 5);
	

	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4);
	god_mode = false;
	
	//HP Initialization
	for (int i = 0; i < MAX_HP; ++i)
	{
		HP[i].Init(0 + (50 * i), 0, 41, 52, 0);
	}

	return true;
}

bool Game::LoadImages()
{
	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Log("IMG_Init, failed to init required png support: %s\n", IMG_GetError());
		return false;
	}

	img_background = SDL_CreateTextureFromSurface(Renderer, IMG_Load("background.png"));
	if (img_background == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_start = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Inicio.png"));
	if (img_start == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	img_end = SDL_CreateTextureFromSurface(Renderer, IMG_Load("Game over.png"));
	if (img_end == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("player-base.png"));
	if (img_player == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_player2 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("player-base2.png"));
	if (img_player2 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	
	img_HP = SDL_CreateTextureFromSurface(Renderer, IMG_Load("player-base.png"));
	if (img_player == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	
	img_enemy1 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("enemy.png"));
	if (img_enemy1 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());	
		return false;
	}

	img_enemy2 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("enemy2.png"));
	if (img_enemy2 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_enemy3 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("enemy3.png"));
	if (img_enemy3 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_enemy4 = SDL_CreateTextureFromSurface(Renderer, IMG_Load("enemy4.png"));
	if (img_enemy4 == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	
	img_boss = SDL_CreateTextureFromSurface(Renderer, IMG_Load("boss-base.png"));
	if (img_boss == NULL) {
			SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
			return false;
	}
	
	img_shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("bullet.png"));
	if (img_shot == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	
	Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
	mix_oscarmasterpiece = Mix_LoadMUS("gamemusic.wav");
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
	Mix_PlayMusic(mix_oscarmasterpiece, -1);

	return true;
}
void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_shot);
	SDL_DestroyTexture(img_enemy1);
	SDL_DestroyTexture(img_enemy2);
	SDL_DestroyTexture(img_enemy3);
	SDL_DestroyTexture(img_enemy4);
	SDL_DestroyTexture(img_enemyEmp);
	SDL_DestroyTexture(img_boss);
	SDL_DestroyTexture(img_silence);
	
	Mix_FreeMusic(mix_oscarmasterpiece);
	
	Mix_Quit();
	
	IMG_Quit();

	
	SDL_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}

bool Game::Update()
{
	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;
	
	if (Boss.GetX() <= 1280) {
		if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -2;
		if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 2;
		
		if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
		{
		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);

		//offset from player: dx, dy = [(29, 3), (29, 59)]
		Shots[idx_shot].Init(x + 29, y + 24, 56, 20, 10);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
		Shots[idx_shot].Init(x + 29, y + 68, 56, 20, 10);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
		}
		
	}

	bool truth = Enemy[idx_enemy].spawnEnemies();

	if (truth == true && Enemy[62].whichNote() != 50)
	{
		int note = Enemy[63].whichNote();
		Enemy[idx_enemy].Init(1920, 960 - (88 * note), 82, 104, 10);
		++idx_enemy;
		idx_enemy %= MAX_ENEMIES;
	}
	else if (truth == true && Enemy[63].whichNote() == 50)
	{
		Boss.Init(1920, 0, 640, 1080, 1);
		Boss.enBoss();
		Mix_VolumeMusic(MIX_MAX_VOLUME);
	}

	//Move boss until in position
	if (Boss.askBoss() && Boss.GetX() > 1280)
	{
		Boss.Move(-1, 0);
	}
	
	for (int i = 0; i < MAX_ENEMIES; ++i) {
		if (Enemy[i].IsAlive() && !Enemy[i].IsEmp() && Enemy[i].GetX() < 460) {
			Enemy[i].ShutDown();
			Player.HPlayer -= 10;
			if (Player.HPlayer == 0) return true;
		}
	}

	if (Boss.IsAlive())
	{
		for (int i = 0; i < MAX_SHOTS; ++i)
		{
			if (Shots[i].IsAlive() && Shots[i].GetX() > 1280)
			{
				Shots[i].ShutDown();
				Boss.HBoss--;
				if (Boss.HBoss == 0)
				{
					Boss.ShutDown();
					SDL_Delay(500);
					return true;
				}
			}
		}
	}
	
	if (Boss.HBoss == 0)
	{
		Boss.ShutDown();
	}

	//Player update
	Player.Move(fx, fy);

	//Silence update
	Silence.Move(1, 0);
	

	SDL_Rect rc;

	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (Enemy[i].IsAlive())
		{
			Enemy[i].Move(-1, 0);
			if (Enemy[i].GetX() > WINDOW_WIDTH)	Enemy[i].ShutDown();

			for (int j = 0; j < MAX_SHOTS; ++j)
			{
				if (Shots[j].GetY() + 20 > Enemy[i].GetY() && Enemy[i].GetY() + 104 > Shots[i].GetY() && Enemy[i].GetX() < Shots[j].GetX())
				{
					Enemy[i].ShutDown();
					Shots[j].ShutDown();
					Shots[j].Init(460, 0, 0, 0, 0);
				}
			}
		}
	}
	
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(1, 0);
			if (Shots[i].GetX() > WINDOW_WIDTH)	Shots[i].ShutDown();
			

		}
	}
	return false;
}

/*
void Game::Start()
{
	SDL_Rect rc;

	Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_start, NULL, &rc);

	SDL_Delay(3000);
}

void Game::End()
{
	SDL_Rect rc;

	Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_end, NULL, &rc);

	SDL_Delay(3000);
}
*/

void Game::Draw()
{
	SDL_Rect rc;

		//Set the color used for drawing operations
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
		//Clear rendering target
		SDL_RenderClear(Renderer);

		//God mode uses red wireframe rectangles for physical objects
		if (god_mode) SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);

		//Draw scene
		if (!Boss.IsAlive())
		{
			Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_background, NULL, &rc);
		}
		else
		{
			Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_backgroundc, NULL, &rc);
		}
		//Draw player
		if (god_mode) {
			Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_player2, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}
		else {
			Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_player, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}

		//Draw silence
		Silence.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_silence, NULL, &rc);

		if (Boss.askBoss() && Boss.IsAlive()) {
			Boss.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_boss, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}

		//Draw enemy
		for (int i = 0; i < MAX_ENEMIES; ++i)
		{
			if (Enemy[i].IsAlive())
			{
				//render the enemy
				//int aux = rand() % 4;
				//if (aux == 0) {
				Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_enemy1, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
				/* }
				else if (aux == 1) {
					Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_enemy2, NULL, &rc);
					if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
				}
				else if (aux == 2) {
					Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_enemy3, NULL, &rc);
					if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
				}
				else if (aux == 3) {
					Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
					SDL_RenderCopy(Renderer, img_enemy4, NULL, &rc);
					if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
				}*/
			}
		}
		//Draw shots
		for (int i = 0; i < MAX_SHOTS; ++i)
		{
			if (Shots[i].IsAlive())
			{
				Shots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_shot, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
		}

		if (god_mode) {
			Player.HPlayer = 30;
		}

		//Draw HP
		if (Player.HPlayer == 30)
		{
			for (int i = 0; i < MAX_HP; ++i)
			{
				HP[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_player, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}

		}
		else if (Player.HPlayer == 20)
		{
			for (int i = 0; i < MAX_HP - 1; ++i)
			{
				HP[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_player, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
		}
		else if (Player.HPlayer == 10)
		{
			for (int i = 0; i < MAX_HP - 2; ++i)
			{
				HP[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
				SDL_RenderCopy(Renderer, img_player, NULL, &rc);
				if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
			}
		}
		//Update screen
		SDL_RenderPresent(Renderer);

		SDL_Delay(10);	// 1000/10 = 100 fps max
}