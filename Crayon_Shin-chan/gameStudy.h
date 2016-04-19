#pragma once

#include "gameNode.h"
#include "gameMap.h"
#include "objectManager.h"

class gameStudy : public gameNode
{
private:
	gameMap* _map;
	objectManager* _objectMgr;

public:
	gameStudy();
	virtual ~gameStudy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};

