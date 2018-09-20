#pragma once
#include "objects.h"


class movingTile : public objects
{
private:
	bool _isRight, _isUp;
	bool _isHorizon;		//true�̸� ���η� �����δ�. false�̸� ���η� �����δ�.

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

