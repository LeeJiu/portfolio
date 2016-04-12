#include "stdafx.h"
#include "player.h"


player::player()
{
}


player::~player()
{
}

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("jjangu_idle", "image/player/jjangu_idle.bmp", 162, 162, 3, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_run", "image/player/jjangu_run.bmp", 432, 158, 6, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_attack", "image/player/jjangu_attack.bmp", 240, 160, 3, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_damage", "image/player/jjangu_damage.bmp", 207, 160, 3, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("jjangu_dead", "image/player/jjangu_dead.bmp", 570, 160, 5, 2, true, 0xff00ff);
	
	_player.player = IMAGEMANAGER->findImage("jjangu_idle");
	_player.state = P_IDLE;
	_player.isRight = true;
	_player.pt.x = CENTERX/3;
	_player.pt.y = CENTERY;
	_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, _player.player->getFrameWidth(), _player.player->getFrameHeight());

	_count = 0;

	return S_OK;
}

void player::release()
{
}

void player::update()
{
	move();
	attack();
	setImage();
}

void player::render()
{
	Rectangle(getMemDC(), _player.coll.left, _player.coll.top, _player.coll.right, _player.coll.bottom);
	_player.player->frameRender(getMemDC(), _player.coll.left, _player.coll.top, _player.player->getFrameX(), _player.player->getFrameY());
}

void player::move()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (_player.coll.left > 0)
		{
			_player.state = P_RUN;
			if (_player.isRight)
				_player.isRight = false;
			_player.pt.x -= 5;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (_player.coll.right <= WINSIZEX)
		{
			_player.state = P_RUN;
			if (!_player.isRight)
				_player.isRight = true;
			_player.pt.x += 5;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		if (_player.coll.bottom > CENTERY)
		{
			_player.state = P_RUN;
			_player.pt.y -= 5;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (_player.coll.bottom < WINSIZEY)
		{
			_player.state = P_RUN;
			_player.pt.y += 5;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		_player.state = P_IDLE;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		_player.state = P_IDLE;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		_player.state = P_IDLE;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		_player.state = P_IDLE;
	}

	_player.coll = RectMakeCenter(_player.pt.x, _player.pt.y, _player.player->getFrameWidth(), _player.player->getFrameHeight());
}

void player::attack()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (_player.state == P_IDLE)
		{
			_player.state = P_ATTACK;
			if (_player.isRight)
			{
				_curFrameX = 0;
			}
			else if (!_player.isRight)
			{
				_curFrameX = IMAGEMANAGER->findImage("jjangu_attack")->getMaxFrameX();
			}
			collision();
		}
	}
}

void player::damage()
{
}

void player::dead()
{
}

void player::collision()
{

}

void player::setFrame()
{
	if (_player.isRight)
	{
		_curFrameY = 0;
		_player.player->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX++;
			if (_curFrameX > _player.player->getMaxFrameX())
			{
				if (_player.state == P_ATTACK)
				{
					_player.state = P_IDLE;
					return;
				}
				else
				{
					_curFrameX = 0;
				}
			}
			_player.player->setFrameX(_curFrameX);
		}
	}
	else
	{
		_curFrameY = 1;
		_player.player->setFrameY(_curFrameY);
		_count++;
		if (_count % 10 == 0)
		{
			_curFrameX--;
			if (_curFrameX < 0)
			{
				if (_player.state == P_ATTACK)
				{
					_player.state = P_IDLE;
					return;
				}
				else
				{
					_curFrameX = _player.player->getMaxFrameX();
				}
			}
			_player.player->setFrameX(_curFrameX);
		}
	}
}

void player::setImage()
{
	switch (_player.state)
	{
	case P_IDLE:
	{
		_player.player = IMAGEMANAGER->findImage("jjangu_idle");
		setFrame();
	}
	break;
	case P_RUN:
	{
		_player.player = IMAGEMANAGER->findImage("jjangu_run");
		setFrame();
	}
	break;
	case P_ATTACK:
	{
		_player.player = IMAGEMANAGER->findImage("jjangu_attack");	//findImage 하면 imageinfo 초기화된다.
		_player.player->setFrameX(_curFrameX);
		setFrame();
	}
	break;
		//case P_SKILL1:
		//case P_SKILL2:
	/*case P_DAMAGE:
		_player.player = IMAGEMANAGER->findImage("jjangu_damage");
		setFrame();
		break;
	case P_DEAD:
		_player.player = IMAGEMANAGER->findImage("jjangu_dead");
		setFrame();
		break;*/
	}
}
