#include "stdafx.h"
#include "chocobee.h"
#include "objectManager.h"


chocobee::chocobee()
{
}


chocobee::~chocobee()
{
}

HRESULT chocobee::init(int x, int y)
{
	IMAGEMANAGER->addFrameImage("chocobee_run", "image/enemy/chocobee_run.bmp", 308, 120, 4, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("chocobee_attack", "image/enemy/chocobee_attack.bmp", 240, 90, 3, 1, true, 0xff00ff);
	IMAGEMANAGER->addImage("shadow", "image/effect/shadow.bmp", 52, 31, true, 0xff00ff);

	_enemy.charater = new image;
	_enemy.charater->init("image/enemy/chocobee_idle.bmp", 80, 90, 1, 1, true, 0xff00ff);

	_enemy.shadow = IMAGEMANAGER->findImage("shadow");

	_chocobeeState = static_cast<CHOCOBEESTATE>(RND->getInt(3));

	_enemy.state = IDLE;
	_enemy.type = CHOCOBEE;
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

	_range = 250.f;

	return S_OK;
}

void chocobee::release()
{
	SAFE_DELETE(_hpBar);
}

void chocobee::update()
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
		if (_enemy.state == RUN)
		{
			//런 상태로 어택!
			move();
			if (_objectMgr->getVObject()[_saveIdx]->getState() == IDLE)
				attack();
		}
		else if (_enemy.state == IDLE)
		{
			//다가왔을 때 폭발 어택!
			attack();
		}
	}
	setImage();
}

void chocobee::render()
{
	_enemy.shadow->alphaRender(getMemDC(), _enemy.pt.x - _enemy.charater->getFrameWidth() / 3, _enemy.pt.y + _enemy.charater->getFrameHeight() / 4, 128);
	//Rectangle(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.coll.right, _enemy.coll.bottom);
	_enemy.charater->frameRender(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.charater->getFrameX(), _enemy.charater->getFrameY());
	if(_enemy.state == RUN)
		_hpBar->render();
}

void chocobee::move()
{
	if (MY_UTIL::getDistance(_enemy.pt.x, _enemy.pt.y, _playerX, _playerY) < _range)
	{
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
		//플레이어랑 멀어지면 죽는다
		_enemy.state = DEAD;
		if (_enemy.isRight)
			_curFrameX = 0;
		else
			_curFrameX = 3;

	}

	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, _enemy.charater->getFrameWidth(), _enemy.charater->getFrameHeight());
}

void chocobee::attack()
{
	if (_enemy.pt.x < _playerX + 60 && _enemy.pt.x > _playerX - 60
		&& _enemy.pt.y < _playerY + 5 && _enemy.pt.y > _playerY - 5
		&& _enemy.state == RUN)
	{
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

		int width = IMAGEMANAGER->findImage("chocobee_run")->getFrameWidth();
		int height = IMAGEMANAGER->findImage("chocobee_run")->getFrameHeight();
		_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, width, height);
		collision();
	}
	else if(_enemy.pt.x < _playerX + 60 && _enemy.pt.x > _playerX - 60
		&& _enemy.pt.y < _playerY + 5 && _enemy.pt.y > _playerY - 5
		&& _enemy.state == IDLE)
	{
		//idle 상태일 때 플레이어가 어텍하면 터진다
		collision();
	}
}

void chocobee::damage(int damage)
{
	if (_enemy.curHp == _enemy.maxHp)
	{
		switch (_chocobeeState)
		{
		case ITEM_HP:
			_objectMgr->getVObject()[_saveIdx]->recover(50, 1);
			_enemy.curHp = 0;
			dead();
			return;
		case ITEM_MP:
			_objectMgr->getVObject()[_saveIdx]->recover(50, 2);
			_enemy.curHp = 0;
			dead();
			return;
		case ITEM_NONE:
			_enemy.state = ATTACK;
			_curFrameX = 0;
			_curFrameY = 0;
			break;
		}
	}

	_enemy.curHp -= damage;
	_hpBar->decreaseBar(damage);
	if (_enemy.curHp < 0)
	{
		_enemy.curHp = 0;
		_enemy.state = DEAD;
		if (_enemy.isRight)
			_curFrameX = 0;
		else
			_curFrameX = 3;
	}
}

void chocobee::dead()
{
	_isDead = true;
	_enemy.state = NONE;
}

void chocobee::collision()
{
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_enemy.coll, &_objectMgr->getVObject()[_saveIdx]->getRect()))
	{
		if (_enemy.state == RUN)
		{
			if (_objectMgr->getVObject()[_saveIdx]->getState() != DAMAGE
				&& _objectMgr->getVObject()[_saveIdx]->getState() != DEAD)
				_objectMgr->getVObject()[_saveIdx]->damage(20);
		}
		else if (_enemy.state == IDLE)
		{
			if (_objectMgr->getVObject()[_saveIdx]->getState() == ATTACK)
			{
				_enemy.state = ATTACK;
				_curFrameX = 0;
				_curFrameY = 0;
				int width = IMAGEMANAGER->findImage("chocobee_attack")->getFrameWidth();
				int height = IMAGEMANAGER->findImage("chocobee_attack")->getFrameHeight();
				_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, width, height);
				_objectMgr->getVObject()[_saveIdx]->damage(50);
			}
		}
	}
}

void chocobee::setImage()
{
	switch (_enemy.state)
	{
	case IDLE:
	{
		_enemy.charater->init("image/enemy/chocobee_idle.bmp", 80, 90, 1, 1, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case RUN:
	{
		_enemy.charater->init("image/enemy/chocobee_run.bmp", 308, 120, 4, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		_enemy.charater->setFrameY(_curFrameY);
		setFrame();
	}
	break;
	case ATTACK:
	{
		_enemy.charater->init("image/enemy/chocobee_attack.bmp", 240, 90, 3, 1, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		_enemy.charater->setFrameY(_curFrameY);
		setFrame();
	}
	break;
	case DEAD:
	{
		_enemy.charater->init("image/enemy/chocobee_dead.bmp", 244, 126, 4, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case NONE:
		return;
	}
}

void chocobee::setFrame()
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
					_enemy.state = RUN;
					_curFrameX = 3;
					_curFrameY = 1;
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
					_enemy.state = RUN;
					_curFrameX = 0;
					_curFrameY = 0;
					return;
				}
				else if (_enemy.state == DEAD)
				{
					dead();
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