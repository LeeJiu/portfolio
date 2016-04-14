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

	objectManager* _objectMgr;

public:
	gameMap();
	~gameMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void move();	//�� �̵� ����������
	void setObject();

	void setObjectMgrMemoryLink(objectManager* objectMgr) { _objectMgr = objectMgr; }
};

