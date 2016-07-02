#include "stdafx.h"
#include "stage.h"


stage::stage()
{
}


stage::~stage()
{
}

HRESULT stage::init(int stageNum)
{
	_map = new stageMap;
	_map->init(stageNum);

	_objMgr = new objectManager;
	_objMgr->init(stageNum);

	_enemyMgr = new enemyManager;
	_enemyMgr->init(stageNum);

	_player = new player;
	_player->init();

	_enemyMgr->setMapMemoryLink(_map);
	_enemyMgr->setObjMemoryLink(_objMgr);

	_player->setMapMemoryLink(_map);
	_player->setObjMemoryLink(_objMgr);
	_player->setEnemyMemoryLink(_enemyMgr);

	return S_OK;
}

void stage::release()
{
	_map->release();
	_objMgr->release();
	_enemyMgr->release();
	_player->release();
	SAFE_DELETE(_map);
	SAFE_DELETE(_objMgr);
	SAFE_DELETE(_enemyMgr);
	SAFE_DELETE(_player);
}

void stage::update()
{
	_map->update();
	_objMgr->update();
	_enemyMgr->update();
	_player->update();
}

void stage::render()
{
	_map->render();
	_objMgr->render();
	_enemyMgr->render();
	_player->render();
}
