#pragma once
#include "gameNode.h"
#include "beeto.h"

class stageMap;
class objectManager;

class enemyManager : public gameNode
{
private:
	vector<enemy*> _vEnemy;
	vector<enemy*>::iterator _viEnemy;

	int _stageNum;
	bool _load;

	stageMap* _map;
	objectManager* _objMgr;

public:
	enemyManager();
	~enemyManager();

	virtual HRESULT init(int stageNum);
	void release();
	void update();
	void render();

	void loadEnemy();

	void setEnemy(tagEnemy& stEnemy);
	
	void setMapMemoryLink(stageMap* map) { _map = map; }
	void setObjMemoryLink(objectManager* objMgr) { _objMgr = objMgr; }

	vector<enemy*>& getVEnemy() { return _vEnemy; }
};

