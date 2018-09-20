#pragma once

#include "beeto.h"
#include "skeleton.h"
#include "dragon.h"

class objectManager;
class player;

class enemyManager
{
private:
	vector<enemy*> _vEnemy;
	vector<enemy*>::iterator _viEnemy;

	enemy* _boss;

	int _stageNum;
	bool _load;

	objectManager* _objMgr;
	player* _player;

public:
	enemyManager();
	~enemyManager();

	virtual HRESULT init(int stageNum);
	void release();
	void update();
	void render();

	void loadEnemy();

	void setEnemy(tagEnemy& stEnemy);
	
	void setObjMemoryLink(objectManager* objMgr) { _objMgr = objMgr; }
	void setPlayerMemoryLink(player* player) { _player = player; }

	vector<enemy*>& getVEnemy() { return _vEnemy; }
	enemy* getBoss() { return _boss; }
};

