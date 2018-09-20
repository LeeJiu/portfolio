#pragma once
#include "gameNode.h"

class objectManager;
class player;

class enemy : public gameNode
{
protected:
	image* _image;
	RECT _rc, _coll, _aware;
	float _initX, _initY, _x, _y, _jumpPwr;
	bool _isRight, _isDead, _isBounce, _isRightHit, _isAware;
	ENEMYSTATE _state;

	float _gravity, _deadCnt;
	int _curFrameX, _curFrameY, _count, _alpha, _life, _initLife;

	objectManager* _objMgr;
	player* _player;

public:
	enemy();
	~enemy();

	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();

	void pixelCollision();

	virtual void setRect();
	virtual void setIsDead(bool isDead);
	virtual void setBounce(bool isBounce, bool isRightHit, float jumpPwr);

	bool getIsDead() { return _isDead; }
	int getLife() { return _life; }
	float getX() { return _x; }
	float getY() { return _y; }
	RECT getRect() { return _rc; }
	ENEMYSTATE getState() { return _state; }

	void setObjMemoryLink(objectManager* objMgr) { _objMgr = objMgr; }
	void setPlayerMemoryLink(player* player) { _player = player; }
};