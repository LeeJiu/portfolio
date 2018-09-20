#pragma once
#include "objects.h"


class chest : public objects
{
public:
	chest();
	~chest();

	HRESULT init(OBJECTTYPE type, int x, int y);
	void release();
	void update();
	void render();

	void setIsBroken(bool isBroken);
};

