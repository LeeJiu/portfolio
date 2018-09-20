#include "stdafx.h"
#include "dragon.h"
#include "player.h"


dragon::dragon()
{
}


dragon::~dragon()
{
}

HRESULT dragon::init(int x, int y)
{
	_alpha = 255;

	_deadCnt = _unbeatCnt = 0.f;

	_initLife = _life = 8;

	_isAware = false;
	_awareW = 450;
	_awareH = 250;

	_atkRange = 300, _atk2Range = 175;

	_state = ES_IDLE;

	_image = new image;
	_image->init("image/enemy/bubbleDragon_idle.bmp", 4344, 178, 12, 1, true, 0xff00ff);

	_initX = _x = x + _image->getFrameWidth() * 0.5;
	_initY = _y = y + _image->getFrameHeight() * 0.5 + 8;
	setRect();

	for (int i = 0; i < 3; ++i)
	{
		_bubbles[i].init(_player, _atk.left + 64, _atk.top + (_atk.bottom - _atk.top) * 0.5);
	}

	return S_OK;
}

void dragon::release()
{
	enemy::release();
}

void dragon::update()
{
	aware();

	if(_isAware)
		attack();

	for (int i = 0; i < 3; ++i)
	{
		_bubbles[i].update();
	}

	collision();

	deadMotion();

	setImage();
}

void dragon::render()
{
	if (!_isDead)
	{
		//Rectangle(getMemDC(), _coll.left, _coll.top, _coll.right, _coll.bottom);
		//Rectangle(getMemDC(), _coll2.left, _coll2.top, _coll2.right, _coll2.bottom);
		//Rectangle(getMemDC(), _atk.left, _atk.top, _atk.right, _atk.bottom);
		//Rectangle(getMemDC(), _aware.left, _aware.top, _aware.right, _aware.bottom);

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

		for (int i = 0; i < 3; ++i)
		{
			_bubbles[i].render();
		}
	}
}

void dragon::aware()
{
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_aware, &_player->getCollRect()))
	{
		_isAware = true;
		return;
	}

	_isAware = false;
}

void dragon::attack()
{
	_dist = abs(_x - _player->getX());

	//공격 범위 밖이면 return
	if (_dist > _atkRange) return;

	//공격 중이거나 데미지 입는 중이면 return
	if (_state == ES_ATTACK || _state == ES_ATTACK2 || _state == ES_DEAD) return;

	if (_bubbles[2].getRender()) return;


	if (_dist < _atkRange && _dist > _atk2Range)
	{
		_state = ES_ATTACK;
		_curFrameX = 0;
		
		for (int i = 0; i < 3; ++i)
		{
			_bubbles[i].setRender(true, 0.2f * i);
		}
	}
	else if(_dist < _atk2Range)
	{
		_state = ES_ATTACK2;
		_curFrameX = 0;

		if (IntersectRect(&RectMake(0, 0, 0, 0), &_atk, &_player->getCollRect()))
		{
			_player->damage(1);
			_player->setBounce(true, 5.f);
		}
	}	
}

void dragon::collision()
{
	if (_state == ES_DEAD) return;

	if (_player->getState() == PS_DIG)
	{
		if (IntersectRect(&RectMake(0, 0, 0, 0), &_coll, &_player->getAtkRect())
			|| IntersectRect(&RectMake(0, 0, 0, 0), &_coll2, &_player->getAtkRect()))
		{
			_life -= 1;
			if (_life <= 0)
				_life = 0;

			_state = ES_DEAD;
			_curFrameX = 0;
		}
	}
	else if (_player->getState() == PS_DIGDOWN)
	{
		if (IntersectRect(&RectMake(0, 0, 0, 0), &_coll, &_player->getCollRect())
			|| IntersectRect(&RectMake(0, 0, 0, 0), &_coll2, &_player->getCollRect()))
		{
			_player->setBounce(false, 10.f);
			_life -= 1;
			if (_life <= 0)
				_life = 0;

			_state = ES_DEAD;
			_curFrameX = 0;
		}
	}
	else if (_player->getState() == PS_DEAD)
	{
		_state = ES_IDLE;
		_curFrameX = 0;
	}
}

void dragon::setIsDead(bool isDead)
{
	if (_isDead)
		return;

	_isDead = isDead;
}

void dragon::deadMotion()
{
	if (_state == ES_DEAD)
	{
		if (_life != 0 && _jumpPwr == 0.f)
		{
			_unbeatCnt += TIMEMANAGER->getElapsedTime();

			if (_unbeatCnt > 0.5f)
			{
				_unbeatCnt = 0.f;
				_state = ES_IDLE;
				_curFrameX = 0;
			}
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

void dragon::setImage()
{
	switch (_state)
	{
	case ES_IDLE:
		_image->init("image/enemy/bubbleDragon_idle.bmp", 4344, 178, 12, 1, true, 0xff00ff);
		break;
	case ES_ATTACK:
		_image->init("image/enemy/bubbleDragon_attack.bmp", 1086, 178, 3, 1, true, 0xff00ff);
		break;
	case ES_ATTACK2:
		_image->init("image/enemy/bubbleDragon_attack2.bmp", 2172, 178, 6, 1, true, 0xff00ff);
		break;
	case ES_DEAD:
		_image->init("image/enemy/bubbleDragon_idle.bmp", 4344, 178, 12, 1, true, 0xff00ff);
		_image->alphaInit();
		break;
	}

	setFrame();
}

void dragon::setFrame()
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
	else if (_state == ES_ATTACK || _state == ES_ATTACK2)
	{
		if (_count % 10 == 0)
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
	else
	{
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
}

void dragon::setRect()
{
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_atk = RectMakeCenter(_x - 112, _y + 56, 96, 56);
	_coll = RectMakeCenter(_x + 64, _y, 200, 150);
	_coll2 = RectMakeCenter(_x - 70, _y + 56, 80, 60);
	_aware = RectMakeCenter(_x - _awareW * 0.5, _y, _awareW, _awareH);
}
