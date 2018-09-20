#pragma once
#include "enemy.h"


class beeto : public enemy
{
private:
	int _distance;		//이동거리
	int _dirX;

public:
	beeto();
	~beeto();

	HRESULT init(int x, int y);
	void release();
	void update();

	void move();
	void bounce();
	void collision();

	void setImage();
	void setFrame();

	void setIsDead(bool isDead);
	void setRect();
};

