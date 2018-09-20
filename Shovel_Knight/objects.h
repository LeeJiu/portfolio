#pragma once
#include "gameNode.h"

class player;

class objects : public gameNode
{
protected:
	image* _image;
	RECT _rc;
	int _x, _y;
	bool _isBroken;
	bool _onCharacter;
	OBJECTTYPE _type;

	player* _player;

public:
	objects();
	~objects();

	virtual HRESULT init(OBJECTTYPE type, int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();

	void setRect(float x, float y);
	virtual void setIsBroken(bool isBroken) { _isBroken = isBroken; }
	void setOnCharacter(bool onCharacter) { _onCharacter = onCharacter; }

	bool getIsBroken() { return _isBroken; }
	bool getOnCharacter() { return _onCharacter; }
	int getX() { return _x; }
	int getY() { return _y; }
	RECT getRect() { return _rc; }
	OBJECTTYPE getType() { return _type; }

	void setPlayerMemoryLink(player* player) { _player = player; }
};

