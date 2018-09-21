#include "stdafx.h"
#include "gameOver.h"


gameOver::gameOver()
{
}


gameOver::~gameOver()
{
}

HRESULT gameOver::init()
{
	_sourX = _sourY = _count = _fireFrameX = _charFrameX = 0;

	IMAGEMANAGER->addImage("campfire_bg", "image/background/gameover.bmp", WINSIZEX, WINSIZEY, false, false);
	IMAGEMANAGER->addImage("gameover", "image/gameover_txt.bmp", 288, 91, true, 0xff00ff);

	_fire = IMAGEMANAGER->addFrameImage("campfire", "image/background/campfire.bmp", 3780, 154, 21, 1, true, 0xff00ff);
	_character = IMAGEMANAGER->addFrameImage("shovel_camp", "image/player/shovel_camp.bmp", 156, 54, 3, 1, true, 0xff00ff);

	_start = false;

	return S_OK;
}

void gameOver::release()
{
	SOUNDMANAGER->stop("campfire");
}

void gameOver::update()
{
	if (!_start)
	{
		SOUNDMANAGER->play("campfire", 0.7f);
		_start = true;
	}

	_count++;

	if (_count % 15 == 0)
	{
		_charFrameX++;
		if (_charFrameX > _character->getMaxFrameX() - 1)
			_charFrameX = 0;
		_character->setFrameX(_charFrameX);
	}

	if (_count % 15 == 0)
	{
		_fireFrameX++;
		if (_fireFrameX > _fire->getMaxFrameX())
		{
			_fireFrameX = _fire->getMaxFrameX();		
		}
		if (_fireFrameX > _fire->getMaxFrameX() - 10)
		{
			_character->setFrameX(_character->getMaxFrameX());
		}
		_fire->setFrameX(_fireFrameX);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		SCENEMANAGER->changeScene("title");
	}
}

void gameOver::render()
{
	IMAGEMANAGER->render("campfire_bg", getMemDC());
	IMAGEMANAGER->render("gameover", getMemDC(), CENTERX - 144, 100);
	_fire->frameRender(getMemDC(), CENTERX - 120, CENTERY);
	_character->frameRender(getMemDC(), CENTERX - 100, CENTERY + 100);
}
