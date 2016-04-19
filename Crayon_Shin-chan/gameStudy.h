#pragma once

#include "gameNode.h"
#include "gameMap.h"
#include "gameScene.h"
#include "objectManager.h"

class gameStudy : public gameNode
{
private:
	gameScene* _scene;
	gameMap* _map;
	objectManager* _objectMgr;

	SCENE _trigger;

public:
	gameStudy();
	virtual ~gameStudy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};

