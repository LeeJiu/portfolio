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

	void move();
	void attack();
	void damage();
	void dead();
	void collision();
};

