#include "stdafx.h"
#include "gameMap.h"
#include "gameObject.h"
#include "objectManager.h"


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
	_enemyCnt = 0;

	checkScroll();
	findPlayer();

	if (_enemyCnt == 0) 
	{
		move();
	}

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
	TextOut(getMemDC(), 0, 90, str, strlen(str));

	sprintf_s(str, "saveX : %d", _saveX);
	TextOut(getMemDC(), 0, 110, str, strlen(str));
}

void gameMap::checkScroll()
{
	//맵에 적이 모두 죽지 않으면 스크롤하지 않는다
	for (int i = 0; i < _objectMgr->getVObject().size(); ++i)
	{
		if (_objectMgr->getVObject()[i]->getType() == PLAYER) continue;
		else
		{
			if (!_objectMgr->getVObject()[i]->isDead())
				_enemyCnt++;
		}
	}
}

void gameMap::findPlayer()
{
	for (int i = 0; i < _objectMgr->getVObject().size(); ++i)
	{
		if (_objectMgr->getVObject()[i]->getType() == PLAYER)
		{
			if (_enemyCnt == 0 && _objectMgr->getVObject()[i]->getRect().right > WINSIZEX)
			{
				_saveX = _objectMgr->getVObject()[i]->getRect().left;
				_saveIdx = i;
				break;
			}
		}
	}
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
		else if (_moveX >= 3350 && _saveX >= 60)
		{
			_settingCnt = 0;
		}

		_moveX += 5;
		_objectMgr->getVObject()[_saveIdx]->setX(_saveX);
	}
}

//맵 위에 오브젝트를 깔아주자 / 적, 아이템
void gameMap::setObject()
{
	//적을 배치한다
	if (_moveX == 0)
	{
		_objectMgr->setObject(BOXBOY, 500, CENTERY);
		//_objectMgr->setObject(CHOCOBEE, 500, CENTERY);
		//_objectMgr->setObject(HIP, 500, CENTERY);
	}
	else if (_moveX == 570)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 100);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 350, CENTERY + 200);
		_objectMgr->setObject(CHOCOBEE, 300, 200);
	}
	else if (_moveX <= 1150 && _moveX >= 1100)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 100);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 350, CENTERY + 200);
	}
	else if (_moveX <= 1750 && _moveX >= 1700)
	{
		_objectMgr->setObject(CHOCOBEE, WINSIZEX - 100, WINSIZEY - 100);
		_objectMgr->setObject(BOXBOY, CENTERX, CENTERY);
		_objectMgr->setObject(BOXBOY, CENTERX + 100, CENTERY + 50);
		_objectMgr->setObject(BOXBOY, CENTERX - 50, CENTERY + 100);
	}
	else if (_moveX <= 2300 && _moveX >= 2250)
	{
		_objectMgr->setObject(CHOCOBEE, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 100);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 200);
	}
	else if (_moveX <= 2900 && _moveX >= 2850)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 100, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 100, CENTERY + 100);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 150);
	}
	else if (_moveX >= 3350)
	{
		_objectMgr->setObject(BOXBOY, CENTERX, CENTERY);
		_objectMgr->setObject(BOXBOY, CENTERX, CENTERY + 100);
		_objectMgr->setObject(BOXBOY, CENTERX, CENTERY + 200);
		_objectMgr->setObject(HIP, WINSIZEX - 50, CENTERY);
	}
}
