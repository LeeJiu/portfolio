#include "stdafx.h"
#include "objectManager.h"
#include "gameMap.h"


objectManager::objectManager()
{
}


objectManager::~objectManager()
{
}

HRESULT objectManager::init()
{
	setObject(PLAYER, 0, 0);
	return S_OK;
}

void objectManager::release()
{
	for (_viObject = _vObject.begin(); _viObject != _vObject.end(); ++_viObject)
	{
		(*_viObject)->release();
		SAFE_DELETE(*_viObject);
	}
	_vObject.clear();
}

void objectManager::update()
{
	for (_viObject = _vObject.begin(); _viObject != _vObject.end(); ++_viObject)
	{
		(*_viObject)->update();
	}
}

void objectManager::render()
{
	//rect의 bottom(y축)으로 정렬해주자
	std::sort(_vObject.begin(), _vObject.end(), objectManager::Compare);

	for (_viObject = _vObject.begin(); _viObject != _vObject.end(); ++_viObject)
	{
		if ((*_viObject)->isDead()) continue;

		(*_viObject)->render();
	}
}

void objectManager::setObject(TYPE type, int x, int y)
{
	switch (type)
	{
	case PLAYER:
		_obj = new player;
		_obj->init();
		_obj->setObjectMgrMemoryLink(this);
		_vObject.push_back(_obj);
		break;
	case BOXBOY:
		_obj = new boxboy;
		_obj->init(x, y);
		_obj->setObjectMgrMemoryLink(this);
		_vObject.push_back(_obj);
		break;
	case CHOCOBEE:
		_obj = new chocobee;
		_obj->init(x, y);
		_obj->setObjectMgrMemoryLink(this);
		_vObject.push_back(_obj);
		break;
	/*case HIP:
		break;
	case BOSS:
		break;
	default:
		break;*/
	}
}