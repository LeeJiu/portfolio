#pragma once
#include "gameNode.h"


class title : public gameNode
{
private:
	image* _cursor;
	int _cursorX, _cursorY, _cursorDist, _selectScene;
	bool _select;
	bool _start;

	int _btAlpha[2];
	int _count, _curFrameX;

public:
	title();
	~title();

	HRESULT init();
	void release();
	void update();
	void render();

	void keyControl();
	void setFrame();
	void moveCursor();
	void moveScene();
};

