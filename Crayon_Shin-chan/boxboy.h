#pragma once
#include "enemy.h"


class boxboy : public enemy
{
private:

public:
	boxboy();
	~boxboy();

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
	void damage(int damage);
	void dead();
	void collision();

	void setImage();
	void setFrame();

	void setObjectMgrMemoryLink(objectManager* objectMgr) { _objectMgr = objectMgr; }
};

