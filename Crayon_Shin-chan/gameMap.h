#pragma once
#include "gameNode.h"

class player;
class enemyManager;

class gameMap : public gameNode
{
private:
	image* _map;
	int _moveX;
	int _saveX;
	int _settingCnt;

	player* _player;
	enemyManager* _enemyMgr;

public:
	gameMap();
	~gameMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void move();	//맵 이동 제한해주자
	void setObject();

	void setPlayerMemoryLink(player* player) { _player = player; }
	void setEnemyMgrMemoryLink(enemyManager* enemyManager) { _enemyMgr = enemyManager; }
};

