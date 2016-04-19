#pragma once
#include "enemy.h"

struct tagFireball
{
	image* fireball;
	RECT coll;
	STATE state;
	POINT pt;
	bool fire;
	bool isRight;
};


class hip : public enemy
{
private:
	tagFireball _fireball;

	float _range2;
	float _time;
	bool _hit;

public:
	hip();
	~hip();

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();

	int getX() { return _enemy.pt.x; }
	int getY() { return _enemy.pt.y; }
	//void setX(int x) {}
	//void setY(int y) {}

	void move();
	void attack();
	void fire();
	void moveFire();
	void damage(int damage);
	void dead();
	void collision();

	void setImage();
	void setFrame();

	void setObjectMgrMemoryLink(objectManager* objectMgr) { _objectMgr = objectMgr; }
};

