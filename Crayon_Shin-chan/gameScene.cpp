#include "stdafx.h"
#include "gameScene.h"


gameScene::gameScene()
{
}


gameScene::~gameScene()
{
}

HRESULT gameScene::init()
{
	_sceneImage = new image;
	_sceneImage->init("image/ui/title.bmp", WINSIZEX, WINSIZEY, false, false);
	_trigger = TITLE;

	return S_OK;
}

void gameScene::release()
{
	SAFE_DELETE(_sceneImage);
}

void gameScene::update()
{
	if(_trigger == TITLE)
		_sceneImage->init("image/ui/title.bmp", WINSIZEX, WINSIZEY, false, false);
	else if(_trigger == GAMEOVER)
		_sceneImage->init("image/ui/gameover.bmp", WINSIZEX, WINSIZEY, false, false);
	else if(_trigger == CLEAR)
		_sceneImage->init("image/ui/clear.bmp", WINSIZEX, WINSIZEY, false, false);

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		if (_trigger == TITLE || _trigger == GAMEOVER)
		{
			_trigger = GAMESTART;
		}
		else if (_trigger == CLEAR)
		{
			_trigger = TITLE;
		}
	}
}

void gameScene::render()
{
	_sceneImage->render(getMemDC(), 0, 0);
}
