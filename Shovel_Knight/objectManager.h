#pragma once
#include "gameNode.h"
#include "block.h"
#include "ladder.h"

class objectManager : public gameNode
{
private:
	vector<objects*> _vObject;
	vector<objects*>::iterator _viObject;

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

