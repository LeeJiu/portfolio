#pragma once
#include "gameNode.h"

enum ENEMYSTATE
{
	IDLE,
	RUN,
	ATTACK,
	DEAD
};

struct tagEnemy
{
	image* enemy;
	image* shadow;
	ENEMYSTATE state;
	RECT coll;
	POINT pt;
	int hp;
	bool isRight;
};

class enemy : public gameNode
{
protected:
	tagEnemy _enemy;

public:
	/*virtual*/ enemy();
	/*virtual*/ ~enemy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	
	virtual void move();
	virtual void attack();
	virtual void damage();
	virtual void dead();
	virtual void collision();	//인자 : 플레이어의 렉트
};