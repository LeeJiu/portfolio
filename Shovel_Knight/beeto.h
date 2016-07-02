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

	HRESULT init(stageMap* map, objectManager* objMgr, int x, int y);
	void release();
	void update();
	void render();

	void move();
	void tileCollision();

	void setImage();
	void setFrame();
};

