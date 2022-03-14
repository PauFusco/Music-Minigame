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

//not tried
bool Entity::IsEmp()
{
	return is_emp;
}
bool Entity::SetEmp()
{
	return true;
}


void Entity::Move(int dx, int dy)
{
	x += dx * speed;
	y += dy * speed;
}

//it works now
int Entity::whichNote()
{
	
	if (noctr == 1) {
		++noctr;
		return 2;
	}
	else if (noctr == 2) {
		++noctr;
		return 5;
	}
	else if (noctr == 3) {
		++noctr;
		return 3;
	}
	++noctr;
	
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
	 SOL = 11
	 LA = 12
	*/
}

//works
bool Entity::spawnEnemies()
{
	if (enctr == 100) {
		enctr++;
		return true;
	}
	enctr++;
	return false;
}