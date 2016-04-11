#include "stdafx.h"
#include "player.h"


player::player()
{
}


player::~player()
{
}

HRESULT player::init()
{
	_player.pt.x = CENTERX/3;
	_player.pt.y = CENTERY;
	_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, 50, 50);


	return S_OK;
}

void player::release()
{
}

void player::update()
{
	move();
}

void player::render()
{
	Rectangle(getMemDC(), _player.coll.left, _player.coll.top, _player.coll.right, _player.coll.bottom);
}

void player::move()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if(_player.coll.left > 0)
			_player.pt.x -= 5;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if(_player.coll.right <= WINSIZEX)
			_player.pt.x += 5;
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		if (_player.coll.bottom > CENTERY)
			_player.pt.y -= 5;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (_player.coll.bottom < WINSIZEY)
			_player.pt.y += 5;
	}

	_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, 50, 50);
}

void player::attack()
{
}

void player::damage()
{
}

void player::dead()
{
}

void player::collision()
{
}