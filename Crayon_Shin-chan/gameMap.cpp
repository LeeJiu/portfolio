#include "stdafx.h"
#include "gameMap.h"
#include "player.h"


gameMap::gameMap()
{
}


gameMap::~gameMap()
{
}

HRESULT gameMap::init()
{
	_map = IMAGEMANAGER->addImage("stage1", "image/background/stage1.bmp", 4000, 480, false, false);

	_moveX = 0;

	return S_OK;
}

void gameMap::release()
{
}

void gameMap::update()
{
	//플레이어가 오른쪽 끝 화면에 닿으면
	if (_player->getRect().right > WINSIZEX)
	{
		_saveX = _player->getRect().left;
	}
	move();
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
		if (_saveX < 0)
			_saveX = 0;
		_moveX += 5;
		_player->setX(_saveX);
	}
}
