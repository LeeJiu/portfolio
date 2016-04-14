#pragma once
#include "gameNode.h"

class objectManager;

class gameMap : public gameNode
{
private:
	image* _map;
	int _moveX;
	int _saveX;
	int _saveIdx;
	int _settingCnt;
	int _enemyCnt;

	objectManager* _objectMgr;

public:
	gameMap();
	~gameMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void checkScroll();
	void findPlayer();
	void move();	//맵 이동 제한해주자
	void setObject();

	void setObjectMgrMemoryLink(objectManager* objectMgr) { _objectMgr = objectMgr; }
};

