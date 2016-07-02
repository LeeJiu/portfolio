#pragma once
#include "gameNode.h"

class stageMap;
class objectManager;

enum ENEMYSTATE
{
	ES_IDLE, ES_WALK, ES_ATTACK, ES_DEAD
};

class enemy : public gameNode
{
protected:
	image* _image;
	RECT _rc;
	float _x, _y;
	bool _isDead;
	ENEMYSTATE _state;

	float _gravity;
	int _curFrameX, _curFrameY, _count;

	stageMap* _map;
	objectManager* _objMgr;

public:
	enemy();
	~enemy();

	virtual HRESULT init(stageMap* map, objectManager* objMgr, int x, int y);
	void release();
	void update();
	void render();

	void setIsDead(bool isDead) { _isDead = isDead; }

	bool getIsDead() { return _isDead; }
	float getX() { return _x; }
	float getY() { return _y; }
	RECT getRect() { return _rc; }
};

