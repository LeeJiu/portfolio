#pragma once

#include "gameNode.h"
#include "player.h"
#include "gameMap.h"

class gameStudy : public gameNode
{
private:
	gameMap* _map;
	player* _player;

public:
	gameStudy();
	virtual ~gameStudy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};

