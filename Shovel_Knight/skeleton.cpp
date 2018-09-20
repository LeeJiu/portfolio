#include "stdafx.h"
#include "skeleton.h"
#include "player.h"


skeleton::skeleton()
{
}


skeleton::~skeleton()
{
}

HRESULT skeleton::init(int x, int y)
{
	_alpha = 255;

	_deadCnt = 0.f;

	_gravity = 0.5f;

	_isRight = true;
	_isAware = false;
	_state = ES_IDLE;
	_curFrameX = _curFrameY = 0;

	_image = new image;
	_image->init("image/enemy/skeleton_idle.bmp", 78, 116, 1, 2, true, 0xff00ff);

	_initX = _x = x + _image->getFrameWidth() * 0.5;
	_initY = _y = y + _image->getFrameHeight() * 0.5;
	setRect();

	_atkRange = 64;
	_initLife = _life = 2;

	return S_OK;
}

void skeleton::release()
{
	enemy::release();
}

void skeleton::update()
{
	aware();

	if (_isAware)
		attack();

	collision();

	move();

	setImage();
}

void skeleton::move()
{
	if (_isBounce)
		bounce();

	_y -= _jumpPwr;
	_jumpPwr -= _gravity;

	if (_state == ES_WALK)
	{
		if (_isRight)
		{
			_x += 2;
		}
		else
		{
			_x -= 2;
		}
	}

	setRect();

	enemy::pixelCollision();

	if (_state == ES_DEAD)
	{
		if (_life != 0 && _jumpPwr == 0.f)
		{
			_state = ES_IDLE;
			_curFrameX = 0;
		}
		else
		{
			_deadCnt += TIMEMANAGER->getElapsedTime();

			if (_deadCnt > 0.5f)
			{
				_deadCnt = 0.f;
				ITEMMANAGER->addGemsItem(_x, _y, false, false, 60);
				setIsDead(true);
			}
		}
	}
}

void skeleton::bounce()
{
	if (_state != ES_DEAD)
	{
		_state = ES_DEAD;
		_curFrameX = 0;
	}

	if (_isRightHit)
	{
		_x += 5;
	}
	else
	{
		_x -= 5;
	}
}

void skeleton::attack()
{
	//공격 범위 밖이면 return
	if (abs(_x - _player->getX()) > _atkRange) return;

	//공격 중이면 return
	if (_state == ES_ATTACK) return;

	//좌-플레이어
	if (_x >= _player->getX())
	{
		_curFrameY = 1;
	}
	//우-플레이어
	else
	{
		_curFrameY = 0;
	}

	if (IntersectRect(&RectMake(0, 0, 0, 0), &_rc, &_player->getCollRect()))
	{
		_state = ES_ATTACK;
		_curFrameX = 0;

		_player->damage(1);
		_player->setBounce(true, 5.f);
	}
}

void skeleton::aware()
{
	//인식 범위에 플레이어가 들어왔는지
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_aware, &_player->getCollRect()))
	{
		_isAware = true;

		if (_state == ES_IDLE)
		{
			_state = ES_WALK;
			_curFrameX = 0;

			if (_player->getX() < _x)
			{
				_isRight = false;
				_curFrameY = 1;
			}
			else
			{
				_isRight = true;
				_curFrameY = 0;
			}
		}
		return;
	}

	_isAware = false;

	if (_state == ES_WALK)
	{
		_state = ES_IDLE;
		_curFrameX = 0;
	}
}

void skeleton::collision()
{
	if (_state == ES_DEAD) return;

	if (_player->getState() == PS_DIG)
	{
		if (IntersectRect(&RectMake(0, 0, 0, 0), &_coll, &_player->getAtkRect()))
		{
			enemy::setBounce(true, _player->getIsRight(), 5.f);
			_life -= 1;
			if (_life <= 0)
				_life = 0;
		}
	}
	else if (_player->getState() == PS_DIGDOWN)
	{
		if (IntersectRect(&RectMake(0, 0, 0, 0), &_coll, &_player->getCollRect()))
		{
			_player->setBounce(true, 8.f);
			enemy::setBounce(false, false, 5.f);
			_life -= 1;
			if (_life <= 0)
				_life = 0;
		}
	}
	else if (_player->getState() == PS_DEAD)
	{
		_state = ES_IDLE;
		_curFrameX = 0;
	}
}

void skeleton::setImage()
{
	switch (_state)
	{
	case ES_IDLE:
		_image->init("image/enemy/skeleton_idle.bmp", 78, 116, 1, 2, true, 0xff00ff);
		_image->setFrameY(_curFrameY);
		break;
	case ES_WALK:
		_image->init("image/enemy/skeleton_walk.bmp", 312, 124, 4, 2, true, 0xff00ff);
		_image->setFrameY(_curFrameY);
		break;
	case ES_ATTACK:
		_image->init("image/enemy/skeleton_attack.bmp", 272, 144, 2, 2, true, 0xff00ff);
		_image->setFrameY(_curFrameY);
		break;
	case ES_DEAD:
		_image->init("image/enemy/skeleton_idle.bmp", 78, 116, 1, 2, true, 0xff00ff);
		_image->alphaInit();
		_image->setFrameY(_curFrameY);
		break;
	}

	setFrame();
}

void skeleton::setFrame()
{
	_count++;

	if (_state == ES_DEAD)
	{
		if (_count % 5 == 0)
		{
			if (_alpha == 256)
				_alpha -= 128;
			else
				_alpha += 128;
		}
	}
	else if (_state == ES_ATTACK)
	{
		if (_count % 20 == 0)
		{
			_curFrameX++;

			if (_curFrameX > _image->getMaxFrameX())
			{
				_state = ES_IDLE;
				_curFrameX = 0;
			}
			_image->setFrameX(_curFrameX);
		}
	}

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

void skeleton::setRect()
{
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_aware = RectMakeCenter(_x, _y, 300, 64);
	_coll = RectMakeCenter(_x, _y, 80, 64);
}