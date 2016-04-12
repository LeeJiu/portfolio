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

	_enemyMgr = new enemyManager;
	_enemyMgr->init();

	_map->setPlayerMemoryLink(_player);
	_map->setEnemyMgrMemoryLink(_enemyMgr);

	_player->setMapMemoryLink(_map);
	_player->setEnemyMgrMemoryLink(_enemyMgr);

	_enemyMgr->setPlayerMemoryLink(_player);

	return S_OK;
}

void gameStudy::release()
{
	_player->release();
	SAFE_DELETE(_map);
	SAFE_DELETE(_player);
	SAFE_DELETE(_enemyMgr);

	gameNode::release();
}

void gameStudy::update()
{
	_map->update();
	_player->update();
	_enemyMgr->update();

	gameNode::update();
}

void gameStudy::render()
{
	//백버퍼에서 뿌려준다
	
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//======================
	//이 사이에서 그려주면 됨.

	_map->render();
	_player->render();
	_enemyMgr->render();

	//마우스 좌표 위치 보기용
	char str[128];
	sprintf_s(str, "x: %d, y: %d", _ptMouse.x, _ptMouse.y);
	TextOut(getMemDC(), _ptMouse.x, _ptMouse.y, str, strlen(str));

	//======================
	TIMEMANAGER->render(getMemDC());
	this->getBackBuffer()->render(getHDC(), 0, 0);
}