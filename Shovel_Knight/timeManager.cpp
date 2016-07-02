#include "stdafx.h"
#include "timeManager.h"


timeManager::timeManager() : _timer(NULL)
{
}


timeManager::~timeManager()
{
}

HRESULT timeManager::init()
{
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release()
{
	if (_timer != NULL)
	{
		SAFE_DELETE(_timer);
	}
}

void timeManager::update(float lock /*= 0.0f*/)
{
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

void timeManager::render(HDC hdc)
{
	char str[256];
	string strFrame;

	//���� ����� ���� ( TRANSPARENT : ����, QPAQUEL : ������ )
	SetBkMode(hdc, TRANSPARENT);

	//���� ����
	SetTextColor(hdc, RGB(255, 255, 255));

#ifdef _DEBUG
	{
		if (_timer != NULL)
		{
			//������ �������
			sprintf_s(str, "framePerSec : %d", _timer->getFrameRate());
			TextOut(hdc, _sourX, _sourY, str, strlen(str));

			//���� Ÿ�� �������
			sprintf_s(str, "worldTime : %f", _timer->getWorldTime());
			TextOut(hdc, _sourX, _sourY + 20, str, strlen(str));

			//���� Tick �������
			sprintf_s(str, "elapsedTime : %f", _timer->getElapsedTime());
			TextOut(hdc, _sourX, _sourY + 40, str, strlen(str));
		}
	}
#else
	{
		if (_timer != NULL)
		{
			sprintf_s(str, "framePerSec : %d", _timer->getFrameRate());
			TextOut(hdc, 0, 0, str, strlen(str));
		}
	}
#endif

}