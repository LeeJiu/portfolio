#include "stdafx.h"
#include "title.h"


title::title()
{
}


title::~title()
{
}

HRESULT title::init()
{
	_sourX = 0;
	_sourY = 0;

	IMAGEMANAGER->addImage("title", "image/menu/title.bmp", WINSIZEX, WINSIZEY, false, false);

	IMAGEMANAGER->addImage("btStart", "image/menu/startgame.bmp", 200, 50, true, 0xff00ff);
	IMAGEMANAGER->findImage("btStart")->alphaInit();
	IMAGEMANAGER->addImage("btMaptool", "image/menu/maptool.bmp", 200, 50, true, 0xff00ff);
	IMAGEMANAGER->findImage("btMaptool")->alphaInit();
	IMAGEMANAGER->addImage("btExit", "image/menu/exit.bmp", 200, 50, true, 0xff00ff);

	_cursor = IMAGEMANAGER->addFrameImage("cursor", "image/menu/cursor.bmp", 200, 25, 2, 1, true, 0xff00ff);

	_cursorX = CENTERX - 220;
	_cursorY = CENTERY + 62;
	_cursorDist = 350;

	_select = false;
	_selectScene = 1;

	_btAlpha[0] = _btAlpha[1] = 255;

	_start = false;

	return S_OK;
}

void title::release()
{
	SOUNDMANAGER->stop("main");
}

void title::update()
{
	if (!_start)
	{
		SOUNDMANAGER->play("main", 0.7f);
		_start = true;
	}

	setFrame();
	keyControl();
	moveCursor();
}

void title::render()
{
	IMAGEMANAGER->render("title", getMemDC());
	IMAGEMANAGER->findImage("btStart")->alphaRender(getMemDC(), CENTERX - 100, CENTERY + 50, _btAlpha[0]);
	IMAGEMANAGER->findImage("btMaptool")->alphaRender(getMemDC(), CENTERX - 100, CENTERY + 100, _btAlpha[1]);
	IMAGEMANAGER->findImage("btExit")->render(getMemDC(), CENTERX - 100, CENTERY + 150);

	_cursor->frameRender(getMemDC(), _cursorX, _cursorY);
}

void title::keyControl()
{
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (_select) return;

		_selectScene--;
		_cursorY -= 50;
		if (_selectScene < 1)
		{
			_selectScene = 3;
			_cursorY = CENTERY + 162;
		}

		SOUNDMANAGER->play("select", 1);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (_select) return;

		_selectScene++;
		_cursorY += 50;
		if (_selectScene > 3)
		{
			_selectScene = 1;
			_cursorY = CENTERY + 62;
		}

		SOUNDMANAGER->play("select", 1);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		if (!_select)
		{
			SOUNDMANAGER->play("ok", 1);
			_select = true;
		}
	}
}

void title::setFrame()
{
	if (_select) return;

	_count++;
	if (_count % 30 == 0)
	{
		_curFrameX++;
		if (_curFrameX == 2)
			_curFrameX = 0;
		_cursor->setFrameX(_curFrameX);
	}
}

void title::moveCursor()
{
	if (_select)
	{
		_cursorX += 10;
		_cursorDist -= 10;

		_count++;
		if (_count % 10 == 0)
		{
			if(_btAlpha[_selectScene - 1] <= 128)
				_btAlpha[_selectScene - 1] += 128;
			else
				_btAlpha[_selectScene - 1] -= 128;
		}

		if (_cursorDist < 0)
		{
			moveScene();
		}
	}
}

void title::moveScene()
{
	switch (_selectScene)
	{
	case 1:
		//게임스타트 시, 임시로 스테이지로 바로 넘어가고, 0번 스테이지 넣음
		SCENEMANAGER->changeScene("stage", 0);
		break;
	case 2:
		SCENEMANAGER->changeScene("maptool");
		break;
	case 3:
		PostMessage(_hWnd, WM_DESTROY, 0, 0);
		break;
	}
}
