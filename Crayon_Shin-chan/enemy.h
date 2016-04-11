#pragma once
#include "gameNode.h"

class player;

enum ENEMYSTATE
{
	E_IDLE,
	E_RUN,
	E_ATTACK,
	E_DEAD
};

struct tagEnemy
{
	image* enemy;
	image* shadow;
	ENEMYSTATE state;
	RECT coll;
	POINT pt;
	int hp;
	bool isDead;
	bool isRight;
};

class enemy : public gameNode
{
protected:
	tagEnemy _enemy;

	player* _player;

public:
	enemy();
	virtual ~enemy();

	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
	
	virtual void move();
	virtual void attack();
	virtual void damage();
	virtual void dead();
	virtual void collision();
	virtual bool isDead() { return _enemy.isDead; }

	//¿¢¼¼½º ÇÔ¼ö
	virtual int getX() { return _enemy.pt.x; }
	virtual int getY() { return _enemy.pt.y; }

	virtual void setPlayerMemoryLink(player* player) { _player = player; }
};