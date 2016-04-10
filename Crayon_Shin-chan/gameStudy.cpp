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

	_map = new gameMap;
	_map->init();
	
	_player = new player;
	_player->init();

	_map->setPlayerMemoryLink(_player);
	_player->setMapMemoryLink(_map);

	return S_OK;
}

void gameStudy::release()
{
	SAFE_DELETE(_map);
	SAFE_DELETE(_player);

	gameNode::release();
}

void gameStudy::update()
{
	_map->update();
	_player->update();

	gameNode::update();
}

void gameStudy::render()
{
	//백버퍼에서 뿌려준다
	
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//IMAGEMANAGER->findImage("background")->render(getMemDC());
	//======================
	//이 사이에서 그려주면 됨.

	_map->render();
	_player->render();

	//======================
	TIMEMANAGER->render(getMemDC());
	this->getBackBuffer()->render(getHDC(), 0, 0);
}