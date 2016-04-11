#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"

enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}

HRESULT enemyManager::init()
{

	return S_OK;
}

void enemyManager::release()
{
}

void enemyManager::update()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		(*_viEnemy)->update();
	}
}

void enemyManager::render()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		if ((*_viEnemy)->isDead()) continue;

		(*_viEnemy)->render();
	}
}

void enemyManager::setEnemy(ENEMYTYPE type, int x, int y)
{
	switch (type)
	{
	case BOXBOY:
		enemy* enemy = new boxboy;
		enemy->init(x, y);
		enemy->setPlayerMemoryLink(_player);	//플레이어의 정보를 던져준다
		_vEnemy.push_back(enemy);
		break;
	/*case CHOCOBEE:
		break;
	case HIP:
		break;
	case BOSS:
		break;
	default:
		break;*/
	}
}