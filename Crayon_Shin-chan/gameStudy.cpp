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
	
	_objectMgr = new objectManager;
	_objectMgr->init();

	_map->setObjectMgrMemoryLink(_objectMgr);
	_objectMgr->setMapMemoryLink(_map);

	return S_OK;
}

void gameStudy::release()
{
	SAFE_DELETE(_map);
	SAFE_DELETE(_objectMgr);

	gameNode::release();
}

void gameStudy::update()
{
	_map->update();
	_objectMgr->update();

	gameNode::update();
}

void gameStudy::render()
{
	//����ۿ��� �ѷ��ش�
	
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//======================
	//�� ���̿��� �׷��ָ� ��.

	_map->render();
	_objectMgr->render();

	//���콺 ��ǥ ��ġ �����
	char str[128];
	sprintf_s(str, "x: %d, y: %d", _ptMouse.x, _ptMouse.y);
	TextOut(getMemDC(), _ptMouse.x, _ptMouse.y, str, strlen(str));

	//======================
	TIMEMANAGER->render(getMemDC());
	this->getBackBuffer()->render(getHDC(), 0, 0);
}