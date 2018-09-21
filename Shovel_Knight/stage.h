#pragma once
#include "gameNode.h"
#include "player.h"
#include "stageMap.h"
#include "objectManager.h"
#include "enemyManager.h"
#include "stateUI.h"
#include <typeinfo>

class stage : public gameNode
{
private:
	player* _player;
	stageMap* _map;
	objectManager* _objMgr;
	enemyManager* _enemyMgr;
	stateUI* _ui;

	float _delayTime;
	bool _start;

public:
	stage();
	~stage();

	virtual HRESULT init(int stageNum);
	void release();
	void update();
	void render();
};

