#include "stdafx.h"
#include "itemManager.h"


itemManager::itemManager()
{
}


itemManager::~itemManager()
{
}

HRESULT itemManager::init()
{
	_vItem.reserve(50);
	return S_OK;
}

void itemManager::release()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		(*_viItem)->release();
	}
}

void itemManager::update()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end();)
	{
		if (!(*_viItem)->getIsOn())
		{
			SAFE_DELETE(*_viItem);
			_viItem = _vItem.erase(_viItem);
		}
		else
		{
			(*_viItem)->update();
			++_viItem;
		}
	}
}

void itemManager::render()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		(*_viItem)->render();
	}
}

void itemManager::addGem(float x, float y, bool isBounce, bool isRight, int gemNum, int * gold)
{
	int num = (*gold) / gemNum;
	if (num > 0)
		*gold -= gemNum * num;
	else if (num == 0)
		return;
	
	ITEMTYPE type;
	if (gemNum == 1) type = GEM1;
	else if (gemNum == 5) type = GEM5;
	else if (gemNum == 10) type = GEM10;
	else if (gemNum == 20) type = GEM20;
	else if (gemNum == 35) type = GEM35;

	int rndX;
	for (int i = 0; i < num; ++i)
	{
		rndX = RND->getInt(50);

		item* gem;
		gem = new item;
		gem->init(type, x + rndX, y, isBounce, isRight);
		_vItem.push_back(gem);
	}
}

void itemManager::addGemsItem(float x, float y, bool isBounce, bool isRight, int gold)
{
	int orgGold = gold;

	addGem(x, y, isBounce, isRight, 35, &orgGold);
	addGem(x, y, isBounce, isRight, 20, &orgGold);
	addGem(x, y, isBounce, isRight, 10, &orgGold);
	addGem(x, y, isBounce, isRight, 5, &orgGold);
	addGem(x, y, isBounce, isRight, 1, &orgGold);
}

void itemManager::addLifeItem(float x, float y, int life)
{
}