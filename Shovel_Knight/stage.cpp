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
	_sourX = 0;
	_sourY = 0;
	_delayTime = 0.f;

	_map = new stageMap;
	_map->init(stageNum);
	
	_objMgr = new objectManager;
	_objMgr->init(stageNum);	

	_player = new player;
	_player->init();

	_enemyMgr = new enemyManager;
	_enemyMgr->setObjMemoryLink(_objMgr);
	_enemyMgr->setPlayerMemoryLink(_player);
	_enemyMgr->init(stageNum);

	_ui = new stateUI;
	_ui->setSp(_player->getSp());
	_ui->setGold(_player->getGold());
	_ui->setMaxLife(_player->getLife());
	_ui->setMaxBoss(_enemyMgr->getBoss()->getLife());
	_ui->init();

	_player->setObjMemoryLink(_objMgr);
	_player->setEnemyMemoryLink(_enemyMgr);

	_map->setObjMemoryLink(_objMgr);

	return S_OK;
}

void stage::release()
{
	_map->release();
	_objMgr->release();
	_enemyMgr->release();
	_player->release();
	_ui->release();
	SAFE_DELETE(_map);
	SAFE_DELETE(_objMgr);
	SAFE_DELETE(_enemyMgr);
	SAFE_DELETE(_player);
	SAFE_DELETE(_ui);
}

void stage::update()
{
	_player->update();
	_enemyMgr->update();

	//map 정보 갱신
	_map->moveMap(_player->getX(), _player->getY());
	_map->update();

	ITEMMANAGER->update();
	_objMgr->update();
	
	//ui 정보 갱신
	_ui->setSp(_player->getSp());
	_ui->setGold(_player->getGold());
	_ui->setLife(_player->getLife());
	_ui->setBoss(_enemyMgr->getBoss()->getLife());
	_ui->update();

	EFFECTMANAGER->update();

	if (_enemyMgr->getBoss()->getIsDead())
	{
		// 딜레이 주자.
		_delayTime += TIMEMANAGER->getElapsedTime();
		if(_delayTime >= 1.f)
			SCENEMANAGER->changeScene("clear");
	}
	else if (_player->getIsDead())
	{
		SCENEMANAGER->changeScene("gameover");
	}

	if (KEYMANAGER->isOnceKeyDown(VK_BACK))
	{
		SCENEMANAGER->changeScene("title");
	}
}

void stage::render()
{
	_map->render();
	_objMgr->render();
	_enemyMgr->render();
	_player->render();
	ITEMMANAGER->render();
	EFFECTMANAGER->render();
	_ui->render();
}
