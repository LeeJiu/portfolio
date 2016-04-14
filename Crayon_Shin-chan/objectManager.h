#pragma once

#include "gameNode.h"
#include "player.h"
#include "boxboy.h"
#include "chocobee.h"
#include <vector>
#include <algorithm>

class gameMap;

class objectManager : public gameNode
{
private:
	typedef vector<gameObject*> vObject;
	typedef vObject::iterator viObject;

private:
	vObject _vObject;
	viObject _viObject;

	gameObject* _obj;
	gameMap* _gameMap;

public:
	objectManager();
	~objectManager();

	HRESULT init();
	void release();
	void update();
	void render();
	
	void setObject(TYPE type, int x, int y);

	static bool Compare(gameObject* start, gameObject*	end)
	{
		return start->getRect().bottom < end->getRect().bottom;
	}

	inline vector<gameObject*> getVObject() { return _vObject; }
	inline vector<gameObject*>::iterator getVIObject() { return _viObject; }
	
	void setMapMemoryLink(gameMap* gameMap) { _gameMap = gameMap; }
};

