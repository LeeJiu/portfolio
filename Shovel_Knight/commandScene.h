#pragma once

#include "gameNode.h"

class commandScene : public gameNode
{
private:
	enum eKey
	{
		key_left,
		key_up,
		key_right,
		key_down
	};

	string _str;

	vector<BYTE> _vCommand;

	float _fCommandTime;
	float _fCommandClear;

public:
	commandScene();
	~commandScene();

	HRESULT init();
	void release();
	void update();
	void render();

	void command();
};

