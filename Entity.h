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

	bool IsEmp();
	bool SetEmp();
	
	void Move(int dx, int dy);
	bool spawnEnemies();
	int  whichNote();

	int pos = 0;
	int enctr = 0;
	int noctr = 1;
private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;

	bool is_boss = false;
	
	bool is_emp;
};