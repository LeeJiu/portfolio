#pragma once
#include "enemy.h"


class boxboy : public enemy
{
private:
	float _range;

public:
	boxboy();
	~boxboy();

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();

	void move();
	void attack();
	void damage(int damage);
	void dead();
	void collision();

	void setImage();
	void setFrame();

	void setPlayerMemoryLink(player* player) { _player = player; }
};

