#pragma once
#include "gameNode.h"

class gameMap;

enum PLAYERSTATE
{
	IDLE,
	RUN,
	ATTACK,
	SKILL1,
	SKILL2,
	DAMAGE,
	DEAD
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
	//void skill();		//����
	void collision();	//���� : �÷��̾��� ��Ʈ, ���ʹ��� ��Ʈ

	//������ �Լ� / �׽�Ʈ��
	float getX() { return _player.pt.x; }
	float getY() { return _player.pt.y; }
	RECT getRect() { return _player.coll; }

	void setX(float x) { _player.pt.x = x + (_player.coll.right - _player.coll.left) / 2; }

	void setEnemyManagerMemoryLink() {  }
	void setMapMemoryLink(gameMap* gameMap) { _gameMap = gameMap; }
};

