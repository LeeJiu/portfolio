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

	for (int i = 0; i < _objectMgr->getVObject().size(); ++i)
	{
		if (_objectMgr->getVObject()[i]->getType() == PLAYER)
		{
			if (_objectMgr->getVObject()[i]->getRect().right > WINSIZEX)
			{
				_saveX = _objectMgr->getVObject()[i]->getRect().left;
				_saveIdx = i;
				break;
			}
		}
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

	sprintf_s(str, "saveX : %d", _saveX);
	TextOut(getMemDC(), 0, 80, str, strlen(str));
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
		_objectMgr->getVObject()[_saveIdx]->setX(_saveX);
	}
}

//맵 위에 오브젝트를 깔아주자 / 적, 아이템
void gameMap::setObject()
{
	//적을 배치한다
	if (_moveX == 0)
	{
		//_objectMgr->setObject(BOXBOY, 500, CENTERY);
		_objectMgr->setObject(CHOCOBEE, 500, CENTERY);
	}
	else if (_moveX == 570)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 100, CENTERY + 50);
		_objectMgr->setObject(CHOCOBEE, WINSIZEX - 150, CENTERY + 100);
	}
	else if (_moveX <= 1150 && _moveX >= 1100)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX <= 1750 && _moveX >= 1700)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX <= 2300 && _moveX >= 2250)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX <= 2900 && _moveX >= 2850)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
	else if (_moveX >= 3350)
	{
		_objectMgr->setObject(BOXBOY, WINSIZEX - 50, CENTERY);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 200, CENTERY + 50);
		_objectMgr->setObject(BOXBOY, WINSIZEX - 350, CENTERY + 100);
	}
}
