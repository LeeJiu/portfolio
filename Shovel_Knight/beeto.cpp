#include "stdafx.h"
#include "beeto.h"
#include "player.h"


beeto::beeto()
{
}


beeto::~beeto()
{
}

HRESULT beeto::init(int x, int y)
{
	_distance = 128;
	_dirX = 1;

	_alpha = 255;

	_deadCnt = 0.f;

	_gravity = 0.5f;

	_initLife = _life = 1;

	_state = ES_WALK;

	_image = new image;
	_image->init("image/enemy/beeto_walk.bmp", 208, 64, 4, 2, true, 0xff00ff);

	_initX = _x = x + _image->getFrameWidth() * 0.5;
	_initY = _y = y + _image->getFrameHeight() * 0.5;
	setRect();

	return S_OK;
}

void beeto::release()
{
	enemy::release();
}

void beeto::update()
{
	collision();
	move();

	setImage();
}

void beeto::move()
{
	if (_isBounce)
	{
		bounce();

		_y -= _jumpPwr;
		_jumpPwr -= _gravity;
	}
	else
	{
		_y -= _jumpPwr;
		_jumpPwr -= _gravity;
		
		if (_state != ES_DEAD) 
		{
			_x += _dirX;
			_distance -= 2;
			if (_distance <= 0)
			{
				_distance = 128;
				_dirX *= -1;
			}
		}
	}

	if (_dirX < 0)
		_curFrameY = 1;
	else
		_curFrameY = 0;

	setRect();

	enemy::pixelCollision();

	if (_state == ES_DEAD)
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

void beeto::bounce()
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

void beeto::collision()
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
	else
	{
		if (IntersectRect(&RectMake(0, 0, 0, 0), &_coll, &_player->getCollRect()))
		{
			_player->setBounce(true, 8.f);
			_player->damage(1);
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
		_image->alphaInit();
		_image->setFrameY(_curFrameY);
		break;
	}

	setFrame();
}

void beeto::setFrame()
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

void beeto::setIsDead(bool isDead)
{
	enemy::setIsDead(isDead);
	
	_state = ES_WALK;
	_curFrameX = 0;
}

void beeto::setRect()
{
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_coll = _rc;
}
