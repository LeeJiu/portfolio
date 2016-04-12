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
	IMAGEMANAGER->addFrameImage("boxboy_idle", "image/enemy/boxboy_idle.bmp", 180, 130, 4, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("boxboy_run", "image/enemy/boxboy_run.bmp", 330, 154, 6, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("boxboy_attack", "image/enemy/boxboy_attack.bmp", 344, 130, 8, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("boxboy_dead", "image/enemy/boxboy_dead.bmp", 660, 200, 4, 2, true, 0xff00ff);

	_enemy.enemy = IMAGEMANAGER->findImage("boxboy_idle");
	_enemy.state = E_IDLE;
	_enemy.isRight = false;
	_enemy.pt.x = x;
	_enemy.pt.y = y;
	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, _enemy.enemy->getFrameWidth(), _enemy.enemy->getFrameHeight());
	_enemy.isDead = false;
	_enemy.curHp = _enemy.maxHp = 200;

	IMAGEMANAGER->addImage("enemy_bg", "image/ui/enemy_gauge_bg.bmp", 54, 9, true, 0xff00ff);
	IMAGEMANAGER->addImage("enemy_hp", "image/ui/enemy_gauge_hp.bmp", 54, 9, true, 0xff00ff);

	_hpBar = new progressBar;
	_hpBar->init("enemy_bg", "enemy_hp", _enemy.maxHp, _enemy.curHp);
	_hpBar->setBar(_enemy.coll.left, _enemy.coll.top - 5, _enemy.enemy->getFrameWidth(), 5);

	_range = 200.f;

	return S_OK;
}

void boxboy::release()
{
	SAFE_DELETE(_hpBar);
}

void boxboy::update()
{
	if (KEYMANAGER->isOnceKeyDown('Y'))
		_test = !_test;

	_hpBar->update();
	_hpBar->setPosition(_enemy.coll.left, _enemy.coll.top - 5);
	if (_enemy.state != E_DEAD && _enemy.state != NONE)
	{
		move();
		attack();
	}
	setImage();
}

void boxboy::render()
{
	if(_test)
		Rectangle(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.coll.right, _enemy.coll.bottom);
	_enemy.enemy->frameRender(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.enemy->getFrameX(), _enemy.enemy->getFrameY());
	_hpBar->render();
}

void boxboy::move()
{
	if (MY_UTIL::getDistance(_enemy.pt.x, _enemy.pt.y, _player->getX(), _player->getY()) < _range)
	{
		if (_enemy.state != E_ATTACK)
			_enemy.state = E_RUN;

		//플레이어 쪽으로 이동
		if (_enemy.pt.x > _player->getX() + 40)
		{
			if (_enemy.isRight)
				_enemy.isRight = false;
			_enemy.pt.x -= 2;
		}
		else if (_enemy.pt.x < _player->getX() - 40)
		{
			if (!_enemy.isRight)
				_enemy.isRight = true;
			_enemy.pt.x += 2;
		}
		else if (_enemy.pt.y > _player->getY() + 2)
		{
			_enemy.pt.y -= 2;
		}
		else if (_enemy.pt.y < _player->getY() - 2)
		{
			_enemy.pt.y += 2;
		}
	}
	else
	{
		_enemy.state = E_IDLE;
	}
	
	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, _enemy.enemy->getFrameWidth(), _enemy.enemy->getFrameHeight());
}

void boxboy::attack()
{
	if (_enemy.pt.x < _player->getX() + 50 && _enemy.pt.x > _player->getX() - 50
		&& _enemy.pt.y < _player->getY() + 5 && _enemy.pt.y > _player->getY() - 5
		&& _enemy.state != E_ATTACK)
	{
		_enemy.state = E_ATTACK;
		if (_enemy.pt.x > _player->getX())
		{
			_enemy.isRight = false;
			_curFrameX = IMAGEMANAGER->findImage("boxboy_attack")->getMaxFrameX();
		}
		else if (_enemy.pt.x < _player->getX())
		{
			_enemy.isRight = true;
			_curFrameX = 0;
		}

		int width = IMAGEMANAGER->findImage("boxboy_attack")->getFrameWidth();
		int height = IMAGEMANAGER->findImage("boxboy_attack")->getFrameHeight();
		_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, width, height);
		collision();
	}
}

void boxboy::damage(int damage)
{
	_enemy.curHp -= damage;
	_hpBar->decreaseBar(damage);
	if (_enemy.curHp <= 0)
	{
		_enemy.curHp = 0;
		_enemy.state = E_DEAD;
		if (_enemy.isRight)
			_curFrameX = 0;
		else
			_curFrameX = 3;
	}
}

void boxboy::dead()
{
	_enemy.isDead = true;
}

void boxboy::collision()
{
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_enemy.coll, &_player->getRect()))
	{
		_player->damage(20);
	}
}

void boxboy::setImage()
{
	switch (_enemy.state)
	{
	case E_IDLE:
	{
		_enemy.enemy = IMAGEMANAGER->findImage("boxboy_idle");
		setFrame();
	}
	break;
	case E_RUN:
	{
		_enemy.enemy = IMAGEMANAGER->findImage("boxboy_run");
		setFrame();
	}
	break;
	case E_ATTACK:
	{
		_enemy.enemy = IMAGEMANAGER->findImage("boxboy_attack");	//findImage 하면 imageinfo 초기화된다.
		_enemy.enemy->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case E_DEAD:
	{
		_enemy.enemy = IMAGEMANAGER->findImage("boxboy_dead");
		_enemy.enemy->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case NONE:
		return;
	}
}

void boxboy::setFrame()
{
	if (_enemy.isRight)
	{
		_curFrameY = 0;
		_enemy.enemy->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX++;
			if (_curFrameX > _enemy.enemy->getMaxFrameX())
			{
				if (_enemy.state == E_ATTACK)
				{
					_enemy.state = E_IDLE;
					return;
				}
				else if (_enemy.state == E_DEAD)
				{
					dead();
					_enemy.state = NONE;		//더 이상 프레임을 돌지 않는다.
					return;
				}
				else
				{
					_curFrameX = 0;
				}
			}
			_enemy.enemy->setFrameX(_curFrameX);
		}
	}
	else
	{
		_curFrameY = 1;
		_enemy.enemy->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX--;
			if (_curFrameX < 0)
			{
				if (_enemy.state == E_ATTACK)
				{
					_enemy.state = E_IDLE;
					return;
				}
				else if (_enemy.state == E_DEAD)
				{
					dead();
					_enemy.state = NONE;
					return;
				}
				else
				{
					_curFrameX = _enemy.enemy->getMaxFrameX();
				}
			}
			_enemy.enemy->setFrameX(_curFrameX);
		}
	}
}
