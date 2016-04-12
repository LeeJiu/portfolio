#include "stdafx.h"
#include "gameMap.h"
#include "player.h"
#include "enemyManager.h"


gameMap::gameMap()
{
}


gameMap::~gameMap()
{
}

HRESULT gameMap::init()
{
	_map = IMAGEMANAGER->addImage("stage1", "image/background/stage1.bmp", 4000, 480, false, false);

	_moveX = _settingCnt = 0;

	return S_OK;
}

void gameMap::release()
{
}

void gameMap::update()
{
	_settingCnt++;

	//플레이어가 오른쪽 끝 화면에 닿으면
	if (_player->getRect().right > WINSIZEX)
	{
		_saveX = _player->getRect().left;
		
	}
	move();

	if (_settingCnt == 1)
	{
		setObject();
	}
}

void gameMap::render()
{
	_map->render(getMemDC(), 0, 0, _moveX, 0, WINSIZEX, WINSIZEY);

	char str[128];
	sprintf_s(str, "moveX : %d", _moveX);
	TextOut(getMemDC(), 0, 60, str, strlen(str));
}

void gameMap::move()
{
	if (_saveX > 0 && _moveX < 4000 - WINSIZEX)
	{
		_saveX -= 5;
		if (_saveX <= 0)
		{
			_saveX = 0;
			_settingCnt = 0;
		}
		_moveX += 5;
		_player->setX(_saveX);
	}
}

//맵 위에 오브젝트를 깔아주자 / 적, 아이템
void gameMap::setObject()
{
	//적을 배치한다
	if (_moveX == 0)
	{
		_enemyMgr->setEnemy(BOXBOY, 500, CENTERY);
	}
	else if (_moveX == 570)
	{
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 50, CENTERY);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 100, CENTERY + 50);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 150, CENTERY + 100);
	}
	else if (_moveX == 1145)
	{
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 50, CENTERY);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX == 1720)
	{
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 50, CENTERY);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX == 2295)
	{
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 50, CENTERY);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX == 2870)
	{
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 50, CENTERY);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX == 3360)
	{
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 50, CENTERY);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_enemyMgr->setEnemy(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
}
