#pragma once

#include "gameNode.h"
#include "title.h"
#include "maptool.h"
#include "stage.h"
#include "gameOver.h"
#include "clear.h"

class gameStudy : public gameNode
{
private:
	bool _isPixel;

public:
	gameStudy();
	virtual ~gameStudy();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

};

