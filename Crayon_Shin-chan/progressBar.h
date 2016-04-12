#pragma once

#include "gameNode.h"


class progressBar : public gameNode
{
private:
	image* _bgImage;
	image* _fgImage;
	RECT _bgRc, _fgRc;
	float _x, _y, _height;

	int _maxGauge;
	int _curGauge;
	float _percent;

public:
	progressBar();
	~progressBar();

	HRESULT init(const char* bgImgName, const char* fgImgName, int maxGauge, int curGauge);
	void release();
	void update();
	void render();

	void setBar(float x, float y, float width, float height);
	void setPosition(float x, float y);
	void decreaseBar(int gauge);
	void increaseBar(int gauge);

	image* getBgImage() { return _bgImage; }
};

