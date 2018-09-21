#pragma once
#include "gameNode.h"


class gameOver : public gameNode
{
private:
	image* _fire;
	image* _character;
	int _count, _fireFrameX, _charFrameX;
	bool _start;

public:
	gameOver();
	~gameOver();

	HRESULT init();
	void release();
	void update();
	void render();
};

