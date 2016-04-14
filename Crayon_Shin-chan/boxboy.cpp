#include "stdafx.h"
#include "boxboy.h"
#include "objectManager.h"

boxboy::boxboy()
{
}


boxboy::~boxboy()
{
}

HRESULT boxboy::init(int x, int y)
{
	IMAGEMANAGER->addFrameImage("boxboy_attack", "image/enemy/boxboy_attack.bmp", 344, 130, 8, 2, true, 0xff00ff);

	_enemy.charater = new image;
	_enemy.charater->init("image/enemy/boxboy_idle.bmp", 180, 130, 4, 2, true, 0xff00ff);

	_enemy.state = IDLE;
	_enemy.type = BOXBOY;
	_enemy.isRight = false;
	_enemy.pt.x = x;
	_enemy.pt.y = y;
	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, _enemy.charater->getFrameWidth(), _enemy.charater->getFrameHeight());
	_enemy.curHp = _enemy.maxHp = 200;
	_isDead = false;

	IMAGEMANAGER->addImage("enemy_bg", "image/ui/enemy_gauge_bg.bmp", 54, 9, true, 0xff00ff);
	IMAGEMANAGER->addImage("enemy_hp", "image/ui/enemy_gauge_hp.bmp", 54, 9, true, 0xff00ff);

	_hpBar = new progressBar;
	_hpBar->init("enemy_bg", "enemy_hp", _enemy.maxHp, _enemy.curHp);
	_hpBar->setBar(_enemy.coll.left, _enemy.coll.top - 5, _enemy.charater->getFrameWidth(), 5);

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

	//플레이어를 찾아온다.
	for (int i = 0; i < _objectMgr->getVObject().size(); ++i)
	{
		if (_objectMgr->getVObject()[i]->getType() != PLAYER) continue;
		if (_objectMgr->getVObject()[i]->getType() == PLAYER)
		{
			_playerX = _objectMgr->getVObject()[i]->getX();
			_playerY = _objectMgr->getVObject()[i]->getY();
			_saveIdx = i;
			break;
		}
	}

	_hpBar->update();
	_hpBar->setPosition(_enemy.coll.left, _enemy.coll.top - 5);
	if (_enemy.state != DEAD && _enemy.state != NONE)
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
	_enemy.charater->frameRender(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.charater->getFrameX(), _enemy.charater->getFrameY());
	_hpBar->render();
}

void boxboy::move()
{
	if (MY_UTIL::getDistance(_enemy.pt.x, _enemy.pt.y, _playerX, _playerY) < _range)
	{
		if (_enemy.state != ATTACK)
			_enemy.state = RUN;

		//플레이어 쪽으로 이동
		if (_enemy.pt.x > _playerX + 40)
		{
			if (_enemy.isRight)
				_enemy.isRight = false;
			_enemy.pt.x -= 2;
		}
		else if (_enemy.pt.x < _playerX - 40)
		{
			if (!_enemy.isRight)
				_enemy.isRight = true;
			_enemy.pt.x += 2;
		}
		else if (_enemy.pt.y > _playerY + 2)
		{
			_enemy.pt.y -= 2;
		}
		else if (_enemy.pt.y < _playerY - 2)
		{
			_enemy.pt.y += 2;
		}
	}
	else
	{
		_enemy.state = IDLE;
	}
	
	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, _enemy.charater->getFrameWidth(), _enemy.charater->getFrameHeight());
}

void boxboy::attack()
{
	if (_enemy.pt.x < _playerX + 50 && _enemy.pt.x > _playerX - 50
		&& _enemy.pt.y < _playerY + 5 && _enemy.pt.y > _playerY - 5
		&& _enemy.state != ATTACK)
	{
		_enemy.state = ATTACK;
		if (_enemy.pt.x > _playerX)
		{
			_enemy.isRight = false;
			_curFrameX = 7;
		}
		else if (_enemy.pt.x < _playerX)
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
		_enemy.state = DEAD;
		if (_enemy.isRight)
			_curFrameX = 0;
		else
			_curFrameX = 3;
	}
}

void boxboy::dead()
{
	_isDead = true;
	_enemy.state = NONE;		//더 이상 프레임을 돌지 않는다.
}

void boxboy::collision()
{
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_enemy.coll, &_objectMgr->getVObject()[_saveIdx]->getRect()))
	{
		//if (_player->getState() != DAMAGE && _player->getState() != DEAD)
		//	_player->damage(20);
		//else if (_player->getState() == P_DEAD)
		//	_enemy.state = E_IDLE;

		if (_objectMgr->getVObject()[_saveIdx]->getState() != DAMAGE
			&& _objectMgr->getVObject()[_saveIdx]->getState() != DEAD)
			_objectMgr->getVObject()[_saveIdx]->damage(20);
	}
}

void boxboy::setImage()
{
	switch (_enemy.state)
	{
	case IDLE:
	{
		_enemy.charater->init("image/enemy/boxboy_idle.bmp", 180, 130, 4, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case RUN:
	{
		_enemy.charater->init("image/enemy/boxboy_run.bmp", 330, 154, 6, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case ATTACK:
	{
		_enemy.charater->init("image/enemy/boxboy_attack.bmp", 344, 130, 8, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case DEAD:
	{
		_enemy.charater->init("image/enemy/boxboy_dead.bmp", 660, 200, 4, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
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
		_enemy.charater->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX++;
			if (_curFrameX > _enemy.charater->getMaxFrameX())
			{
				if (_enemy.state == ATTACK)
				{
					_enemy.state = IDLE;
					return;
				}
				else if (_enemy.state == DEAD)
				{
					dead();
					return;
				}
				else
				{
					_curFrameX = 0;
				}
			}
			_enemy.charater->setFrameX(_curFrameX);
		}
	}
	else
	{
		_curFrameY = 1;
		_enemy.charater->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX--;
			if (_curFrameX < 0)
			{
				if (_enemy.state == ATTACK)
				{
					_enemy.state = IDLE;
					return;
				}
				else if (_enemy.state == DEAD)
				{
					dead();
					_enemy.state = NONE;
					return;
				}
				else
				{
					_curFrameX = _enemy.charater->getMaxFrameX();
				}
			}
			_enemy.charater->setFrameX(_curFrameX);
		}
	}
}
