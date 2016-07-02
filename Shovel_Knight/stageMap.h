#pragma once
#include "gameNode.h"

class stageMap : public gameNode
{
private:
	tagTile _tile[TILENUMY][TILENUMX];

public:
	stageMap();
	~stageMap();

	virtual HRESULT init(int stageNum);
	void release();
	void update();
	void render();

	void load(int stageNum);

	void moveMap();

	tagTile(*getTile())[TILENUMX] { return _tile; }
};

