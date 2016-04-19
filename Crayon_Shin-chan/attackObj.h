#pragma once
#include "gameNode.h"

enum OBJSTATE
{
	MOVE,
	BOMB
};

struct tagAttObj
{
	image* image;
	OBJSTATE state;
	RECT coll;
	POINT pt, fire;
	float speed;
	bool isRight;
	bool isFire;
};

class attackObj : public gameNode
{
private:
	vector<tagAttObj> _vAttObj;
	vector<tagAttObj>::iterator _viAttObj;

	float _range;
	int _curFrameX, _curFrameY, _count;

public:
	attackObj();
	~attackObj();

	HRESULT init(int objMax, float range);
	void release();
	void update();
	void render();

	void fire(int x, int y, bool isRight);
	void move();

	void setImage();
	void setFrame();

	vector<tagAttObj> getVAttObj() { return _vAttObj; }
	vector<tagAttObj>::iterator getVIAttObj() { return _viAttObj; }
};

