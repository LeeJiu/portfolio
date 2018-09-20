#pragma once
#include "gameNode.h"

class player;

class bubbleShot : public gameNode
{
private:
	image* _image;
	RECT _rc;
	float _initX, _initY, _x, _y, _range, _speed, _delay;
	int _curFrameX, _count;
	bool _isRender;

	player* _player;

public:
	bubbleShot();
	~bubbleShot();

	HRESULT init(player * player, float x, float y);
	void release();
	void update();
	void render();

	void collision();

	void setRect();
	void setFrame();

	void setRender(bool render, float time = 0.f);
	bool getRender() { return _isRender; }
};

