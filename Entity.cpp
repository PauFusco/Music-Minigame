#include "Entity.h"
#include <iostream>
#include <cstdlib>
#include <time.h>


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

//Boss is ALIVE
void Entity::enBoss() {
	is_boss = true;
}
bool Entity::askBoss() {
	return is_boss;
}


void Entity::Move(int dx, int dy)
{
	x += dx * speed;
	y += dy * speed;
}

//Enemy count: 10
int Entity::whichNote()
{
	srand(time(NULL));

	if (noctr < 10) {
		++noctr;
		return rand() % 11;
	}
	else if (noctr == 10) {
		return 50;
	}
	else {
		++noctr;
		return rand() % 11;
	}
	noctr++;

	/*
	IDX:
	 DO = 0
	 RE = 1
	 MI = 2
	 FA = 3
	 SOL = 4
	 LA = 5
	 SI = 6
	 DO = 7
	 RE = 8
	 MI = 9
	 FA = 10
	*/
}

//Enemy spawn timer: 1 sec (every 100 frames)
bool Entity::spawnEnemies()
{
	if (enctr == 100) {
		enctr++;
		return true;
	}
	enctr++;
	return false;
}