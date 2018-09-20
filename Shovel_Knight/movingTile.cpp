#include "stdafx.h"
#include "movingTile.h"
#include "player.h"


movingTile::movingTile()
{
}


movingTile::~movingTile()
{
}

HRESULT movingTile::init(OBJECTTYPE type, int x, int y)
{
	if (type == MOVINGTILE01)
	{
		_image = IMAGEMANAGER->findImage("movingTile01");
	}

	_type = type;
	_x = x;
	_y = y;
	_rc = RectMake(_x, _y, _image->getWidth(), _image->getHeight());
	
	_isRight = true;
	_isUp = true;

	return S_OK;
}

void movingTile::release()
{
}

void movingTile::update()
{
	if (_onCharacter)
	{
		RECT cmpRect = _player->getCollRect();
		if (cmpRect.left > _rc.right || cmpRect.right < _rc.left)
		{
			_onCharacter = false;
			_player->setGravity();
		}
	}

	move();
}

void movingTile::render()
{
	_image->render(getMemDC(), _rc.left, _rc.top);
}

void movingTile::move()
{
	int centerX, centerY;
	centerX = _x + _image->getFrameWidth() / 2;
	centerY = _y + _image->getFrameHeight() / 2;

	if (GetPixel(getPixelDC(), centerX + 16, centerY) == 0x00ff00)
		_isHorizon = true;
	else if (GetPixel(getPixelDC(), centerX, centerY - 16) == 0x00ff00)
		_isHorizon = false;

	//가로로 움직인다.
	if (_isHorizon)
	{
		//오른쪽으로
		if (_isRight)
		{
			_x += 2;
			if (_onCharacter)
				_player->setX(_player->getX() + 2);
			if (GetPixel(getPixelDC(), centerX + 16, centerY) != 0x00ff00)
				_isRight = false;
		}
		//왼쪽으로
		else
		{
			_x -= 2;
			if (_onCharacter)
				_player->setX(_player->getX() - 2);
			if (GetPixel(getPixelDC(), centerX - 16, centerY) != 0x00ff00)
				_isRight = true;
		}
	}
	//세로로 움직인다.
	else
	{
		//윗쪽으로
		if (_isUp)
		{
			_y -= 2;
			if (_onCharacter)
				_player->setY(_player->getY() - 2);
			if (GetPixel(getPixelDC(), centerX, centerY - 16) != 0x00ff00)
				_isUp = false;
		}
		//아랫쪽으로
		else
		{
			_y += 2;
			if (_onCharacter)
				_player->setY(_player->getY() + 2);
			if (GetPixel(getPixelDC(), centerX, centerY + 16) != 0x00ff00)
				_isUp = true;
		}
	}

	setRect(_x, _y);
}
