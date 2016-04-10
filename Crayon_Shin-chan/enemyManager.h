#pragma once

#include "gameNode.h"
#include "enemy.h"
#include <vector>

class player;

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
	
	void setEnemy();
	void removeEnemy(int arrNum);
	void collision();					//에너미들의 충돌 체크 함수를 부르자

	inline vector<enemy*> getVEnemy() { return _vEnemy; }
	inline vector<enemy*>::iterator getVIEnemy() { return _viEnemy; }

	void setPlayerMemoryLink(player* player) { _player = player; }
};

