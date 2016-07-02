#include "stdafx.h"
#include "ladder.h"


ladder::ladder()
{
}


ladder::~ladder()
{
}

HRESULT ladder::init(OBJECTTYPE type, int x, int y)
{
	_image = IMAGEMANAGER->findImage("ladder");

	_type = type;
	_x = x;
	_y = y;
	_rc = RectMake(_x, _y, _image->getWidth(), _image->getHeight());

	return S_OK;
}

void ladder::release()
{
}

void ladder::update()
{
}

void ladder::render()
{
	_image->render(getMemDC(), _rc.left, _rc.top);
}
