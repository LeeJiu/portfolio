#pragma once
#include "objects.h"


class ladder : public objects
{
public:
	ladder();
	~ladder();

	HRESULT init(OBJECTTYPE type, int x, int y);
	void release();
	void update();
	void render();
};

