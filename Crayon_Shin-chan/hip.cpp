#include "stdafx.h"
#include "hip.h"
#include "objectManager.h"

hip::hip()
{
}


hip::~hip()
{
}


HRESULT hip::init(int x, int y)
{
	IMAGEMANAGER->addFrameImage("hip_attack", "image/enemy/hip_attack.bmp", 340, 200, 4, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("hip_run", "image/enemy/hip_run.bmp", 378, 160, 6, 2, true, 0xff00ff);

	_enemy.charater = new image;
	_enemy.charater->init("image/enemy/hip_idle.bmp", 100, 160, 2, 2, true, 0xff00ff);

	_enemy.state = IDLE;
	_enemy.type = HIP;
	_enemy.isRight = false;
	_enemy.pt.x = x;
	_enemy.pt.y = y;
	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, _enemy.charater->getFrameWidth(), _enemy.charater->getFrameHeight());
	_enemy.curHp = _enemy.maxHp = 3000;
	_isDead = false;

	IMAGEMANAGER->addImage("enemy_bg", "image/ui/enemy_gauge_bg.bmp", 54, 9, true, 0xff00ff);
	IMAGEMANAGER->addImage("enemy_hp", "image/ui/enemy_gauge_hp.bmp", 54, 9, true, 0xff00ff);

	_hpBar = new progressBar;
	_hpBar->init("enemy_bg", "enemy_hp", _enemy.maxHp, _enemy.curHp);
	_hpBar->setBar(_enemy.coll.left, _enemy.coll.top - 5, _enemy.charater->getFrameWidth(), 5);

	IMAGEMANAGER->addFrameImage("fireball", "image/effect/fireball.bmp", 180, 80, 2, 2, true, 0xff00ff);

	_fireball.fireball = IMAGEMANAGER->findImage("fireball");
	_fireball.fire = false;
	_fireball.isRight = _enemy.isRight;
	_fireball.state = IDLE;

	_range = 150.f;
	_range2 = 200.f;

	return S_OK;
}

void hip::release()
{
	SAFE_DELETE(_hpBar);
	//SAFE_DELETE(_fireball.fireball);
}

void hip::update()
{
	if (KEYMANAGER->isOnceKeyDown('Y'))
		_test = !_test;

	_time += TIMEMANAGER->getElapsedTime();

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
		if (_enemy.state == RUN)
			attack();
		else if (_enemy.state == ATTACK)
			fire();

		if (_fireball.fire)
			moveFire();
	}
	setImage();
}

void hip::render()
{
	//Rectangle(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.coll.right, _enemy.coll.bottom);
	_enemy.charater->frameRender(getMemDC(), _enemy.coll.left, _enemy.coll.top, _enemy.charater->getFrameX(), _enemy.charater->getFrameY());
	_hpBar->render();
	if(_fireball.fire)
		_fireball.fireball->frameRender(getMemDC(), _fireball.coll.left, _fireball.coll.top, 
		_fireball.fireball->getFrameX(), _fireball.fireball->getFrameY());
}

void hip::move()
{
	float distance = MY_UTIL::getDistance(_enemy.pt.x, _enemy.pt.y, _playerX, _playerY);
	if (distance < _range && _enemy.state != DAMAGE)
	{
		_enemy.state = RUN;

		//때리지 못했으면
		if (!_hit)
		{
			//플레이어 쪽으로 이동
			if (_enemy.pt.x > _playerX + 40)
			{
				if (_enemy.isRight)
					_enemy.isRight = false;
				_enemy.pt.x -= 3;
			}
			else if (_enemy.pt.x < _playerX - 40)
			{
				if (!_enemy.isRight)
					_enemy.isRight = true;
				_enemy.pt.x += 3;
			}
			else if (_enemy.pt.y > _playerY + 2)
			{
				_enemy.pt.y -= 3;
			}
			else if (_enemy.pt.y < _playerY - 2)
			{
				_enemy.pt.y += 3;
			}
		}
		//때렸으면
		else
		{
			//플레이어 반대쪽으로 이동
			if (_enemy.pt.x > _playerX)
			{
				if (!_enemy.isRight)
					_enemy.isRight = true;
				if(_enemy.coll.right < WINSIZEX)
					_enemy.pt.x += 3;
			}
			else if (_enemy.pt.x < _playerX)
			{
				if (_enemy.isRight)
					_enemy.isRight = false;
				if (_enemy.coll.left > 0)
					_enemy.pt.x -= 3;
			}
			else if (_enemy.pt.y > _playerY)
			{
				if(_enemy.coll.top < WINSIZEY)
					_enemy.pt.y += 3;
			}
			else if (_enemy.pt.y < _playerY)
			{
				if(_enemy.coll.bottom > 0)
					_enemy.pt.y -= 3;
			}
		}
	}
	else if (distance > _range && distance < _range2)
	{
		_enemy.state = IDLE;
		_hit = false;
	}
	else if (distance > _range2)
	{
		if (_time > 3)
		{
			_time = 0;
			_enemy.state = ATTACK;
			if (_enemy.pt.x > _playerX)
			{
				_enemy.isRight = false;
				_curFrameX = 3;
			}
			else if (_enemy.pt.x < _playerX)
			{
				_enemy.isRight = true;
				_curFrameX = 0;
			}

			int width = IMAGEMANAGER->findImage("hip_attack")->getFrameWidth();
			int height = IMAGEMANAGER->findImage("hip_attack")->getFrameHeight();
		}
		else
		{
			_enemy.state = RUN;
			//플레이어 y축으로 이동
			if (_enemy.pt.y > _playerY + 2)
			{
				_enemy.pt.y -= 3;
			}
			else if (_enemy.pt.y < _playerY - 2)
			{
				_enemy.pt.y += 3;
			}
			else
			{
				_enemy.state = IDLE;
			}
		}
	}

	_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, 
		_enemy.charater->getFrameWidth(), _enemy.charater->getFrameHeight());
}

void hip::attack()
{
	if (_enemy.pt.x < _playerX + 50 && _enemy.pt.x > _playerX - 50
		&& _enemy.pt.y < _playerY + 5 && _enemy.pt.y > _playerY - 5
		&& _enemy.state == RUN)
	{
		if (_enemy.pt.x > _playerX)
		{
			_enemy.isRight = false;
			_curFrameX = 5;
		}
		else if (_enemy.pt.x < _playerX)
		{
			_enemy.isRight = true;
			_curFrameX = 0;
		}

		int width = IMAGEMANAGER->findImage("hip_run")->getFrameWidth();
		int height = IMAGEMANAGER->findImage("hip_run")->getFrameHeight();
		_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, width, height);
		collision();
	}
}

void hip::fire()
{
	if (_enemy.state == ATTACK && _fireball.fire == false)
	{
		//적의 어택 프레임 세팅
		if (_enemy.pt.x > _playerX)
		{
			_enemy.isRight = false;
			_curFrameX = 3;
		}
		else if (_enemy.pt.x < _playerX)
		{
			_enemy.isRight = true;
			_curFrameX = 0;
		}

		int width = IMAGEMANAGER->findImage("hip_attack")->getFrameWidth();
		int height = IMAGEMANAGER->findImage("hip_attack")->getFrameHeight();
		_enemy.coll = RectMakeCenter(_enemy.pt.x, _enemy.pt.y, width, height);
		
		//적의 파이어볼 세팅
		_fireball.fire = true;
		if (_enemy.isRight)
		{
			_fireball.pt.x = _enemy.coll.right - _fireball.fireball->getFrameWidth()/2;
			_fireball.pt.y = _enemy.coll.top;
			_fireball.isRight = true;
			_fireball.fireball->setFrameY(0);
		}
		else
		{
			_fireball.pt.x = _enemy.coll.left - _fireball.fireball->getFrameWidth()/2;
			_fireball.pt.y = _enemy.coll.top;
			_fireball.isRight = false;
			_fireball.fireball->setFrameY(1);
		}
		_fireball.coll = RectMake(_fireball.pt.x, _fireball.pt.y, 
			_fireball.fireball->getFrameWidth(), _fireball.fireball->getFrameHeight());
	}
}

void hip::moveFire()
{
	if (_fireball.isRight)
	{
		_fireball.pt.x += 3;
		if (_fireball.pt.x + _fireball.fireball->getFrameWidth() > WINSIZEX)
			_fireball.fire = false;
		if (_count % 5 == 0)
		{
			int frameX = _fireball.fireball->getFrameX();
			frameX++;
			if (frameX > _fireball.fireball->getMaxFrameX()) frameX = 0;
			_fireball.fireball->setFrameX(frameX);
		}
	}
	else
	{
		_fireball.pt.x -= 3;
		if (_fireball.pt.x < 0)
			_fireball.fire = false;
		if (_count % 5 == 0)
		{
			int frameX = _fireball.fireball->getFrameX();
			frameX--;
			if (frameX < 0) frameX = _fireball.fireball->getMaxFrameX();
			_fireball.fireball->setFrameX(frameX);
		}
	}
	_fireball.coll = RectMake(_fireball.pt.x, _fireball.pt.y, 
		_fireball.fireball->getFrameWidth(), _fireball.fireball->getFrameHeight());

	collision();
}

void hip::damage(int damage)
{
	_enemy.state = DAMAGE;
	_hit = false;
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

void hip::dead()
{
	_isDead = true;
	_enemy.state = NONE;		//더 이상 프레임을 돌지 않는다.
}

void hip::collision()
{
	if (IntersectRect(&RectMake(0, 0, 0, 0), &_enemy.coll, &_objectMgr->getVObject()[_saveIdx]->getRect()))
	{
		if (_objectMgr->getVObject()[_saveIdx]->getState() != DAMAGE
			&& _objectMgr->getVObject()[_saveIdx]->getState() != DEAD
			&& _hit == false)
		{
			_objectMgr->getVObject()[_saveIdx]->damage(30);
			_hit = true;
		}
	}
	else if (IntersectRect(&RectMake(0, 0, 0, 0), &_fireball.coll, &_objectMgr->getVObject()[_saveIdx]->getRect())
		&& _fireball.fire == true)
	{
		_objectMgr->getVObject()[_saveIdx]->damage(50);
		_fireball.fire = false;
	}
}

void hip::setImage()
{
	switch (_enemy.state)
	{
	case IDLE:
	{
		_enemy.charater->init("image/enemy/hip_idle.bmp", 100, 160, 2, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case RUN:
	{
		_enemy.charater->init("image/enemy/hip_run.bmp", 378, 160, 6, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case ATTACK:
	{
		_enemy.charater->init("image/enemy/hip_attack.bmp", 340, 200, 4, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case DAMAGE:
	{
		_enemy.charater->init("image/enemy/hip_damage.bmp", 300, 160, 4, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case DEAD:
	{
		_enemy.charater->init("image/enemy/hip_dead.bmp", 316, 148, 4, 2, true, 0xff00ff);
		_enemy.charater->setFrameX(_curFrameX);
		setFrame();
	}
	break;
	case NONE:
		return;
	}
}

void hip::setFrame()
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
				if (_enemy.state == ATTACK || _enemy.state == DAMAGE)
				{
					_enemy.state = IDLE;
					_curFrameX = 0;
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
				if (_enemy.state == ATTACK || _enemy.state == DAMAGE)
				{
					_enemy.state = IDLE;
					_curFrameX = 1;
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
