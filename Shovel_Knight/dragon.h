#pragma once
#include "enemy.h"
#include "bubbleShot.h"


class dragon : public enemy
{
private:
	RECT _coll2, _atk;
	int _awareW, _awareH, _atkRange, _atk2Range, _dist;
	float _unbeatCnt;

	//bubble shot
	bubbleShot _bubbles[3];

public:
	dragon();
	~dragon();

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();

	void aware();
	void attack();
	void deadMotion();
	void collision();

	void setIsDead(bool isDead);

	void setImage();
	void setFrame();

	void setRect();
};

