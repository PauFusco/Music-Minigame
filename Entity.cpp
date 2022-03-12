#include "Entity.h"
#include <iostream>


Entity::Entity()
{
	is_alive = false;
}
Entity::~Entity()
{
}
void Entity::Init(int posx, int posy, int w, int h, int s) 
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
	is_alive = true;
}
void Entity::GetRect(int *posx, int *posy, int *w, int *h)
{
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}
int Entity::GetX()
{
	return x;
}
int Entity::GetY()
{
	return y;
}
int Entity::GetWidth()
{
	return width;
}
void Entity::SetX(int posx)
{
	x = posx;
}
void Entity::ShutDown()
{
	is_alive = false;
}
bool Entity::IsAlive()
{
	return is_alive;
}

void Entity::Move(int dx, int dy)
{
	x += dx * speed;
	y += dy * speed;
}

void Entity::CreateFigure(Entity Enemy, int idx)
{
	int x = 1920, y = 960-104;
	int width = 104, height = 82;
	int speed = 5;

	for (int i = 0; i < 14; i++) {
		if (idx != i) {
			y -= 60;
		}
		else {
			Enemy.Init(x, y, width, height, speed);
		}
	}


	/*
	IDX:
	0 = DO
	1 = RE
	2 = MI
	4 = FA
	5 = SOL
	6 = LA
	7 = SI
	8 = DO
	9 = RE
	10 = MI
	11 = FA
	12 = SOL
	13 = LA
	*/
}

/*
void spawnEnemies()
{
	Entity* enemy;

	if (--enemySpawnTimer <= 0)
	{
		enemy = malloc(sizeof(Entity));
		memset(enemy, 0, sizeof(Entity));
		stage.fighterTail->next = enemy;
		stage.fighterTail = enemy;

		enemy->x = SCREEN_WIDTH;
		enemy->y = rand() % SCREEN_HEIGHT;
		enemy->texture = enemyTexture;
		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

		enemy->dx = -(2 + (rand() % 4));

		enemySpawnTimer = 30 + (rand() % 60);
	}
}
*/