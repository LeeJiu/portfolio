#pragma once
#include "gameNode.h"

class stageMap;
class objectManager;
class enemyManager;

enum PLAYERSTATE
{
	PS_IDLE, PS_WALK,
	PS_JUMPUP, PS_JUMPDOWN,
	PS_CLIMB, PS_DIG, PS_DIGDOWN
};

class player : public gameNode
{
private:
	image* _image;
	RECT _rc;
	RECT _coll;
	float _x, _y;
	bool _isRight, _isClimb, _isBounce;
	bool _onLadder;
	PLAYERSTATE _state;

	float _gravity, _jumpPwr, _climbX, _bounce;

	int _curFrameX, _curFrameY, _count;

	stageMap* _map;
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
	void tileCollision();
	void objCollision();
	void enemyCollision();

	void block(RECT& rc);
	void checkpoint(OBJECTTYPE type, RECT& rc);
	void chest();
	void awl();
	void diggableObject();
	void movingTile();

	void setImage();
	void setFrame();

	void setMapMemoryLink(stageMap* map) { _map = map; }
	void setObjMemoryLink(objectManager* objMgr) { _objMgr = objMgr; }
	void setEnemyMemoryLink(enemyManager* enemyMgr) { _enemyMgr = enemyMgr; }
};

