#pragma once
class Entity
{
public:
	Entity();
	~Entity();
	
	int enemySpawnTimer = 6;

	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int *posx, int *posy, int *w, int *h);
	void SetX(int posx);
	int  GetX();
	int  GetY();
	int  GetWidth();
	void ShutDown(); 
	bool IsAlive();
	void Move(int dx, int dy);
	void CreateFigure(Entity Enemy, int idx);
	void spawnEnemies();

	int pos = 0;
private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
};