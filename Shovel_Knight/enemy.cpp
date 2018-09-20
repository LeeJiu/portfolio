#include "stdafx.h"
#include "enemy.h"
#include "player.h"


enemy::enemy()
{
}


enemy::~enemy()
{
}

HRESULT enemy::init(int x, int y)
{
	return S_OK;
}

void enemy::release()
{
	_image->release();
	SAFE_DELETE(_image);
}

void enemy::update()
{
}

void enemy::render()
{
	if (!_isDead)
	{
		//Rectangle(getMemDC(), _rc.left, _rc.top, _rc.right, _rc.bottom);
		//Rectangle(getMemDC(), _aware.left, _aware.top, _aware.right, _aware.bottom);
		//Rectangle(getMemDC(), _coll.left, _coll.top, _coll.right, _coll.bottom);

		if (_state == ES_DEAD)
		{
			_image->alphaRender(getMemDC(), _rc.left, _rc.top,
				_curFrameX * _image->getFrameWidth(), _curFrameY * _image->getFrameHeight(),
				_image->getFrameWidth(), _image->getFrameHeight(), _alpha);
		}
		else
		{
			_image->frameRender(getMemDC(), _rc.left, _rc.top, _curFrameX, _curFrameY);
		}
	}
}

void enemy::pixelCollision()
{
	//좌
	if (GetPixel(getPixelDC(), _rc.left, (_rc.top + _rc.bottom) / 2) == 0xff00ff)
	{
		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.left + 1; ; ++i)
		{
			if (GetPixel(getPixelDC(), i, (_rc.top + _rc.bottom) / 2) != 0xff00ff)
			{
				int interval = i - _rc.left;
				_x += interval;
				setRect();
				break;
			}
		}
	}

	//우
	if (GetPixel(getPixelDC(), _rc.right, (_rc.top + _rc.bottom) / 2) == 0xff00ff)
	{
		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.right - 1; ; --i)
		{
			if (GetPixel(getPixelDC(), i, (_rc.top + _rc.bottom) / 2) != 0xff00ff)
			{
				int interval = _rc.right - i;
				_x -= interval;
				setRect();
				break;
			}
		}
	}

	//상
	if (GetPixel(getPixelDC(), (_rc.left + _rc.right) / 2, _rc.top) == 0xff00ff)
	{
		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.top + 1; ; ++i)
		{
			if (GetPixel(getPixelDC(), (_rc.left + _rc.right) / 2, i) != 0xff00ff)
			{
				int interval = i - _rc.top;
				_y += interval;
				setRect();
				break;
			}
		}
	}

	//하
	if (GetPixel(getPixelDC(), (_rc.left + _rc.right) / 2, _rc.bottom) == 0xff00ff)
	{
		_isBounce = false;
		_jumpPwr = 0.f;
		_gravity = 0.5f;

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _rc.bottom - 1; ; --i)
		{
			if (GetPixel(getPixelDC(), (_rc.left + _rc.right) / 2, i) != 0xff00ff)
			{
				int interval = _rc.bottom - i;
				_y -= interval;
				setRect();
				break;
			}
		}
	}
}

void enemy::setRect()
{
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
}

void enemy::setIsDead(bool isDead)
{
	_isDead = isDead;
	_x = _initX;
	_y = _initY;
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	_state = ES_IDLE;
	_curFrameX = 0;

	_life = _initLife;
}

void enemy::setBounce(bool isBounce, bool isRightHit, float jumpPwr)
{
	if (!_isBounce)
	{
		_isBounce = isBounce;
		_isRightHit = isRightHit;
		_jumpPwr = jumpPwr;

		_state = ES_DEAD;
		_curFrameX = 0;
	}
}
