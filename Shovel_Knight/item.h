#pragma once

class image;

class item
{
private:
	image* _image;
	RECT _rc;
	float _x, _y;
	float _jumpPwr, _gravity, _count, _time;
	int _imgFrameX, _gold;
	bool _isBounce, _isRight, _isOn;
	ITEMTYPE _type;

public:
	item();
	~item();

	HRESULT init(ITEMTYPE type, float x, float y, bool isBounce, bool isRight);
	void release();
	void update();
	void render();

	void bounce();
	void pixelCollision();

	void setRect(float x, float y);
	void setIsOn(bool isOn) { _isOn = isOn; }

	int getGold() { return _gold; }
	bool getIsOn() { return _isOn; }
	RECT getRect() { return _rc; }
	ITEMTYPE getType() { return _type; }
};

