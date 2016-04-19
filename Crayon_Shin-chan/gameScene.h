#pragma once
#include "gameNode.h"


class gameScene : public gameNode
{
private:
	image* _sceneImage;
	SCENE _trigger;
public:

	gameScene();
	~gameScene();

	HRESULT init();
	void release();
	void update();
	void render();

	SCENE getTrigger() { return _trigger; }
	void setTrigger(SCENE trigger) { _trigger = trigger; }
};

