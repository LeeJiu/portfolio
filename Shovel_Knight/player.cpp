#include "stdafx.h"
#include "player.h"
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
	IMAGEMANAGER->addFrameImage("shovel_climbstop", "image/player/shovel_climbstop.bmp", 40, 52, 1, 1, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("shovel_damage", "image/player/shovel_damage.bmp", 80, 152, 1, 2, true, 0xff00ff);
	IMAGEMANAGER->findImage("shovel_damage")->alphaInit();


	_image = IMAGEMANAGER->findImage("shovel_idle");
	_state = PS_IDLE;
	_x = _sourX + 32;
	_y = _sourY + (WINSIZEY / 2);
	setRect();

	_gravity = 0.5f, _jumpPwr = 0.0f, _speed = 3.0f, _deadTime = 0.0f;

	_isRight = true;

	_curFrameX = _curFrameY = _count = 0;
	_alpha = 255;

	_life = 8;
	_gold = 100;
	_sp = 30;

	return S_OK;
}

void player::release()
{
}

void player::update()
{
	//죽었으면 업뎃하지 않는다
	if (_isDead)
		return;

	keyControl();

	move();

	setImage();
}

void player::render()
{
	//Rectangle(getMemDC(), _coll.left, _coll.top, _coll.right, _coll.bottom);
	//Rectangle(getMemDC(), _atk.left, _atk.top, _atk.right, _atk.bottom);

	if (_state == PS_DIG)
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top + 6, _curFrameX, _curFrameY);
	}
	else if (_state == PS_DAMAGE || _state == PS_DEAD)
	{
		_image->alphaRender(getMemDC(), _rc.left, _rc.top, 
			_curFrameX * _image->getFrameWidth(), _curFrameY * _image->getFrameHeight(), 
			_image->getFrameWidth(), _image->getFrameHeight(), _alpha);
	}
	else
	{
		_image->frameRender(getMemDC(), _rc.left, _rc.top, _curFrameX, _curFrameY);
	}

	char str[128];
	sprintf_s(str, "gravity? %f", _gravity);
	TextOut(getMemDC(), _sourX + 10, _sourY + 200, str, strlen(str));
	
	sprintf_s(str, "x, y? (%3f, %3f)", _x, _y);
	TextOut(getMemDC(), _sourX + 10, _sourY + 250, str, strlen(str));
}

void player::keyControl()
{
	//이동키
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (_isColl) return;

		if (_state == PS_DAMAGE || _state == PS_DIG) return;

		_isRight = false;	
		if (_state != PS_CLIMB && _state != PS_CLIMBSTOP)
			_curFrameY = 1;

		if (_state == PS_IDLE)
		{
			_state = PS_WALK;
		}

		if (_state == PS_CLIMB || _state == PS_CLIMBSTOP) return;

		_x -= _speed;
		if (_coll.left - _speed < 0)
		{
			_x = 16;	//_coll 의 half size
		}
	}
	else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (_isColl) return;

		if (_state == PS_DAMAGE || _state == PS_DIG) return;

		_isRight = true;
		if (_state != PS_CLIMB && _state != PS_CLIMBSTOP)
			_curFrameY = 0;

		if (_state == PS_IDLE)
		{
			_state = PS_WALK;
		}

		if (_state == PS_CLIMB || _state == PS_CLIMBSTOP) return;

		_x += _speed;
		if (_coll.right + _speed > WINSIZEX * PAGENUMX)
		{
			_x = (WINSIZEX * PAGENUMX) - 16;
		}
	}


	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		//사다리 확인
		ladderCheck();

		if (_centerLadder)
		{
			if (_state != PS_CLIMB)
			{
				_state = PS_CLIMB;
				_curFrameX = _curFrameY = 0;
				_gravity = _jumpPwr = 0.f;
				_x = _climbX;
			}

			_y -= _speed;

			//사다리 오르내리기 프레임
			if (_count % 10 == 0)
			{
				_curFrameX++;
				if (_curFrameX > _image->getMaxFrameX())
					_curFrameX = 0;
			}
		}
		else
		{
			if (_bottomLadder)
			{
				if (_state == PS_CLIMBSTOP)
				{
					_state = PS_IDLE;
					_curFrameX = _curFrameY = 0;
					_gravity = 0.5f;
					_x = _climbX;
					_y -= 32;
				}
				else if (_state == PS_CLIMB)
				{
					_state = PS_CLIMBSTOP;
					_curFrameX = _curFrameY = 0;
					_gravity = _jumpPwr = 0.f;
					_x = _climbX;
				}
			}
		}
	}
	else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (_state == PS_JUMPUP || _state == PS_JUMPDOWN)
		{
			_state = PS_DIGDOWN;
			_curFrameX = 0;
			if (_state != PS_CLIMB && _state != PS_IDLE)
				_jumpPwr *= 0.5;
		}

		//사다리 확인
		ladderCheck();
		
		if (_centerLadder)
		{
			if (_bottomLadder)
			{
				if (_state != PS_CLIMB)
				{
					_state = PS_CLIMB;
					_curFrameX = _curFrameY = 0;
					_gravity = _jumpPwr = 0.f;
					_x = _climbX;
				}

				_y += _speed;

				//사다리 오르내리기 프레임
				if (_count % 10 == 0)
				{
					_curFrameX++;
					if (_curFrameX > _image->getMaxFrameX())
						_curFrameX = 0;
				}
			}
			else
			{
				if (_state == PS_CLIMB)
				{
					_state = PS_JUMPDOWN;
					_curFrameX = _curFrameY = 0;
					_gravity = 0.5f;
					_x = _climbX;
				}
			}
		}
		else
		{
			if (_bottomLadder)
			{
				if (_state == PS_CLIMBSTOP)
				{
					_state = PS_CLIMB;
					_curFrameX = _curFrameY = 0;
					_gravity = _jumpPwr = 0.f;
					_x = _climbX;
					_y += 2;
				}
				else if (_state == PS_IDLE)
				{
					_state = PS_CLIMBSTOP;
					_curFrameX = _curFrameY = 0;
					_gravity = _jumpPwr = 0.f;
					_x = _climbX;
					_y += 32;
				}
			}
		}
	}

	//점프키
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (_state == PS_IDLE || _state == PS_WALK)
		{
			_isColl = false;
			_state = PS_JUMPUP;
			_curFrameX = 0;
			_jumpPwr = 10.0f;
			_gravity = 0.5f;
		}
	}

	//공격키
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		attack();
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
	{
		if (_state == PS_WALK)
		{
			_state = PS_IDLE;
			_curFrameX = 0;
		}
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
	{
		if (_state == PS_WALK)
		{
			_state = PS_IDLE;
			_curFrameX = 0;
		}
	}

	setRect();
}

void player::move()
{
	if (correctY())
		return;

	if (_isBounce)
		bounce();

	_y -= _jumpPwr;
	_jumpPwr -= _gravity;
	setRect();

	if (_y > WINSIZEY * (PAGENUMY - 1))
	{
		_y = WINSIZEY * (PAGENUMY - 1) - 32;
		_state = PS_DEAD;
		setRect();
		return;
	}

	if (_state == PS_JUMPUP && _jumpPwr < 0)
		_state = PS_JUMPDOWN;	

	pixelCollision();
	objCollision();
	itemCollision();
}

void player::bounce()
{
	if (_state != PS_DIG && _state != PS_DIGDOWN)
	{
		if (_isRight)
		{
			_x -= 5;
		}
		else/* if (!_isRight)*/
		{
			_x += 5;
		}
	}
	else if (_state == PS_DIG)
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
}

void player::attack()
{
	if (_state != PS_DIG && _state != PS_CLIMB)
	{
		_state = PS_DIG;
		_curFrameX = 0;
	}
}

bool player::correctY()
{
	//UI에 캐릭터가 가려지지 않게 보정
	if (_y < _sourY + 64)
	{
		_y = _sourY + 80;
		setRect();
		return true;
	}
	else if (_y > _sourY + WINSIZEY - 33)
	{
		_y = _sourY + WINSIZEY + 80;
		setRect();
		return true;
	}
	else if (_y < _sourY + 80 && _y > _sourY + 64)
	{
		_y = _sourY - 32;
		setRect();
		return true;
	}

	return false;
}

void player::ladderCenter(float x, float y)
{
	int left = 0, right = 0;

	if (GetPixel(getPixelDC(), x, y) == 0xff0000)
	{
		for (int i = x - 1; ; --i)
		{
			if (GetPixel(getPixelDC(), i, y) != 0xff0000)
			{
				left = i;
				break;
			}
		}
	}

	if (GetPixel(getPixelDC(), x, y) == 0xff0000)
	{
		for (int i = x + 1; ; ++i)
		{
			if (GetPixel(getPixelDC(), i, y) != 0xff0000)
			{
				right = i;
				break;
			}
		}
	}

	_climbX = (left + right) / 2;
}

void player::ladderCheck()
{
	//중앙
	if (GetPixel(getPixelDC(), _x, _y) == 0xff0000)
	{
		_centerLadder = true;
		ladderCenter(_x, _y);
	}
	else
	{
		_centerLadder = false;
	}
	
	//하단
	if (GetPixel(getPixelDC(), _x, _y + 33) == 0xff0000)
	{
		_bottomLadder = true;
		ladderCenter(_x, _y + 33);
	}
	else
	{
		_bottomLadder = false;
	}
}

void player::pixelCollision()
{
	if (_state == PS_CLIMB || _state == PS_CLIMBSTOP) return;

	//좌
	if (GetPixel(getPixelDC(), _coll.left, (_coll.top + _coll.bottom) / 2 + 5) == 0xff00ff)
	{
		_isColl = true;

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _coll.left + 1; ; ++i)
		{
			if (GetPixel(getPixelDC(), i, (_coll.top + _coll.bottom) / 2 + 5) != 0xff00ff)
			{
				int interval = i - _coll.left;
				_x += interval;
				setRect();
				break;
			}
		}
	}

	//우
	if (GetPixel(getPixelDC(), _coll.right, (_coll.top + _coll.bottom) / 2 + 5) == 0xff00ff)
	{
		_isColl = true;

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _coll.right - 1; ; --i)
		{
			if (GetPixel(getPixelDC(), i, (_coll.top + _coll.bottom) / 2 + 5) != 0xff00ff)
			{
				int interval = _coll.right - i;
				_x -= interval;
				setRect();
				break;
			}
		}
	}

	//중앙상단
	if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, _coll.top + 2) == 0xff00ff)
	{
		_jumpPwr = 0.f;

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _coll.top + 1; ; ++i)
		{
			if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, i) != 0xff00ff)
			{
				int interval = i - _coll.top;
				_y += interval;
				setRect();
				_state = PS_JUMPDOWN;
				break;
			}
		}
	}
	//좌상단
	else if (GetPixel(getPixelDC(), _coll.left, _coll.top + 2) == 0xff00ff)
	{
		_isColl = true;

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _coll.top + 1; ; ++i)
		{
			if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, i) != 0xff00ff)
			{
				int interval = i - _coll.top;
				_y += interval;
				setRect();
				_state = PS_JUMPDOWN;
				break;
			}
		}

	}
	//우상단
	else if (GetPixel(getPixelDC(), _coll.right, _coll.top + 2) == 0xff00ff)
	{
		_isColl = true;

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _coll.top + 1; ; ++i)
		{
			if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, i) != 0xff00ff)
			{
				int interval = i - _coll.top;
				_y += interval;
				setRect();
				_state = PS_JUMPDOWN;
				break;
			}
		}
	}

	//하
	if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, _coll.bottom) == 0xff00ff)
	{
		_isColl = false;
		_isBounce = false;
		_gravity = 0.5f;
		_jumpPwr = 0.f;

		if (_state != PS_IDLE && _state != PS_WALK && _state != PS_DIG && _state != PS_DEAD)
		{
			_state = PS_IDLE;
			_curFrameX = 0;
		}

		//파고든 만큼 밀어주기 위해 파고든 정도를 체크
		for (int i = _coll.bottom - 1; ; --i)
		{
			if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, i) != 0xff00ff)
			{
				int interval = _coll.bottom - i;
				_y -= interval;
				setRect();
				break;
			}
		}
	}
	//하단이 사다리이면
	else if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, _coll.bottom) == 0xff0000)
	{
		_isColl = false;

		//캐릭터의 중심 좌표가 사다리가 아니면
		if (GetPixel(getPixelDC(), _x, _y) != 0xff0000)
		{
			for (int i = _coll.bottom - 1; ; --i)
			{
				if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, i) != 0xff0000)
				{
					int interval = _coll.bottom - i;
					_y -= interval;
					setRect();
					break;
				}
			}
		}
	}
	//하단이 가시이면
	else if (GetPixel(getPixelDC(), (_coll.left + _coll.right) / 2, _coll.bottom) == 0x0000ff)
	{
		_isColl = false;
		_gravity = 0.5f;

		damage(_life);

		setBounce(true, 10.f);
	}
}

void player::objCollision()
{
	int size = _objMgr->getVObject().size();
	for (int i = 0; i < size; ++i)
	{
		if (_objMgr->getVObject()[i]->getX() < _sourX
			|| _objMgr->getVObject()[i]->getX() > _sourX + WINSIZEX) continue;
		if (_objMgr->getVObject()[i]->getIsBroken()) continue;

		OBJECTTYPE type = _objMgr->getVObject()[i]->getType();
		RECT cmpRect = _objMgr->getVObject()[i]->getRect();
		RECT temp;

		if (_state == PS_DIG)
		{
			if (IntersectRect(&temp, &cmpRect, &_atk))
			{
				digObject(type, i);
			}
		}

		if (IntersectRect(&temp, &cmpRect, &_coll))
		{
			_isBounce = false;
			_gravity = 0.5f;

			int width = temp.right - temp.left;
			int height = temp.bottom - temp.top;

			if (width > height)
			{
				//위에서 충돌
				if (temp.top == cmpRect.top)
				{
					if (_state == PS_JUMPUP || _state == PS_JUMPDOWN)
						_state = PS_IDLE;
					_jumpPwr = 0.f;
					_y -= height;

					if (_state == PS_DIGDOWN)
					{
						digdownObject(type, i);
					}

					if (type == MOVINGTILE01 || type == MOVINGTILE02)
					{
						_gravity = 0.f;
						_objMgr->getVObject()[i]->setPlayerMemoryLink(this);
						_objMgr->getVObject()[i]->setOnCharacter(true);
					}
				}
				//아래에서 충돌
				else if (temp.bottom == cmpRect.bottom)
				{
					if(_state != PS_JUMPDOWN)
						_jumpPwr = 0.f;
					_y += height;
					_state = PS_JUMPDOWN;
					_curFrameX = 0;
				}
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
			setRect();
			return;
		}
	}
}

void player::itemCollision()
{
	int size = ITEMMANAGER->getVItem().size();
	RECT cmpRect;
	
	for (int i = 0; i < size; ++i)
	{
		cmpRect = ITEMMANAGER->getVItem()[i]->getRect();

		if (IntersectRect(&RectMake(0, 0, 0, 0), &cmpRect, &_coll))
		{
			if (ITEMMANAGER->getVItem()[i]->getType() == LIFE)
			{
				_life;
			}
			else
			{
				_gold += ITEMMANAGER->getVItem()[i]->getGold();
			}

			ITEMMANAGER->getVItem()[i]->setIsOn(false);
			break;
		}
	}
}

void player::digdownObject(OBJECTTYPE type, int arrNum)
{
	switch (type)
	{
	case BLOCK_SMALL: case BLOCK_BIG:
		_objMgr->getVObject()[arrNum]->setIsBroken(true);
		setBounce(true, 10.f);
		break;
	case CHECKPOINT01:
		setBounce(true, 5.f);
		break;
	case CHECKPOINT02:
		setBounce(true, 5.f);
		break;
	case CHEST:
		setBounce(true, 5.f);
		break;
	case AWL:
		setBounce(true, 5.f);
		break;
	case MOVINGTILE01: case MOVINGTILE02:
		_state = PS_IDLE;
		_curFrameX = 0;
		break;
	case PLATTER:
		setBounce(true, 5.f);
		break;
	default:
		break;
	}
}

void player::digObject(OBJECTTYPE type, int arrNum)
{
	switch (type)
	{
	case BLOCK_SMALL: case BLOCK_BIG:
		_objMgr->getVObject()[arrNum]->setIsBroken(true);
		break;
	case CHECKPOINT01:
		break;
	case CHECKPOINT02:
		break;
	case CHEST:
		_objMgr->getVObject()[arrNum]->setIsBroken(true);
		break;
	case PILE01: case PILE02: case PLATTER:
		break;
	default:
		break;
	}
}

void player::damage(int damage)
{
	if (_state == PS_DAMAGE || _state == PS_DEAD) return;

	_life -= damage;
	if (_life <= 0)
	{
		_life = 0;
		_state = PS_DEAD;
		_curFrameX = 0;
		return;
	}

	//데미지 모션
	_state = PS_DAMAGE;
	_curFrameX = 0;
}

void player::setBounce(bool isBounce, float jumpPwr)
{
	_isBounce = isBounce;
	_jumpPwr = jumpPwr;
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
	case PS_CLIMBSTOP:
		_image = IMAGEMANAGER->findImage("shovel_climbstop");
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
	case PS_DAMAGE: case PS_DEAD:
		_image = IMAGEMANAGER->findImage("shovel_damage");
		_image->setFrameY(_curFrameY);
		break;
	}

	//이미지 사이즈로 갱신
	setRect();

	setFrame();
}

void player::setFrame()
{
	_count++;

	if (_state == PS_DAMAGE)
	{
		if (_count % 5 == 0)
		{
			if (_alpha >= 256)
				_alpha -= 128;
			else
				_alpha += 128;
		}
	}
	else if (_state == PS_DEAD)
	{
		if (_count % 5 == 0)
		{
			if (_alpha >= 256)
				_alpha -= 128;
			else
				_alpha += 128;
		}

		_deadTime += TIMEMANAGER->getElapsedTime();
		if (_deadTime >= 1.f)
		{
			_isDead = true;
			return;
		}
	}

	if (_count % 7 == 0)
	{
		if (_state != PS_CLIMB)
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

void player::setRect()
{
	_rc = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_coll = RectMakeCenter(_x, _y, 32, 64);

	if(_isRight)
		_atk = RectMakeCenter(_x + 48, _y + 12, 32, 32);
	else
		_atk = RectMakeCenter(_x - 48, _y + 12, 32, 32);
}
