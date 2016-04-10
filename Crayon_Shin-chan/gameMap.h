#pragma once
#include "gameNode.h"

class player;

class gameMap : public gameNode
{
private:
	image* _map;
	int _moveX;
	int _saveX;

	player* _player;

public:
	gameMap();
	~gameMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void move();	//맵 이동 제한해주자

	void setPlayerMemoryLink(player* player) { _player = player; }
};

