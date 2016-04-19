#include "stdafx.h"
#include "player.h"
#include "objectManager.h"


player::player()
{
}


player::~player()
{
}

HRESULT player::init()
{
	_time = _count = 0;
	
	IMAGEMANAGER->addFrameImage("jjangu_idle", "image/player/jjangu_idle.bmp", 162, 162, 3, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_run", "image/player/jjangu_run.bmp", 432, 158, 6, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_attack", "image/player/jjangu_attack.bmp", 240, 160, 3, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_damage", "image/player/jjangu_damage.bmp", 207, 160, 3, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_dead", "image/player/jjangu_dead.bmp", 570, 160, 5, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_skill1", "image/player/jjangu_skill1.bmp", 600, 160, 6, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_skill2", "image/player/jjangu_skill2.bmp", 300, 160, 3, 2, true, 0xff00ff);
	IMAGEMANAGER->addImage("shadow", "image/effect/shadow.bmp", 52, 31, true, 0xff00ff);

	_player.charater = IMAGEMANAGER->findImage("jjangu_idle");
	_player.shadow = IMAGEMANAGER->findImage("shadow");
	_player.state = IDLE;
	_player.isRight = true;
	_player.pt.x = CENTERX/3;
	_player.pt.y = CENTERY;
	_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, _player.charater->getFrameWidth(), _player.charater->getFrameHeight());
	_player.maxHp = _player.curHp = 1000;
	_player.maxMp = _player.curMp = 1000;

	IMAGEMANAGER->addImage("bgBar", "image/ui/gauge_bg.bmp", 162, 44, true, 0xff00ff);
	IMAGEMANAGER->addImage("hpBar", "image/ui/gauge_hp.bmp", 162, 44, true, 0xff00ff);
	IMAGEMANAGER->addImage("mpBar", "image/ui/gauge_mp.bmp", 162, 44, true, 0xff00ff);

	_hpBar = new progressBar;
	_hpBar->init("bgBar", "hpBar", _player.maxHp, _player.curHp);
	//_hpBar->setBar(0, 5, 250, 50);
	_hpBar->setBar(35, -15, 250, 50);

	_mpBar = new progressBar;
	_mpBar->init("bgBar", "mpBar", _player.maxMp, _player.curMp);
	//_mpBar->setBar(0, 45, 250, 50);
	_mpBar->setBar(35, 25, 250, 50);

	IMAGEMANAGER->addImage("hpText", "image/ui/txt_hp.bmp", 90, 40, true, 0xff00ff);
	IMAGEMANAGER->addImage("mpText", "image/ui/txt_mp.bmp", 90, 40, true, 0xff00ff);

	return S_OK;
}

HRESULT player::init(int x, int y)
{
	return S_OK;
}

void player::release()
{
	SAFE_DELETE(_hpBar);
	SAFE_DELETE(_mpBar);
}

void player::update()
{
	if (KEYMANAGER->isOnceKeyDown('G'))
	{
		_player.curHp = 50;
		_hpBar->decreaseBar(950);
		_player.curMp = 20;
		_mpBar->decreaseBar(980);
	}
	if(KEYMANAGER->isOnceKeyDown('H'))
	{
		_player.curHp = _player.maxHp;
		_hpBar->increaseBar(1000);
		_player.curMp = _player.maxMp;
		_mpBar->increaseBar(1000);
	}

	_time += TIMEMANAGER->getElapsedTime();

	if (_player.state == IDLE || _player.state == RUN)
	{
		if (_time > 1)
		{
			_time = 0;
			_player.curMp += 5;
			_mpBar->increaseBar(5);
			if (_player.curMp > _player.maxMp)
			{
				_player.curMp = _player.maxMp;
			}
		}
	}

	move();
	attack();
	setImage();

	_hpBar->update();
	_mpBar->update();
}

void player::render()
{
	_hpBar->render();
	_mpBar->render();
	
	IMAGEMANAGER->findImage("hpText")->render(getMemDC(), 0, 5, 40, 20, 0, 0, 90, 40);
	IMAGEMANAGER->findImage("mpText")->render(getMemDC(), 0, 45, 40, 20, 0, 0, 90, 40);
	
	if(_player.isRight)
		_player.shadow->alphaRender(getMemDC(), _player.pt.x - _player.charater->getFrameWidth() / 2, _player.pt.y + _player.charater->getFrameHeight()/3, 128);
	else
	{
		if (_player.state == IDLE)
			_player.shadow->alphaRender(getMemDC(), _player.pt.x - _player.charater->getFrameWidth() / 2, _player.pt.y + _player.charater->getFrameHeight() / 3, 128);
		else
			_player.shadow->alphaRender(getMemDC(), _player.pt.x - _player.charater->getFrameWidth() / 4, _player.pt.y + _player.charater->getFrameHeight() / 3, 128);
	}
	
	_player.charater->frameRender(getMemDC(), _player.coll.left, _player.coll.top, _player.charater->getFrameX(), _player.charater->getFrameY());

	//char str[128];
	//sprintf_s(str, "mp : %d", _player.curMp);
	//TextOut(getMemDC(), 0, 130, str, strlen(str));
}

void player::move()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (_player.coll.left > 0 
			&& _player.state != DAMAGE && _player.state != DEAD)
		{
			if(_player.state != SKILL1)
				_player.state = RUN;

			if (_player.isRight)
				_player.isRight = false;
			_player.pt.x -= 5;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (_player.coll.right <= WINSIZEX 
			&& _player.state != DAMAGE && _player.state != DEAD)
		{
			if (_player.state != SKILL1)
				_player.state = RUN;
			if (!_player.isRight)
				_player.isRight = true;
			_player.pt.x += 5;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		if (_player.coll.bottom > CENTERY
			/*&& _player.state != DAMAGE*/ && _player.state != DEAD)
		{
			if (_player.state != SKILL1)
				_player.state = RUN;
			_player.pt.y -= 5;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (_player.coll.bottom < WINSIZEY
			/*&& _player.state != DAMAGE*/ && _player.state != DEAD)
		{
			if (_player.state != SKILL1)
				_player.state = RUN;
			_player.pt.y += 5;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		if (_player.state != DEAD)
			_player.state = IDLE;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		if (_player.state != DEAD)
			_player.state = IDLE;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		if (_player.state != DEAD)
			_player.state = IDLE;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		if (_player.state != DEAD)
			_player.state = IDLE;
	}

	_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, _player.charater->getFrameWidth(), _player.charater->getFrameHeight());
}

void player::attack()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (_player.state == IDLE)
		{
			_player.state = ATTACK;
			if (_player.isRight)
			{
				_curFrameX = 0;
			}
			else if (!_player.isRight)
			{
				_curFrameX = 2;
			}
			int width = IMAGEMANAGER->findImage("jjangu_attack")->getFrameWidth();
			int height = IMAGEMANAGER->findImage("jjangu_attack")->getFrameHeight();
			_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, width, height);
			collision();
		}
	}

	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		if (_player.state == IDLE)
		{
			if (_player.curMp >= 50)
			{
				_player.curMp -= 50;
				_mpBar->decreaseBar(50);
				skill(1);
			}
		}
	}
	
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		if (_player.state == IDLE)
		{
			if (_player.curMp >= 80)
			{
				_player.curMp -= 80;
				_mpBar->decreaseBar(80);
				skill(2);
			}
		}
	}

	if (_player.state == SKILL1)
	{
		collision();
	}
}

void player::damage(int damage)
{
	_player.state = DAMAGE;
	if (_player.isRight)
	{
		_curFrameX = 0;
	}
	else if (!_player.isRight)
	{
		_curFrameX = IMAGEMANAGER->findImage("jjangu_damage")->getMaxFrameX();
	}
	_player.curHp -= damage;
	_hpBar->decreaseBar(damage);
	if (_player.curHp <= 0)
	{
		_player.curHp = 0;
		dead();
	}
}

void player::recover(int recovery, int type)
{
	if (type == 1)
	{
		_player.curHp += recovery;
		_hpBar->increaseBar(recovery);
		if (_player.curHp > _player.maxHp)
		{
			_player.curHp = _player.maxHp;
		}
	}
	else
	{
		_player.curMp += recovery;
		_mpBar->increaseBar(recovery);
		if (_player.curMp > _player.maxMp)
		{
			_player.curMp = _player.maxMp;
		}
	}
}

void player::dead()
{
	_player.state = DEAD;
	_isDead = true;
}

void player::skill(int skillNum)
{
	int width;
	int height;

	switch (skillNum)
	{
	case 1:
	{
		_player.state = SKILL1;
		if (_player.isRight)
		{
			_curFrameX = 0;
		}
		else if (!_player.isRight)
		{
			_curFrameX = 5;
		}
		width = IMAGEMANAGER->findImage("jjangu_skill1")->getFrameWidth();
		height = IMAGEMANAGER->findImage("jjangu_skill1")->getFrameHeight();
	}
	break;
	case 2:
	{
		_player.state = SKILL2;
		if (_player.isRight)
		{
			_curFrameX = 0;
		}
		else if (!_player.isRight)
		{
			_curFrameX = 2;
		}
		width = IMAGEMANAGER->findImage("jjangu_skill2")->getFrameWidth();
		height = IMAGEMANAGER->findImage("jjangu_skill2")->getFrameHeight();
	}
	break;
	}
	_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, width, height);
	collision();
}

void player::collision()
{
	for (int i = 0; i < _enemy->getVObject().size(); ++i)
	{
		if (_enemy->getVObject()[i]->getType() == PLAYER) continue;
		if (_enemy->getVObject()[i]->isDead()) continue;

		if (IntersectRect(&RectMake(0, 0, 0, 0), &_player.coll, &_enemy->getVObject()[i]->getRect()))
		{
			if (_player.coll.bottom <= _enemy->getVObject()[i]->getRect().bottom + 10
				&& _player.coll.bottom >= _enemy->getVObject()[i]->getRect().bottom - 10
				&& _enemy->getVObject()[i]->getState() != DEAD)
			{
				if (_player.state != SKILL1)
				{
					if (_player.isRight != _enemy->getVObject()[i]->isRight())
						_enemy->getVObject()[i]->damage(50);
					else
						_enemy->getVObject()[i]->damage(80);
				}
				if (_player.state == SKILL1)
				{
					_enemy->getVObject()[i]->damage(1);
				}
				if (_player.state == SKILL2)
				{
					_enemy->getVObject()[i]->damage(100);
				}
			}
		}
	}
}

void player::setFrame()
{
	if (_player.isRight)
	{
		_curFrameY = 0;
		_player.charater->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX++;
			if (_curFrameX > _player.charater->getMaxFrameX())
			{
				if (_player.state == ATTACK || _player.state == DAMAGE
					|| _player.state == SKILL1 || _player.state == SKILL2)
				{
					_player.state = IDLE;
					return;
				}
				else if (_player.state == DEAD)
				{
					_curFrameX = _player.charater->getMaxFrameX();
					return;
				}
				else
				{
					_curFrameX = 0;
				}
			}
			_player.charater->setFrameX(_curFrameX);
		}
	}
	else
	{
		_curFrameY = 1;
		_player.charater->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX--;
			if (_curFrameX < 0)
			{
				if (_player.state == ATTACK || _player.state == DAMAGE
					|| _player.state == SKILL1 || _player.state == SKILL2)
				{
					_player.state = IDLE;
					return;
				}
				else if (_player.state == DEAD)
				{
					_curFrameX = 0;
					return;
				}
				else
				{
					_curFrameX = _player.charater->getMaxFrameX();
				}
			}
			_player.charater->setFrameX(_curFrameX);
		}
	}
}

void player::setImage()
{
	switch (_player.state)
	{
	case IDLE:
		_player.charater = IMAGEMANAGER->findImage("jjangu_idle");
		setFrame();
		break;
	case RUN:
		_player.charater = IMAGEMANAGER->findImage("jjangu_run");
		setFrame();
		break;
	case ATTACK:
		_player.charater = IMAGEMANAGER->findImage("jjangu_attack");	//findImage 하면 imageinfo 초기화된다.
		_player.charater->setFrameX(_curFrameX);
		setFrame();
		break;
	case SKILL1:
		_player.charater = IMAGEMANAGER->findImage("jjangu_skill1");
		_player.charater->setFrameX(_curFrameX);
		setFrame();
		break;
	case SKILL2:
		_player.charater = IMAGEMANAGER->findImage("jjangu_skill2");
		_player.charater->setFrameX(_curFrameX);
		setFrame();
		break;
	case DAMAGE:
		_player.charater = IMAGEMANAGER->findImage("jjangu_damage");
		_player.charater->setFrameX(_curFrameX);
		setFrame();
		break;
	case DEAD:
		_player.charater = IMAGEMANAGER->findImage("jjangu_dead");
		setFrame();
		break;
	}
}
