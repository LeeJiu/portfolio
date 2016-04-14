#pragma once
#include "gameObject.h"
#include "progressBar.h"

class objectManager;

struct tagEnemy
{
	image* enemy;
	image* shadow;
	STATE state;
	RECT coll;
	POINT pt;
	int curHp, maxHp;
	bool isRight;
};

class enemy : public gameObject
{
protected:
	tagCharacter _enemy;

	progressBar* _hpBar;

	objectManager* _objectMgr;

	float _range;
	int _saveIdx;
	int _playerX;
	int _playerY;

	bool _isDead;
	bool _test;

public:
	enemy();
	virtual ~enemy();

	virtual HRESULT init();
	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();

	//¿¢¼¼½º ÇÔ¼ö
	virtual int getX();
	virtual int getY();
	virtual RECT getRect() { return _enemy.coll; }
	virtual TYPE getType() { return _enemy.type; }
	virtual STATE getState() { return _enemy.state; }
	virtual void setX(int x);
	virtual void setY(int y);
	
	virtual void damage(int damage);
	virtual bool isDead() { return _isDead; }
	
	//virtual void move();
	//virtual void attack();
	//virtual void dead();
	//virtual void collision();

	//virtual void setImage();
	//virtual void setFrame();

	//virtual ENEMYSTATE getState() { return _enemy.state; }

	virtual void setObjectMgrMemoryLink(objectManager* objectMgr) { _objectMgr = objectMgr; }
};