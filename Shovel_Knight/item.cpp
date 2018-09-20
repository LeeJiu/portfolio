#include "stdafx.h"
#include "item.h"
#include "gameNode.h"


item::item()
{
}


item::~item()
{
}

HRESULT item::init(ITEMTYPE type, float x, float y, bool isBounce, bool isRight)
{
	_image = new image;

	_type = type;

	//이미지
	switch (_type)
	{
	case GEM1:
		_image = IMAGEMANAGER->findImage("gems");
		_imgFrameX = 0;
		_gold = 1;
		break;
	case GEM5:
		_image = IMAGEMANAGER->findImage("gems");
		_imgFrameX = 1;
		_gold = 5;
		break;
	case GEM10:
		_image = IMAGEMANAGER->findImage("gems");
		_imgFrameX = 2;
		_gold = 10;
		break;
	case GEM20:
		_image = IMAGEMANAGER->findImage("gems");
		_imgFrameX = 3;
		_gold = 20;
		break;
	case GEM35:
		_image = IMAGEMANAGER->findImage("gems");
		_imgFrameX = 4;
		_gold = 35;
		break;
	case GEM50:
		_image = IMAGEMANAGER->findImage("gems");
		_imgFrameX = 5;
		_gold = 50;
		break;
	case LIFE:
		break;
	}

	_x = x;
	_y = y;
	_rc = RectMake(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	
	_jumpPwr = RND->getFromFloatTo(2.f, 4.f);
	_gravity = 0.5f;
	_count = 0.f;

	_isOn = true;
	_isBounce = isBounce;
	
	if (_isBounce)
	{
		_isRight = isRight;
	}

	return S_OK;
}

void item::release()
{
	_image->release();
	SAFE_DELETE(_image);
}

void item::update()
{
	if (_x < _sourX || _x > _sourX + WINSIZEX)
	{
		_isOn = false;
		return;
	}

	if (_isBounce)
		bounce();

	_y -= _jumpPwr;
	_jumpPwr -= _gravity;

	setRect(_x, _y);

	pixelCollision();
}

void item::render()
{
	//Rectangle(getMemDC(), _rc.left, _rc.top, _rc.right, _rc.bottom);

	if(_isOn)
		_image->frameRender(gameNode::getMemDC(), _x, _y, _imgFrameX, 0);
}

void item::bounce()
{
	if (_isRight)
	{
		_x -= 5;
	}
	else/* if (!_isRight)*/
	{
		_x += 5;
	}
}

void item::pixelCollision()
{
	//좌
	if (GetPixel(gameNode::getPixelDC(), _rc.left, (_rc.top + _rc.bottom) / 2) == 0xff00ff)
	{
		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.left + 1; ; ++i)
		{
			if (GetPixel(gameNode::getPixelDC(), i, (_rc.top + _rc.bottom) / 2) != 0xff00ff)
			{
				int interval = i - _rc.left;
				_x += interval;
				setRect(_x, _y);
				break;
			}
		}
	}

	//우
	if (GetPixel(gameNode::getPixelDC(), _rc.right, (_rc.top + _rc.bottom) / 2) == 0xff00ff)
	{
		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.right - 1; ; --i)
		{
			if (GetPixel(gameNode::getPixelDC(), i, (_rc.top + _rc.bottom) / 2) != 0xff00ff)
			{
				int interval = _rc.right - i;
				_x -= interval;
				setRect(_x, _y);
				break;
			}
		}
	}

	//상
	if (GetPixel(gameNode::getPixelDC(), (_rc.left + _rc.right) / 2, _rc.top) == 0xff00ff)
	{
		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.top + 1; ; ++i)
		{
			if (GetPixel(gameNode::getPixelDC(), (_rc.left + _rc.right) / 2, i) != 0xff00ff)
			{
				int interval = i - _rc.top;
				_y += interval;
				setRect(_x, _y);
				break;
			}
		}
	}

	//하
	if (GetPixel(gameNode::getPixelDC(), (_rc.left + _rc.right) / 2, _rc.bottom) == 0xff00ff)
	{
		_isBounce = false;
		_jumpPwr = 0.f;
		_gravity = 0.5f;

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.bottom - 1; ; --i)
		{
			if (GetPixel(gameNode::getPixelDC(), (_rc.left + _rc.right) / 2, i) != 0xff00ff)
			{
				int interval = _rc.bottom - i;
				_y -= interval;
				setRect(_x, _y);
				break;
			}
		}

		if (_isOn)
		{
			_count += TIMEMANAGER->getElapsedTime();

			if (_count > 1.5f)
			{
				_count = 0.f;
				_isOn = false;
			}
		}
	}
}

void item::setRect(float x, float y)
{
	_x = x;
	_y = y;
	_rc = RectMake(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
}
