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

	void move();	//�� �̵� ����������

	void setPlayerMemoryLink(player* player) { _player = player; }
};

