#include "stdafx.h"
#include "boxboy.h"
#include "player.h"


boxboy::boxboy()
{
}


boxboy::~boxboy()
{
}

HRESULT boxboy::init(int x, int y)
{
	_enemy.pt.x = x;
	_enemy.pt.y = y;
	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, 50, 50);
	_enemy.isDead = false;

	return S_OK;
}

void boxboy::release()
{
}

void boxboy::update()
{
	move();
	collision();
}

void boxboy::render()
{
	Rectangle(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.coll.right, _enemy.coll.bottom);
}

void boxboy::move()
{
	//플레이어 쪽으로 이동
	if (_enemy.pt.x > _player->getX() + 50)
		_enemy.pt.x -= 2;
	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, 50, 50);
}

void boxboy::attack()
{
}

void boxboy::damage()
{
}

void boxboy::dead()
{
	_enemy.isDead = true;
}

void boxboy::collision()
{
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_enemy.coll, &_player->getRect()))
	{
		dead();
	}
}
