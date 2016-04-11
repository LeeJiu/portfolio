#pragma once

#include "gameNode.h"
#include "gameMap.h"
#include "player.h"
#include "enemyManager.h"

class gameStudy : public gameNode
{
private:
	gameMap* _map;
	player* _player;
	enemyManager* _enemyMgr;

public:
	gameStudy();
	virtual ~gameStudy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};

