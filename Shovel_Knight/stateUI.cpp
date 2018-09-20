#include "stdafx.h"
#include "stateUI.h"


stateUI::stateUI()
{
}


stateUI::~stateUI()
{
}

HRESULT stateUI::init()
{
	IMAGEMANAGER->addImage("ui_state", "image/ui/ui_state.bmp", 800, 64, false, false);

	//플레이어 라이프 포인트
	_vLife.reserve(20);
	for (int i = 0; i < _maxLife / 2; ++i)
	{
		image* lifePoint;
		lifePoint = new image;
		lifePoint->init("image/ui/ui_life.bmp", 75, 26, 3, 1, true, 0xff00ff);
		lifePoint->setFrameX(0);
		_vLife.push_back(lifePoint);
	}

	//보스 라이프 포인트
	_vBoss.reserve(20);
	for (int i = 0; i < _maxBoss / 2; ++i)
	{
		image* lifePoint;
		lifePoint = new image;
		lifePoint->init("image/ui/ui_boss.bmp", 75, 26, 3, 1, true, 0xff00ff);
		lifePoint->setFrameX(0);
		_vBoss.push_back(lifePoint);
	}

	//골드
	if (_gold <= 0)
	{
		image* gold;
		gold = new image;
		gold->init("image/ui/ui_number.bmp", 120, 18, 10, 1, true, 0xff00ff);
		gold->setFrameX(0);
		_vGold.push_back(gold);
	}
	while (_gold > 0)
	{
		image* gold;
		gold = new image;
		gold->init("image/ui/ui_number.bmp", 120, 18, 10, 1, true, 0xff00ff);
		gold->setFrameX(_gold % 10);
		_vGold.push_back(gold);

		_gold *= 0.1;
	}

	//스킬 포인트
	if (_sp <= 0)
	{
		image* sp;
		sp = new image;
		sp->init("image/ui/ui_number_sp.bmp", 120, 18, 10, 1, true, 0xff00ff);
		sp->setFrameX(0);
		_vSp.push_back(sp);
	}
	while (_sp > 0)
	{
		image* sp;
		sp = new image;
		sp->init("image/ui/ui_number_sp.bmp", 120, 18, 10, 1, true, 0xff00ff);
		sp->setFrameX(_sp % 10);
		_vSp.push_back(sp);

		_sp *= 0.1;
	}

	return S_OK;
}

void stateUI::release()
{
	int size = _vSp.size();
	for (int i = 0; i < size; ++i)
	{
		_vSp[i]->release();
		SAFE_DELETE(_vSp[i]);
	}
	_vSp.clear();

	size = _vGold.size();
	for (int i = 0; i < size; ++i)
	{
		_vGold[i]->release();
		SAFE_DELETE(_vGold[i]);
	}
	_vGold.clear();

	for (int i = 0; i < _maxLife / 2; ++i)
	{
		_vLife[i]->release();
		SAFE_DELETE(_vLife[i]);
	}
	_vLife.clear();

	for (int i = 0; i < _maxBoss / 2; ++i)
	{
		_vBoss[i]->release();
		SAFE_DELETE(_vBoss[i]);
	}
	_vBoss.clear();
}

void stateUI::update()
{
	skillPoint();
	lifePoint();
	bossPoint();
	gold();
}

void stateUI::render()
{
	IMAGEMANAGER->render("ui_state", getMemDC(), _sourX, _sourY);

	int size = _vSp.size();
	for (int i = 0; i < size; ++i)
	{
		_vSp[size - 1 - i]->frameRender(getMemDC(), _sourX + 182 + (i * 13), _sourY + 35);
	}

	size = _vGold.size();
	for (int i = 0; i < size; ++i)
	{
		_vGold[size - 1 - i]->frameRender(getMemDC(), _sourX + 24 + (i * 13), _sourY + 35);
	}

	for (int i = 0; i < _maxLife / 2; ++i)
	{
		_vLife[i]->frameRender(getMemDC(), _sourX + 300 + (i * 28), _sourY + 30);
	}

	
	for (int i = 0; i < _maxBoss / 2; ++i)
	{
		_vBoss[i]->frameRender(getMemDC(), _sourX + 685 + (i * 28), _sourY + 30);
	}
	
}

void stateUI::skillPoint()
{
	int number = 0;
	int spNum = _sp;
	while (spNum > 0)
	{
		number++;
		spNum *= 0.1;
	}

	int size = _vSp.size();
	if (number >= size)
	{
		int size2 = number - size;

		for (int i = 0; i < size; ++i)
		{
			_vSp[i]->setFrameX(_sp % 10);
			_sp *= 0.1;
		}

		for (int i = 0; i < size2; ++i)
		{
			image* sp;
			sp = new image;
			sp->init("image/ui/ui_number_sp.bmp", 120, 18, 10, 1, true, 0xff00ff);
			sp->setFrameX(_sp % 10);
			_vSp.push_back(sp);

			_sp *= 0.1;
		}
	}
	else if (number < size)
	{
		int size2 = size - number;

		for (int i = 0; i < number; ++i)
		{
			_vSp[i]->setFrameX(_sp % 10);
			_sp *= 0.1;
		}

		for (int i = 0; i < size2; ++i)
		{
			_vSp.back()->release();
			SAFE_DELETE(_vSp.back());
			_vSp.pop_back();
		}
	}
}

void stateUI::lifePoint()
{
	if (_life < 0) return;

	for (int i = 0; i < _life / 2; ++i)
	{
		_vLife[i]->setFrameX(0);
	}

	int emptyLife = _maxLife - _life;

	for (int i = 1; i <= emptyLife / 2; ++i)
	{
		_vLife[(_maxLife / 2) - i]->setFrameX(2);
	}

	if (emptyLife % 2 == 1)
	{
		int halfLife = emptyLife / 2;
		_vLife[(_maxLife / 2) - halfLife - 1]->setFrameX(1);
	}
}

void stateUI::bossPoint()
{
	if (_boss < 0) return;

	for (int i = 0; i < _boss / 2; ++i)
	{
		_vBoss[i]->setFrameX(0);
	}

	int emptyLife = _maxBoss - _boss;

	for (int i = 1; i <= emptyLife / 2; ++i)
	{
		_vBoss[(_maxBoss / 2) - i]->setFrameX(2);
	}

	if (emptyLife % 2 == 1)
	{
		int halfLife = emptyLife / 2;
		_vBoss[(_maxBoss / 2) - halfLife - 1]->setFrameX(1);
	}
}

void stateUI::gold()
{
	int number = 0;
	int goldNum = _gold;
	while (goldNum > 0)
	{
		number++;
		goldNum *= 0.1;
	}

	int size = _vGold.size();
	if (number >= size)
	{
		int size2 = number - size;
		
		for (int i = 0; i < size; ++i)
		{
			_vGold[i]->setFrameX(_gold % 10);
			_gold *= 0.1;
		}

		for(int i = 0; i < size2; ++i)
		{
			image* gold;
			gold = new image;
			gold->init("image/ui/ui_number.bmp", 120, 18, 10, 1, true, 0xff00ff);
			gold->setFrameX(_gold % 10);
			_vGold.push_back(gold);

			_gold *= 0.1;
		}
	}
	else if (number < size)
	{
		int size2 = size - number;

		for (int i = 0; i < number; ++i)
		{
			_vGold[i]->setFrameX(_gold % 10);
			_gold *= 0.1;
		}

		for (int i = 0; i < size2; ++i)
		{
			_vGold.back()->release();
			SAFE_DELETE(_vGold.back());
			_vGold.pop_back();
		}
	}
}
