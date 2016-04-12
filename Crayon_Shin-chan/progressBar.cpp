#include "stdafx.h"
#include "progressBar.h"


progressBar::progressBar()
{
}


progressBar::~progressBar()
{
}

HRESULT progressBar::init(const char * bgImgName, const char * fgImgName, int maxGauge, int curGauge)
{
	_bgImage = IMAGEMANAGER->findImage(bgImgName);
	_fgImage = IMAGEMANAGER->findImage(fgImgName);
	
	_maxGauge = maxGauge;
	_curGauge = curGauge;

	return S_OK;
}

void progressBar::release()
{
}

void progressBar::update()
{
}

void progressBar::render()
{
	_bgImage->render(getMemDC(), _bgRc.left, _bgRc.top, _bgRc.right - _bgRc.left, _bgRc.bottom - _bgRc.top,
		0, 0, _bgImage->getWidth(), _bgImage->getHeight());
	_fgImage->render(getMemDC(), _fgRc.left, _fgRc.top, _fgRc.right - _fgRc.left, _fgRc.bottom - _fgRc.top,
		0, 0, _fgImage->getWidth(), _fgImage->getHeight());
}

void progressBar::setBar(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_height = height;
	_percent = width / _maxGauge;
	_bgRc = RectMake(x, y, width, height);
	_fgRc = RectMake(x, y, width, height);
}

void progressBar::setPosition(float x, float y)
{
	_x = x;
	_y = y;
	_bgRc = RectMake(_x, _y, _bgRc.right - _bgRc.left, _bgRc.bottom - _bgRc.top);
	_fgRc = RectMake(_x, _y, _curGauge * _percent, _height);
}

void progressBar::decreaseBar(int gauge)
{
	_curGauge -= gauge;
	_fgRc = RectMake(_x, _y, _curGauge * _percent, _height);
}

void progressBar::increaseBar(int gauge)
{
	_curGauge += gauge;

	if (_curGauge > _maxGauge)
		_curGauge = _maxGauge;
	
	_fgRc = RectMake(_x, _y, _curGauge * _percent, _height);
}
