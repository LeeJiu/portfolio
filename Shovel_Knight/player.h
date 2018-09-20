#pragma once
#include "gameNode.h"

class objectManager;
class enemyManager;


class player : public gameNode
{
private:
	image* _image;
	RECT _rc;
	RECT _coll, _atk;
	int _life, _gold, _sp;
	float _x, _y;
	bool _isRight, _isBounce, _isColl, _isDead;
	bool _bottomLadder, _centerLadder;
	PLAYERSTATE _state;

	float _gravity, _jumpPwr, _climbX, _speed, _deadTime;

	int _curFrameX, _curFrameY, _count, _alpha;

	objectManager* _objMgr;
	enemyManager* _enemyMgr;

public:
	player();
	~player();

	HRESULT init();
	void release();
	void update();
	void render();

	void keyControl();
	void move();
	void bounce();
	void attack();
	
	bool correctY();

	void ladderCenter(float x, float y);
	void ladderCheck();

	void pixelCollision();
	void objCollision();
	void itemCollision();

	void digdownObject(OBJECTTYPE type, int arrNum);
	void digObject(OBJECTTYPE type, int arrNum);

	void damage(int damage);
	void setBounce(bool isBounce, float jumpPwr);

	void setImage();
	void setFrame();

	void setRect();
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setClimbX(float x) { _climbX = x; }
	void setIsColl(bool isColl) { _isColl = isColl; }
	void setGravity() { _gravity = 0.5f; }

	void setObjMemoryLink(objectManager* objMgr) { _objMgr = objMgr; }
	void setEnemyMemoryLink(enemyManager* enemyMgr) { _enemyMgr = enemyMgr; }

	int getLife() { return _life; }
	int getGold() { return _gold; }
	int getSp() { return _sp; }
	float getX() { return _x; }
	float getY() { return _y; }
	bool getIsDead() { return _isDead; }
	bool getIsRight() { return _isRight; }
	RECT getAtkRect() { return _atk; }
	RECT getCollRect() { return _coll; }
	PLAYERSTATE getState() { return _state; }
};

