#pragma once
#include "gameNode.h"

class gameMap;

enum PLAYERSTATE
{
	P_IDLE,
	P_RUN,
	P_ATTACK,
	P_SKILL1,
	P_SKILL2,
	P_DAMAGE,
	P_DEAD
};

struct tagPlayer
{
	image* player;
	image* shadow;
	PLAYERSTATE state;
	RECT coll;
	POINT pt;
	int hp, mp;
	bool isRight;
};


class player : public gameNode
{
private:
	tagPlayer _player;
	int _count;
	int _indexIdle, _indexRun, _indexAttack;
	int _curFrameX, _curFrameY;

	RECT test;
	POINT pt;

	gameMap* _gameMap;

public:
	player();
	~player();

	HRESULT init();
	void release();
	void update();
	void render();

	void move();
	void attack();
	void damage();
	void dead();
	//void skill();		//보류
	void collision();	//인자 : 플레이어의 렉트, 에너미의 렉트

	//엑세스 함수 / 테스트용
	int getX() { return _player.pt.x; }
	int getY() { return _player.pt.y; }
	RECT getRect() { return _player.coll; }

	void setX(int x) { _player.pt.x = x + (_player.coll.right - _player.coll.left) / 2; }

	void setFrame();
	void setImage();

	void setEnemyManagerMemoryLink() {  }
	void setMapMemoryLink(gameMap* gameMap) { _gameMap = gameMap; }
};

