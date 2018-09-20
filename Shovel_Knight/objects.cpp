#include "stdafx.h"
#include "objects.h"


objects::objects()
{
}


objects::~objects()
{
}

HRESULT objects::init(OBJECTTYPE type, int x, int y)
{
	return S_OK;
}

void objects::release()
{
}

void objects::update()
{
}

void objects::render()
{
}

void objects::setRect(float x, float y)
{
	_x = x;
	_y = y;
	_rc = RectMake(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
}
