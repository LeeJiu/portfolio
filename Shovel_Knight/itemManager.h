#pragma once
#include "singletonBase.h"
#include "item.h"

class itemManager : public singletonBase<itemManager>
{
private:
	vector<item*> _vItem;
	vector<item*>::iterator _viItem;

public:
	itemManager();
	~itemManager();

	HRESULT init();
	void release();
	void update();
	void render();

	void addGem(float x, float y, bool isBounce, bool isRight, int gemNum, int* gold);

	void addGemsItem(float x, float y, bool isBounce, bool isRight, int gold);
	void addLifeItem(float x, float y, int life);

	vector<item*>& getVItem() { return _vItem; }
};

