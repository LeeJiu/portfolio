#pragma once
#include "gameNode.h"


class clear : public gameNode
{
private:
	int _count;
	bool _start;
	
public:
	clear();
	~clear();

	HRESULT init();
	void release();
	void update();
	void render();
};

