#include "stdafx.h"
#include "gameStudy.h"


gameStudy::gameStudy()
{
}


gameStudy::~gameStudy()
{
}

HRESULT gameStudy::init()
{
	gameNode::init(true);

	_trigger = TITLE;

	_scene = new gameScene;
	_scene->init();

	_map = new gameMap;
	_map->init();
	
	_objectMgr = new objectManager;
	_objectMgr->init();

	_map->setObjectMgrMemoryLink(_objectMgr);
	_objectMgr->setMapMemoryLink(_map);

	return S_OK;
}

void gameStudy::release()
{
	SAFE_DELETE(_scene);
	SAFE_DELETE(_map);
	SAFE_DELETE(_objectMgr);

	gameNode::release();
}

void gameStudy::update()
{
	if (_trigger == TITLE)
	{
		_trigger = _scene->getTrigger();
		if(_trigger == GAMESTART)
			_scene->setTrigger(GAMENONE);
	}
	else if (_trigger == GAMEOVER)
	{
		if (_scene->getTrigger() == GAMENONE)
			_scene->setTrigger(GAMEOVER);
		_trigger = _scene->getTrigger();
	}
	else if (_trigger == GAMESTART)
	{
		_trigger = _objectMgr->getTrigger();
		if (_scene->getTrigger() == GAMESTART)
			_scene->setTrigger(GAMENONE);
	}
	else if (_trigger == GAME)
	{
		_trigger = _objectMgr->getTrigger();
	}
	else if (_trigger == CLEAR)
	{
		if(_scene->getTrigger() == GAMENONE)
			_scene->setTrigger(CLEAR);
		_trigger = _scene->getTrigger();
	}

	switch (_trigger)
	{
	case TITLE:
		_scene->update();
		break;
	case GAMESTART:
		_map->release();
		_objectMgr->release();
		_map->init();
		_objectMgr->init();
		_map->setObjectMgrMemoryLink(_objectMgr);
		_objectMgr->setMapMemoryLink(_map);
		break;
	case GAME:
		_map->update();
		_objectMgr->update();
		break;
	case GAMEOVER:
		_scene->update();
		break;
	case CLEAR:
		_scene->update();
		break;
	default:
		break;
	}

	gameNode::update();
}

void gameStudy::render()
{
	//백버퍼에서 뿌려준다
	
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//======================
	//이 사이에서 그려주면 됨.

	switch (_trigger)
	{
	case TITLE:
		_scene->render();
		break;
	case GAMESTART: case GAME:
		_map->render();
		_objectMgr->render();
		break;
	case GAMEOVER:
		_scene->render();
		break;
	case CLEAR:
		_scene->render();
		break;
	default:
		break;
	}

	//======================
	TIMEMANAGER->render(getMemDC());
	this->getBackBuffer()->render(getHDC(), 0, 0);
}