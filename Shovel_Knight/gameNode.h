#pragma once

#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX * PAGENUMX, WINSIZEY * PAGENUMY);
static image* _pixelBuffer = IMAGEMANAGER->addImage("pixelBuffer", WINSIZEX * PAGENUMX, WINSIZEY * PAGENUMY);

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;

public:
	gameNode();
	virtual ~gameNode();

	virtual HRESULT init();
	virtual HRESULT init(int num);
	//virtual HRESULT init(int x, int y);
	virtual HRESULT init(bool managerInit);
	//virtual HRESULT init(OBJECTTYPE type, int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();

	image* getBackBuffer() { return _backBuffer; }
	image* getPixelBuffer() { return _pixelBuffer; }
	static HDC getMemDC() { return _backBuffer->getMemDC(); }
	static HDC getPixelDC() { return _pixelBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

