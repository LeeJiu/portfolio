#pragma once
#include "gameNode.h"

class objectManager;

enum STATE
{
	IDLE,
	RUN,
	ATTACK,
	SKILL1,
	SKILL2,
	DAMAGE,
	DEAD,
	NONE
};

enum TYPE
{
	PLAYER,
	BOXBOY,
	CHOCOBEE,
	HIP,
	BOSS
};

struct tagCharacter
{
	image* charater;
	image* shadow;
	STATE state;
	TYPE type;
	RECT coll;
	POINT pt;
	int curHp, curMp, maxHp, maxMp;
	bool isRight;
};

class gameObject : public gameNode
{
protected:
	int _count;
	int _curFrameX, _curFrameY;

	objectManager* _objectMgr;

public:
	gameObject();
	virtual ~gameObject();

	virtual HRESULT init() = 0;
	virtual HRESULT init(int x, int y) = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	//¿¢¼¼½º ÇÔ¼ö
	virtual int getX() = 0;
	virtual int getY() = 0;
	virtual RECT getRect() = 0;
	virtual TYPE getType() = 0;
	virtual STATE getState() = 0;
	virtual void setX(int x) = 0;
	virtual void setY(int y) = 0;

	virtual void damage(int damage) = 0;
	virtual bool isDead() = 0;

	virtual void setObjectMgrMemoryLink(objectManager* objectMgr) { _objectMgr = objectMgr; }
};

