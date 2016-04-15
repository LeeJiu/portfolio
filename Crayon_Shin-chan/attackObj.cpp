#include "stdafx.h"
#include "attackObj.h"


attackObj::attackObj()
{
}


attackObj::~attackObj()
{
}

HRESULT attackObj::init(int objMax, float range)
{
	_range = range;

	for (int i = 0; i < objMax; i++)
	{
		tagAttObj attObj;
		ZeroMemory(&attObj, sizeof(tagAttObj)); //memset과 동일 메모리를 0으로 초기화 시킨다.
		attObj.image = new image;
		attObj.image->init("image/effect/fireball.bmp", 180, 80, 2, 2, true, 0xff00ff);
		attObj.state = MOVE;
		attObj.speed = 5.0f;
		attObj.isFire = false;

		_vAttObj.push_back(attObj);
	}


	return S_OK;
}

void attackObj::release()
{
}

void attackObj::update()
{
	move();
	setFrame();
}

void attackObj::render()
{
	for (_viAttObj = _vAttObj.begin(); _viAttObj != _vAttObj.end(); ++_viAttObj)
	{
		if (!_viAttObj->isFire) continue;

		//프레임 렌더로 바꿔야한다.
		/*_viAttObj->image->render(getMemDC(), _viAttObj->coll.left,
			_viAttObj->coll.top, 0, 0, _viAttObj->image->getFrameWidth(),
			_viAttObj->image->getFrameHeight());*/
		_viAttObj->image->frameRender(getMemDC(), _viAttObj->coll.left, _viAttObj->coll.top, 
			_viAttObj->image->getFrameX(), _viAttObj->image->getFrameY());
	}
}

void attackObj::fire(int x, int y, bool isRight)
{
	for (_viAttObj = _vAttObj.begin(); _viAttObj != _vAttObj.end(); ++_viAttObj)
	{
		if (_viAttObj->isFire)continue;

		_viAttObj->isFire = true;
		_viAttObj->isRight = isRight;
		_viAttObj->pt.x = _viAttObj->fire.x = x;
		_viAttObj->pt.y = _viAttObj->fire.y = y;
		_viAttObj->coll = RectMakeCenter(_viAttObj->pt.x, _viAttObj->pt.y,
			_viAttObj->image->getFrameWidth(),
			_viAttObj->image->getFrameHeight());
		break;
	}
}

void attackObj::move()
{
	for (_viAttObj = _vAttObj.begin(); _viAttObj != _vAttObj.end(); ++_viAttObj)
	{
		if (!_viAttObj->isFire) continue;

		if(_viAttObj->isRight)
			_viAttObj->pt.x += _viAttObj->speed;
		else
			_viAttObj->pt.x -= _viAttObj->speed;

		_viAttObj->coll = RectMakeCenter(_viAttObj->pt.x, _viAttObj->pt.y,
			_viAttObj->image->getFrameWidth(),
			_viAttObj->image->getFrameHeight());

		if (_range < MY_UTIL::getDistance(_viAttObj->fire.x, _viAttObj->fire.y, _viAttObj->pt.x, _viAttObj->pt.y))
		{
			_viAttObj->isFire = false;
		}
	}
}

void attackObj::setImage()
{
}

void attackObj::setFrame()
{
	_count++;

	if (_count % 5 == 0)
	{
		for (_viAttObj = _vAttObj.begin(); _viAttObj != _vAttObj.end(); ++_viAttObj)
		{
			if (!_viAttObj->isFire) continue;

			
			
		}
	}
}
