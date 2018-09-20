#include "stdafx.h"
#include "objectManager.h"


objectManager::objectManager()
{
}


objectManager::~objectManager()
{
}

HRESULT objectManager::init(int stageNum)
{
	load(stageNum);

	return S_OK;
}

void objectManager::release()
{
}

void objectManager::update()
{
	if (_vObject.empty()) return;

	for (_viObject = _vObject.begin(); _viObject != _vObject.end(); ++_viObject)
	{
		if((*_viObject)->getIsBroken()) continue;
		(*_viObject)->update();
	}
}

void objectManager::render()
{
	if (_vObject.empty()) return;

	for (_viObject = _vObject.begin(); _viObject != _vObject.end(); ++_viObject)
	{
		if ((*_viObject)->getRect().right < _sourX || (*_viObject)->getX() > _sourX + WINSIZEX) continue;
		if ((*_viObject)->getY() < _sourY || (*_viObject)->getY() > _sourY + WINSIZEY) continue;
		if ((*_viObject)->getIsBroken() && (*_viObject)->getType() != CHEST) continue;
		(*_viObject)->render();
	}
}

void objectManager::load(int stageNum)
{
	char str[128], temp[150000];

	//오브젝트 데이터 불러오기
	sprintf_s(str, "Stage%d_obj.txt", stageNum);
	ifstream loadObj(str);

	int dataOrder = 0;		//데이터 순서(개수)
	tagObject obj;
	while (!loadObj.eof())
	{
		loadObj.getline(temp, 100);
		if (dataOrder % 3 == 0)			//0번째 데이터 type
		{
			obj.type = (OBJECTTYPE)atoi(temp);
		}
		else if (dataOrder % 3 == 1)	//1번째 데이터 x
		{
			obj.x = atoi(temp);
		}
		else if (dataOrder % 3 == 2)	//2번째 데이터 y
		{
			obj.y = atoi(temp);
			setObjects(obj);
		}
		dataOrder++;
	}
}

void objectManager::setObjects(tagObject& obj)
{
	objects* object;

	switch (obj.type)
	{
	case BLOCK_SMALL:
		object = new block;
		object->init(BLOCK_SMALL, obj.x, obj.y);
		_vObject.push_back(object);
		break;
	case BLOCK_BIG:
		object = new block;
		object->init(BLOCK_BIG, obj.x, obj.y);
		_vObject.push_back(object);
		break;
	case CHECKPOINT01:
		break;
	case CHECKPOINT02:
		break;
	case CHEST:
		object = new chest;
		object->init(CHEST, obj.x, obj.y);
		_vObject.push_back(object);
		break;
	case AWL:
		break;
	case PILE01:
		break;
	case PILE02:
		break;
	case MOVINGTILE01:
		object = new movingTile;
		object->init(MOVINGTILE01, obj.x, obj.y);
		_vObject.push_back(object);
		break;
	case MOVINGTILE02:
		break;
	case PLATTER:
		break;
	}
}
