#include "stdafx.h"
#include "enemyManager.h"
#include "stageMap.h"


enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}

HRESULT enemyManager::init(int stageNum)
{
	_stageNum = stageNum;

	return S_OK;
}

void enemyManager::release()
{
}

void enemyManager::update()
{
	if (!_load)
	{
		loadEnemy();
		_load = true;
	}

	if (_vEnemy.empty()) return;

	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		if ((*_viEnemy)->getIsDead()) continue;

		(*_viEnemy)->update();
	}
}

void enemyManager::render()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
	{
		if ((*_viEnemy)->getX() < _sourX || (*_viEnemy)->getX() > _sourX + WINSIZEX) continue;
		if ((*_viEnemy)->getY() < _sourY || (*_viEnemy)->getY() > _sourY + WINSIZEY) continue;

		(*_viEnemy)->render();
	}
}

void enemyManager::loadEnemy()
{
	char str[128], temp[150000];

	//에너미 데이터 불러오기
	sprintf_s(str, "Stage%d_enemy.txt", _stageNum);
	ifstream loadEnemy(str);

	int dataOrder = 0;		//데이터 순서(개수)
	tagEnemy enemy;
	while (!loadEnemy.eof())
	{
		loadEnemy.getline(temp, 100);
		if (dataOrder % 3 == 0)			//0번째 데이터 type
		{
			enemy.type = (ENEMYTYPE)atoi(temp);
		}
		else if (dataOrder % 3 == 1)	//1번째 데이터 x
		{
			enemy.x = atoi(temp);
		}
		else if (dataOrder % 3 == 2)	//2번째 데이터 y
		{
			enemy.y = atoi(temp);
			setEnemy(enemy);
		}
		dataOrder++;
	}
}

void enemyManager::setEnemy(tagEnemy & stEnemy)
{
	enemy* enemy;

	switch (stEnemy.type)
	{
	case BEETO:
		enemy = new beeto;
		enemy->init(_map, _objMgr, stEnemy.x, stEnemy.y);
		_vEnemy.push_back(enemy);
		break;
	case BLORB:
		break;
	case SKELETON:
		break;
	case BUBBLEDRAGON:
		break;
	}
}
