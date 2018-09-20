#pragma once

#include "block.h"
#include "movingTile.h"
#include "chest.h"

class objectManager
{
private:
	vector<objects*> _vObject;
	vector<objects*>::iterator _viObject;

	player* _player;

public:
	objectManager();
	~objectManager();

	virtual HRESULT init(int stageNum);
	void release();
	void update();
	void render();

	void load(int stageNum);

	void setObjects(tagObject& obj);

	vector<objects*>& getVObject() { return _vObject; }
};

