#include "stdafx.h"
#include "beeto.h"
#include "stageMap.h"


beeto::beeto()
{
}


beeto::~beeto()
{
}

HRESULT beeto::init(stageMap* map, objectManager* objMgr, int x, int y)
{
	_distance = 128;
	_dirX = 1;

	_gravity = 0.8f;

	_state = ES_WALK;

	_map = map;
	_objMgr = objMgr;

	_image = new image;
	_image->init("image/enemy/beeto_walk.bmp", 208, 64, 4, 2, true, 0xff00ff);

	_x = x;
	_y = y;
	_rc = RectMake(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	return S_OK;
}

void beeto::release()
{
	_image->release();
	SAFE_DELETE(_image);
}

void beeto::update()
{
	move();

	setImage();
}

void beeto::render()
{
	Rectangle(getMemDC(), _rc.left, _rc.top, _rc.right, _rc.bottom);

	if(!_isDead)
		_image->frameRender(getMemDC(), _x, _y, _curFrameX, _curFrameY);
}

void beeto::move()
{
	_y += _gravity;

	_x += _dirX;
	_distance -= 2;

	if (_distance <= 0)
	{
		_distance = 128;
		_dirX *= -1;
	}

	if (_dirX < 0)
		_curFrameY = 1;
	else
		_curFrameY = 0;

	_rc = RectMake(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	tileCollision();
}

void beeto::tileCollision()
{
	for (int y = _sourY / TILESIZE; y < (_sourY + WINSIZEY) / TILESIZE; ++y)
	{
		for (int x = _sourX / TILESIZE; x < (_sourX + WINSIZEX) / TILESIZE; ++x)
		{
			if (x < 0 || x > TILENUMX) break;
			if (y < 0 || y > TILENUMY) break;

			if (_map->getTile()[y][x].type != CRASHABLE
				&& _map->getTile()[y][x].type != BREAKABLE
				&& _map->getTile()[y][x].type != LADDER)
				continue;

			RECT temp;
			if (IntersectRect(&temp, &_map->getTile()[y][x].rc, &_rc))
			{

				int width = temp.right - temp.left;
				int height = temp.bottom - temp.top;

				_y -= height;
				_rc = RectMake(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
				return;
			}
		}
	}
}

void beeto::setImage()
{
	switch (_state)
	{
	case ES_IDLE:
		break;
	case ES_WALK:
		_image->init("image/enemy/beeto_walk.bmp", 208, 64, 4, 2, true, 0xff00ff);
		_image->setFrameY(_curFrameY);
		break;
	case ES_ATTACK:
		break;
	case ES_DEAD:
		_image->init("image/enemy/beeto_dead.bmp", 104, 64, 2, 2, true, 0xff00ff);
		_image->setFrameY(_curFrameY);
		break;
	}

	setFrame();
}

void beeto::setFrame()
{
	_count++;

	if (_count % 10 == 0)
	{
		_curFrameX++;

		if (_curFrameX > _image->getMaxFrameX())
		{
			_curFrameX = 0;
		}
		_image->setFrameX(_curFrameX);
	}
}
