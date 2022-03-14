#include "Game.h"
#include <math.h>


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

	//Init variables
	Player.Init(WINDOW_WIDTH >> 3, WINDOW_HEIGHT/(2) - 52, 82, 104, 5);
	idx_shot = 0;
	//Silence.Init(WINDOW_WIDTH >> 3, WINDOW_HEIGHT >> 1, 104, 82, 5);
	
	int w;
	SDL_QueryTexture(img_background, NULL, NULL, &w, NULL);
	Scene.Init(0, 0, w, WINDOW_HEIGHT, 4);
	god_mode = false;

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
	
	img_player = SDL_CreateTextureFromSurface(Renderer, IMG_Load("player-base.png"));
	if (img_player == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_enemy = SDL_CreateTextureFromSurface(Renderer, IMG_Load("enemy.png"));
	if (img_enemy == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}

	img_enemyEmp = SDL_CreateTextureFromSurface(Renderer, IMG_Load("enemy-hit.png"));
	if (img_enemyEmp == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	
	img_boss = SDL_CreateTextureFromSurface(Renderer, IMG_Load("boss-base.png"));
	if (img_boss == NULL) {
			SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
			return false;
	}

	/*img_silence = SDL_CreateTextureFromSurface(Renderer, IMG_Load("silence.png"));
	if (img_silence == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}*/
	
	img_shot = SDL_CreateTextureFromSurface(Renderer, IMG_Load("bullet.png"));
	if (img_shot == NULL) {
		SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return false;
	}
	return true;
}
void Game::Release()
{
	//Release images
	SDL_DestroyTexture(img_background);
	SDL_DestroyTexture(img_player);
	SDL_DestroyTexture(img_shot);
	SDL_DestroyTexture(img_enemy);
	SDL_DestroyTexture(img_enemyEmp);
	SDL_DestroyTexture(img_boss);
	SDL_DestroyTexture(img_silence);
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
		if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -1;
		if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 1;
	}
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
	

	bool truth = Enemy[idx_enemy].spawnEnemies();
	
	if (truth == true && Enemy[63].whichNote() != 50) {
		int note = Enemy[63].whichNote();
		Enemy[idx_enemy].Init (1920, 960 - (88 * note), 82, 104, 10);
		++idx_enemy;
		idx_enemy %= MAX_ENEMIES;
	}
	else if (truth == true && Enemy[63].whichNote() == 50){
		Boss.Init (1920, 0, 640, 1080, 1);
		Boss.enBoss();
		Boss.HP = 50;
	}

	if (Boss.askBoss() && Boss.GetX() > 1280){
		Boss.Move(-1, 0);
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
			
		}
	}

	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		for (int j = 0; j < 12; ++j) {
			/*if (SDL_HasIntersection(Enemy[j].GetRect(&rc.x, &rc.y, &rc.w, &rc.h), Shots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h)) == true) {
					//Shots[i].ShutDown();
					//Enemy[j].SetEmp();
			}*/
			
			if (Enemy[j].GetX() == Shots[i].GetX()) {
				Shots[i].ShutDown();
				Enemy[j].SetEmp();
			}
		}
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(1, 0);
			if (Shots[i].GetX() > WINDOW_WIDTH)	Shots[i].ShutDown();

		}
		
	}
	return false;
}

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
	Scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_background, NULL, &rc);
	// rc.x += rc.w;
	// SDL_RenderCopy(Renderer, img_background, NULL, &rc);

	//Draw player
	Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_player, NULL, &rc);
	if (god_mode) SDL_RenderDrawRect(Renderer, &rc);

	//Draw silence
	Silence.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, img_silence, NULL, &rc);

	if (Boss.askBoss()) {
		Boss.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, img_boss, NULL, &rc);
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	}

	//Draw enemy Emp doesnt work
	for (int i = 0; i < MAX_ENEMIES; ++i)
	{
		if (Enemy[i].IsAlive() && !Enemy[i].IsEmp())
		{
			//render the enemy
			Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_enemy, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}
		else if (Enemy[i].IsAlive() && Enemy[i].IsEmp())
		{
			Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, img_enemyEmp, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
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

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
	
}