#pragma once

#include "gameNode.h"
#include "enemy.h"
#include <vector>

#include "boxboy.h"

class player;

enum ENEMYTYPE
{
	BOXBOY,
	CHOCOBEE,
	HIP,
	BOSS
};

class enemyManager : public gameNode
{
private:
	typedef vector<enemy*> vEnemy;
	typedef vEnemy::iterator viEnemy;

private:
	vEnemy _vEnemy;
	viEnemy _viEnemy;

	player* _player;

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();
	
	void setEnemy(ENEMYTYPE type, int x, int y);

	inline vector<enemy*> getVEnemy() { return _vEnemy; }
	inline vector<enemy*>::iterator getVIEnemy() { return _viEnemy; }

	void setPlayerMemoryLink(player* player) { _player = player; }
};

