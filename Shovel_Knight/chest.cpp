#include "stdafx.h"
#include "chest.h"


chest::chest()
{
}


chest::~chest()
{
}

HRESULT chest::init(OBJECTTYPE type, int x, int y)
{
	_image = IMAGEMANAGER->findImage("chest");

	image* mapImage = IMAGEMANAGER->findImage("chest_map");

	int intervalY;
	intervalY = _image->getFrameHeight() - mapImage->getFrameHeight();

	_type = type;
	_x = x;
	_y = y - intervalY;
	_rc = RectMake(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());

	return S_OK;
}

void chest::release()
{
}

void chest::update()
{
}

void chest::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

void chest::setIsBroken(bool isBroken)
{
	if (_isBroken) return;

	_isBroken = isBroken;
	_image->setFrameX(1);
	ITEMMANAGER->addGemsItem(_x, _y, false, false, 150);
}
