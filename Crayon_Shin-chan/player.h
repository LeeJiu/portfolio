#pragma once

#pragma warning(disable : 4996)

#include "gameObject.h"
#include "progressBar.h"

class objectManager;

struct tagPlayer
{
	image* player;
	image* shadow;
	STATE state;
	RECT coll;
	POINT pt;
	int curHp, curMp, maxHp, maxMp;
	bool isRight;
};


class player : public gameObject
{
private:
	tagCharacter _player;

	progressBar* _hpBar;
	progressBar* _mpBar;

	objectManager* _enemy;

	bool _isDead;
	bool _test;

public:
	player();
	~player();

	HRESULT init();
	HRESULT init(int x, int y);
	void release();
	void update();
	void render();

	void move();
	void attack();
	void damage(int damage);
	void dead();
	//void skill();		//보류
	void collision();	//인자 : 플레이어의 렉트, 에너미의 렉트

	//엑세스 함수 / 테스트용
	int getX() { return _player.pt.x; }
	int getY() { return _player.pt.y; }
	RECT getRect() { return _player.coll; }
	TYPE getType() { return _player.type; }
	STATE getState() { return _player.state; }

	void setX(int x) { _player.pt.x = x + (_player.coll.right - _player.coll.left) / 2; }
	void setY(int y) { _player.pt.y = y + (_player.coll.bottom - _player.coll.top) / 2; }

	bool isDead() { return _isDead; }
	bool isRight() { return _player.isRight; }

	void setFrame();
	void setImage();

	void setObjectMgrMemoryLink(objectManager* enemy) { _enemy = enemy; }
};

