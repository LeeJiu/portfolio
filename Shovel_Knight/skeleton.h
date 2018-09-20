#pragma once
#include "enemy.h"


class skeleton : public enemy
{
private:
	int _atkRange;

public:
	skeleton();
	~skeleton();

	HRESULT init(int x, int y);
	void release();
	void update();

	void move();
	void bounce();
	void attack();
	void aware();

	void collision();

	void setImage();
	void setFrame();

	void setRect();
};

