#pragma once
#include "objects.h"


class movingTile : public objects
{
private:
	bool _isRight, _isUp;
	bool _isHorizon;		//true이면 가로로 움직인다. false이면 세로로 움직인다.

public:
	movingTile();
	~movingTile();

	virtual HRESULT init(OBJECTTYPE type, int x, int y);
	void release();
	void update();
	void render();

	void move();

	bool getIsRight() { return _isRight; }
};

