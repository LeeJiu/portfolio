#pragma once
#include "gameNode.h"

class objects : public gameNode
{
protected:
	image* _image;
	RECT _rc;
	int _x, _y;
	bool _isBroken;
	OBJECTTYPE _type;

public:
	objects();
	~objects();

	virtual HRESULT init(OBJECTTYPE type, int x, int y);
	void release();
	void update();
	void render();

	void setIsBroken(bool isBroken) { _isBroken = isBroken; }

	bool getIsBroken() { return _isBroken; }
	int getX() { return _x; }
	int getY() { return _y; }
	RECT getRect() { return _rc; }
	OBJECTTYPE getType() { return _type; }
};

