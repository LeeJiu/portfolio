#pragma once
#include "gameNode.h"

class objectManager;

class stageMap : public gameNode
{
private:
	tagTile _tile[TILENUMY][TILENUMX];

	objectManager* _objMgr;

public:
	stageMap();
	~stageMap();

	virtual HRESULT init(int stageNum);
	void release();
	void update();
	void render();

	void load(int stageNum);

	void moveMap(float x, float y);

	tagTile(*getTile())[TILENUMX] { return _tile; }

	void setObjMemoryLink(objectManager* objMgr) { _objMgr = objMgr; }
};

