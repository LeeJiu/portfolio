#include "stdafx.h"
#include "player.h"
#include "stageMap.h"
#include "objectManager.h"
#include "enemyManager.h"


player::player()
{
}


player::~player()
{
}

HRESULT player::init()
{
	IMAGEMANAGER->addFrameImage("shovel_idle", "image/player/shovel_idle.bmp", 68, 124, 1, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("shovel_walk", "image/player/shovel_walk.bmp", 552, 136, 6, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("shovel_dig", "image/player/shovel_dig.bmp", 660, 140, 5, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("shovel_digdown", "image/player/shovel_digdown.bmp", 48, 140, 1, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("shovel_jumpup", "image/player/shovel_jumpup.bmp", 64, 136, 1, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("shovel_jumpdown", "image/player/shovel_jumpdown.bmp", 68, 136, 1, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("shovel_climb", "image/player/shovel_climb.bmp", 100, 64, 2, 1, true, 0xff00ff);

	_image = IMAGEMANAGER->findImage("shovel_idle");
	_x = _sourX + 32;
	_y = _sourY + (WINSIZEY / 2);
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_coll = RectMakeCenter(_x, _y, 32, 64);

	_gravity = 0.8f, _jumpPwr = 0.0f;

	_isRight = true;

	_curFrameX = _curFrameY = _count = 0;

	return S_OK;
}

void player::release()
{
}

void player::update()
{
	keyControl();
	move();

	setImage();
}

void player::render()
{
	Rectangle(getMemDC(), _coll.left, _coll.top, _coll.right, _coll.bottom);

	if (_state == PS_DIG)
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top + 6, _curFrameX, _curFrameY);
	}
	else
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top, _curFrameX, _curFrameY);
	}

	char str[128];
	sprintf_s(str, "on ladder? %d, gravity? %f, jump? %f", 
		(int)_onLadder, _gravity, _jumpPwr);
	TextOut(getMemDC(), 10, 200, str, strlen(str));
}

void player::keyControl()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (_isBounce)
			return;

		if (_state != PS_JUMPUP && _state != PS_JUMPDOWN && _state != PS_DIGDOWN && _state != PS_DIG)
			_state = PS_WALK;
		else if (_state == PS_DIG)
			return;

		_isRight = false;
		_curFrameY = 1;
		_x -= 3;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (_isBounce)
			return;

		if (_state != PS_JUMPUP && _state != PS_JUMPDOWN && _state != PS_DIGDOWN && _state != PS_DIG)
			_state = PS_WALK;
		else if (_state == PS_DIG)
			return;

		_isRight = true;
		_curFrameY = 0;
		_x += 3;
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_isClimb = true;
		if (_onLadder)
		{
			if (_state != PS_CLIMB)
			{
				_state = PS_CLIMB;
				_curFrameX = 0;
				_curFrameY = 0;
				_gravity = 0.f;
				_jumpPwr = 0.f;
				_x = _climbX;
			}
			_y -= 2;

			//사다리 오르내리기 프레임
			if (_count % 10 == 0)
			{
				_curFrameX++;
				if (_curFrameX > _image->getMaxFrameX())
					_curFrameX = 0;
			}
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (_state == PS_JUMPUP || _state == PS_JUMPDOWN)
		{
			_state = PS_DIGDOWN;
			_curFrameX = 0;
			if(_state != PS_CLIMB && _state != PS_IDLE)
				_jumpPwr *= 0.5;
		}

		_isClimb = true;
		if (_onLadder)
		{
			if (_state != PS_CLIMB)
			{
				_state = PS_CLIMB;
				_curFrameX = 0;
				_curFrameY = 0;
				_gravity = 0.f;
				_jumpPwr = 0.f;
				_x = _climbX;
			}
			_y += 2;

			//사다리 오르내리기 프레임
			if (_count % 10 == 0)
			{
				_curFrameX++;
				if (_curFrameX > _image->getMaxFrameX())
					_curFrameX = 0;
			}
		}
	}
	
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (_state != PS_JUMPUP && _state != PS_JUMPDOWN && _state != PS_DIGDOWN && _state != PS_CLIMB)
		{
			_state = PS_JUMPUP;
			_curFrameX = 0;
			_jumpPwr = 15.f;
		}
	}
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		if (_state != PS_DIG)
		{
			_state = PS_DIG;
			_curFrameX = 0;
		}
	}


	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		if (_state != PS_JUMPUP && _state != PS_JUMPDOWN && _state != PS_DIGDOWN)
			_state = PS_IDLE;
		_curFrameX = 0;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		if (_state != PS_JUMPUP && _state != PS_JUMPDOWN && _state != PS_DIGDOWN)
			_state = PS_IDLE;
		_curFrameX = 0;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		_isClimb = false;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		_isClimb = false;
	}

	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_coll = RectMakeCenter(_x, _y, 32, 64);
}

void player::move()
{
	bounce();

	if (!_onLadder)
	{
		_y -= _jumpPwr;
		_jumpPwr -= _gravity;
	}


	if (_state == PS_JUMPUP && _jumpPwr < 0)
		_state = PS_JUMPDOWN;

	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_coll = RectMakeCenter(_x, _y, 32, 64);

	tileCollision();
	objCollision();
	enemyCollision();
}

void player::bounce()
{
	if (_isBounce)
	{
		if (_state != PS_DIG && _state != PS_DIGDOWN)
		{
			if (_isRight)
			{
				_x -= 5;
			}
			else
			{
				_x += 5;
			}
		}

		_bounce -= 5;
		if (_bounce <= 0)
		{
			_isBounce = false;
		}
	}
}

void player::tileCollision()
{
	for (int y = _sourY / TILESIZE; y < (_sourY + WINSIZEY) / TILESIZE; ++y)
	{
		for (int x = _sourX / TILESIZE; x < (_sourX + WINSIZEX) / TILESIZE; ++x)
		{
			if (x < 0 || x > TILENUMX) break;
			if (y < 0 || y > TILENUMY) break;

			if (_map->getTile()[y][x].type != CRASHABLE
				&& _map->getTile()[y][x].type != BREAKABLE
				&& _map->getTile()[y][x].type != LADDER)
				continue;

			RECT temp;
			if (_map->getTile()[y][x].type == LADDER)
			{
				if (IntersectRect(&temp, &_map->getTile()[y][x].rc, &_coll))
				{
					_onLadder = true;
					_climbX = (_map->getTile()[y][x].rc.left + _map->getTile()[y][x].rc.right) / 2;
					if (_isClimb || _state == PS_CLIMB)
					{
						_gravity = 0.f;
						_jumpPwr = 0.f;
						return;
					}
				}
			}
			else
			{
				if (IntersectRect(&temp, &_map->getTile()[y][x].rc, &_coll))
				{
					int width = temp.right - temp.left;
					int height = temp.bottom - temp.top;

					if (_state == PS_JUMPUP || _state == PS_JUMPDOWN || _state == PS_DIGDOWN || _state == PS_CLIMB)
					{
						_state = PS_IDLE;
						_curFrameX = 0;
					}
					_jumpPwr = 0.f;

					if (temp.top == _map->getTile()[y][x].rc.top)
					{
						_y -= height;
					}
					else if (temp.bottom == _map->getTile()[y][x].rc.bottom)
					{
						_y += height;
						_state = PS_JUMPDOWN;
					}

					_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
					_coll = RectMakeCenter(_x, _y, 32, 64);
					return;
				}
			}
		}
	}

	_onLadder = false;
	_gravity = 0.8f;
}

void player::objCollision()
{
	int size = _objMgr->getVObject().size();
	for (int i = 0; i < size; ++i)
	{
		if (_objMgr->getVObject()[i]->getX() < _sourX
			|| _objMgr->getVObject()[i]->getX() > _sourX + WINSIZEX) continue;

		OBJECTTYPE type = _objMgr->getVObject()[i]->getType();
		RECT cmpRect = _objMgr->getVObject()[i]->getRect();

		switch (type)
		{
		case BLOCK_SMALL: case BLOCK_BIG:
			block();
			break;
		case CHECKPOINT01: case CHECKPOINT02:
			checkpoint();
			break;
		case CHEST:
			chest();
			break;
		case AWL:
			awl();
			break;
		case PILE01: case PILE02: case PLATTER:
			diggableObject();
			break;
		case MOVINGTILE01: case MOVINGTILE02:
			movingTile();
			break;
		}

		RECT temp;
		if (IntersectRect(&temp, &cmpRect, &_coll))
		{
			_onLadder = false;
			if (_state == PS_CLIMB)
				_state = PS_IDLE;
			_gravity = 0.8f;

			int width = temp.right - temp.left;
			int height = temp.bottom - temp.top;

			//위에서 충돌
			if (width > height)
			{
				if (_state == PS_JUMPUP || _state == PS_JUMPDOWN || _state == PS_DIGDOWN)
					_state = PS_IDLE;
				_jumpPwr = 0.f;
				_y -= height;
			}
			//옆에서 충돌
			else
			{
				//왼쪽
				if (temp.left == cmpRect.left)
				{
					_x -= width;
				}
				//오른쪽
				else if (temp.right == cmpRect.right)
				{
					_x += width;
				}
			}
			_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
			_coll = RectMakeCenter(_x, _y, 32, 64);			

			return;
		}
	}
}

void player::enemyCollision()
{
	if (_state == PS_DIG || _state == PS_DIGDOWN)
	{
		int size = _enemyMgr->getVEnemy().size();
		for (int i = 0; i < size; ++i)
		{
			if (_enemyMgr->getVEnemy()[i]->getX() < _sourX
				|| _enemyMgr->getVEnemy()[i]->getX() > _sourX + WINSIZEX)
				continue;
			if (_enemyMgr->getVEnemy()[i]->getY() < _sourY
				|| _enemyMgr->getVEnemy()[i]->getY() > _sourY + WINSIZEY)
				continue;
			if (_enemyMgr->getVEnemy()[i]->getIsDead())
				continue;

			RECT temp, cmpRect;
			cmpRect = _enemyMgr->getVEnemy()[i]->getRect();
			if (IntersectRect(&temp, &cmpRect, &_rc))
			{
				_enemyMgr->getVEnemy()[i]->setIsDead(true);
				_isBounce = true;
				_jumpPwr = 13.f;
				_bounce = 96;
				break;
			}
		}
	}
	else
	{
		int size = _enemyMgr->getVEnemy().size();
		for (int i = 0; i < size; ++i)
		{
			if (_enemyMgr->getVEnemy()[i]->getX() < _sourX
				|| _enemyMgr->getVEnemy()[i]->getX() > _sourX + WINSIZEX)
				continue;
			if (_enemyMgr->getVEnemy()[i]->getY() < _sourY
				|| _enemyMgr->getVEnemy()[i]->getY() > _sourY + WINSIZEY)
				continue;
			if (_enemyMgr->getVEnemy()[i]->getIsDead())
				continue;

			RECT temp, cmpRect;
			cmpRect = _enemyMgr->getVEnemy()[i]->getRect();
			if (IntersectRect(&temp, &cmpRect, &_coll))
			{
				_isBounce = true;
				_jumpPwr = 8.f;
				_bounce = 96;
				break;
			}
		}
	}
}

void player::setImage()
{
	switch (_state)
	{
	case PS_IDLE:
		_image = IMAGEMANAGER->findImage("shovel_idle");
		_image->setFrameY(_curFrameY);
		break;
	case PS_WALK:
		_image = IMAGEMANAGER->findImage("shovel_walk");
		_image->setFrameY(_curFrameY);
		break;
	case PS_JUMPUP:
		_image = IMAGEMANAGER->findImage("shovel_jumpup");
		_image->setFrameY(_curFrameY);
		break;
	case PS_JUMPDOWN:
		_image = IMAGEMANAGER->findImage("shovel_jumpdown");
		_image->setFrameY(_curFrameY);
		break;
	case PS_CLIMB:
		_image = IMAGEMANAGER->findImage("shovel_climb");
		_image->setFrameY(_curFrameY);
		break;
	case PS_DIG:
		_image = IMAGEMANAGER->findImage("shovel_dig");
		_image->setFrameY(_curFrameY);
		break;
	case PS_DIGDOWN:
		_image = IMAGEMANAGER->findImage("shovel_digdown");
		_image->setFrameY(_curFrameY);
		break;
	}

	//이미지 사이즈로 갱신
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_coll = RectMakeCenter(_x, _y, 32, 64);

	setFrame();
}

void player::setFrame()
{
	_count++;

	if (_count % 10 == 0)
	{
		if(_state != PS_CLIMB)
			_curFrameX++;
		if (_curFrameX > _image->getMaxFrameX())
		{
			_curFrameX = 0;
			if (_state == PS_DIG)
				_state = PS_IDLE;
		}
		_image->setFrameX(_curFrameX);
	}
}
