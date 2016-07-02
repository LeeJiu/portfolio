#pragma once
#include "objects.h"


class block : public objects
{
public:
	block();
	~block();

	virtual HRESULT init(OBJECTTYPE type, int x, int y);
	void release();
	void update();
	void render();
};

