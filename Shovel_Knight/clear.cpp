#include "stdafx.h"
#include "clear.h"


clear::clear()
{
}


clear::~clear()
{
}

HRESULT clear::init()
{
	_sourX = _sourY = _count = 0;

	IMAGEMANAGER->addImage("clear", "image/background/clear.bmp", WINSIZEX, WINSIZEY, false, false);

	return S_OK;
}

void clear::release()
{
}

void clear::update()
{
	//스파클 이펙트 터트리자
	_count++;
	if (_count % 40 == 0)
	{
		EFFECTMANAGER->play("sparkle1", RND->getFromIntTo(0, WINSIZEX), RND->getFromIntTo(0, WINSIZEY));
		EFFECTMANAGER->play("sparkle2", RND->getFromIntTo(0, WINSIZEX), RND->getFromIntTo(0, WINSIZEY));
		EFFECTMANAGER->play("sparkle3", RND->getFromIntTo(0, WINSIZEX), RND->getFromIntTo(0, WINSIZEY));
	}
	if (_count % 50 == 0)
	{
		EFFECTMANAGER->play("wind", RND->getFromIntTo(0, WINSIZEX), RND->getFromIntTo(0, WINSIZEY));
	}

	EFFECTMANAGER->update();

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		SCENEMANAGER->changeScene("title");
	}
}

void clear::render()
{
	IMAGEMANAGER->render("clear", getMemDC());
	EFFECTMANAGER->render();
}
