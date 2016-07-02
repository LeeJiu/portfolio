#include "stdafx.h"
#include "block.h"


block::block()
{
}


block::~block()
{
}

HRESULT block::init(OBJECTTYPE type, int x, int y)
{
	if (type == BLOCK_SMALL)
	{
		_image = IMAGEMANAGER->findImage("smallBlock");
	}
	else if (type == BLOCK_BIG)
	{
		_image = IMAGEMANAGER->findImage("bigBlock");
	}

	_type = type;
	_x = x;
	_y = y;
	_rc = RectMake(_x, _y, _image->getWidth(), _image->getHeight());

	return S_OK;
}

void block::release()
{
}

void block::update()
{
}

void block::render()
{
	if(!_isBroken)
		_image->render(getMemDC(), _rc.left, _rc.top);
}
