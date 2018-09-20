#pragma once
#include "gameNode.h"
class stateUI : public gameNode
{
private:
	image* _dialog;
	image* _relic;

	vector<image*> _vSp;
	vector<image*>::iterator _viSp;

	vector<image*> _vGold;
	vector<image*>::iterator _viGold;

	vector<image*> _vLife;
	vector<image*>::iterator _viLife;

	vector<image*> _vBoss;
	vector<image*>::iterator _viBoss;

	int _sp, _gold, _life, _boss, _maxLife, _maxBoss;

public:
	stateUI();
	~stateUI();

	HRESULT init();
	void release();
	void update();
	void render();

	void skillPoint();
	void lifePoint();
	void bossPoint();
	void gold();

	void setSp(int sp) { _sp = sp; }
	void setGold(int gold) { _gold = gold; }
	void setLife(int lifePoint) { _life = lifePoint; }
	void setBoss(int lifePoint) { _boss = lifePoint; }
	void setMaxLife(int lifePoint) { _maxLife = lifePoint; }
	void setMaxBoss(int lifePoint) { _maxBoss = lifePoint; }
};

