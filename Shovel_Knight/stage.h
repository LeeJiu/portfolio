#pragma once
#include "gameNode.h"
#include "player.h"
#include "stageMap.h"
#include "objectManager.h"
#include "enemyManager.h"

class stage : public gameNode
{
private:
	player* _player;
	stageMap* _map;
	objectManager* _objMgr;
	enemyManager* _enemyMgr;

public:
	stage();
	~stage();

	virtual HRESULT init(int stageNum);
	void release();
	void update();
	void render();
};

