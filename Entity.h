#pragma once
class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int *posx, int *posy, int *w, int *h);
	void SetX(int posx);
	int  GetX();
	int  GetY();
	int  GetWidth();
	void ShutDown(); 
	bool IsAlive();
	
	void enBoss();
	bool askBoss();
	
	void Move(int dx, int dy);
	bool spawnEnemies();
	int  whichNote();
	
	int HPlayer = 30;
	int HBoss = 50;
	int pos = 0;
	int enctr = 0;
	int noctr = 0;
private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;

	bool is_boss = false;
	

};