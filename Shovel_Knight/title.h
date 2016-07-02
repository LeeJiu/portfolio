#pragma once
#include "gameNode.h"


class title : public gameNode
{
public:
	title();
	~title();

	HRESULT init();
	void release();
	void update();
	void render();
};

