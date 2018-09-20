#include "stdafx.h"
#include "bubbleShot.h"
#include "player.h"


bubbleShot::bubbleShot()
{
}


bubbleShot::~bubbleShot()
{
}

HRESULT bubbleShot::init(player * player, float x, float y)
{
	_player = player;

	_isRender = false;

	_image = new image;
	_image->init("image/object/bubble.bmp", 256, 64, 4, 1, true, 0xff00ff);
	_curFrameX = 0;

	_initX = _x = x;
	_initY = _y = y;

	setRect();

	_speed = 6.f;

	return S_OK;
}

void bubbleShot::release()
{
	_image->release();
	SAFE_DELETE(_image);
}

void bubbleShot::update()
{
	if (!_isRender)	return;

	_delay -= TIMEMANAGER->getElapsedTime();
	if (_delay > FLOAT_EPSILON) return;

	_range -= _speed;
	_speed -= 0.1f;
	if (_speed <= FLOAT_EPSILON)
		_speed = 2.f;
	if (_range <= FLOAT_EPSILON)
	{
		_range = 0.f;
		setRender(false);
		return;
	}
	else
	{
		_x -= _speed;
		_y -= 0.4f;
		setRect();
	}
	
	collision();

	setFrame();
}

void bubbleShot::render()
{
	if (_isRender)
	{
		if(_delay <= FLOAT_EPSILON)
			_image->frameRender(getMemDC(), _rc.left, _rc.top, _curFrameX, 0);
	}
}

void bubbleShot::collision()
{
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_rc, &_player->getCollRect()))
	{
		_player->damage(1);
		_player->setBounce(true, 3.f);
		setRender(false);
	}
}

void bubbleShot::setRect()
{
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
}

void bubbleShot::setFrame()
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

void bubbleShot::setRender(bool render, float time)
{
	_isRender = render;
	_delay = time;

	if (!_isRender)
	{
		EFFECTMANAGER->play("bubble_pong", _x, _y);
	}

	_x = _initX;
	_y = _initY;
	_speed = 6.f;
	_range = RND->getFromFloatTo(200.f, 250.f);
	setRect();
}
