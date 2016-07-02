#include "stdafx.h"
#include "stageMap.h"


stageMap::stageMap()
{
}


stageMap::~stageMap()
{
}

HRESULT stageMap::init(int stageNum)
{
	for (int i = 0; i < TILENUMY; ++i)
	{
		for (int j = 0; j < TILENUMX; ++j)
		{
			_tile[i][j].tileImage = NULL;
			_tile[i][j].imagePos.x = 0;
			_tile[i][j].imagePos.y = 0;
			_tile[i][j].type = NONE;
			_tile[i][j].index = i * TILENUMX + j;
			_tile[i][j].x = j * TILESIZE;
			_tile[i][j].y = i * TILESIZE;
			_tile[i][j].rc = RectMake(_tile[i][j].x, _tile[i][j].y, TILESIZE, TILESIZE);
		}
	}

	load(stageNum);

	return S_OK;
}

void stageMap::release()
{
}

void stageMap::update()
{
	//moveMap();
}

void stageMap::render()
{
	//화면에 보이는 타일
	for (int i = _sourY / TILESIZE; i < (_sourY + WINSIZEY) / TILESIZE; ++i)
	{
		for (int j = _sourX / TILESIZE; j < (_sourX + WINSIZEX) / TILESIZE; ++j)
		{
			if (_tile[i][j].tileImage == NULL) continue;
			else
			{
				_tile[i][j].tileImage->render(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i][j].imagePos.x * TILESIZE, _tile[i][j].imagePos.y * TILESIZE, TILESIZE, TILESIZE);

				if (_tile[i][j].type == LADDER)
				{
					//Rectangle(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					//	_tile[i][j].rc.right, _tile[i][j].rc.bottom);

					char str[128];
					sprintf_s(str, "%d, %d", j, i);
					TextOut(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top, str, strlen(str));
				}
			}
		}
	}
}

void stageMap::load(int stageNum)
{
	char str[128], str2[128], temp[150000];

	//타일 데이터 불러오기
	sprintf_s(str, "Stage%d.txt", stageNum);
	sprintf_s(str2, "Stage%d", stageNum);	//이미지 이름
	ifstream loadTile(str);

	int dataOrder = 0;		//데이터 순서(개수)
	int index = 0;
	while (!loadTile.eof())
	{
		loadTile.getline(temp, 100);
		if (dataOrder % 3 == 0)			//0번째 데이터 index
		{
			index = atoi(temp);
		}
		else if (dataOrder % 3 == 1)	//1번째 데이터 type
		{
			_tile[index / TILENUMX][index % TILENUMX].type = (TILETYPE)atoi(temp);
		}
		else if (dataOrder % 3 == 2)	//2번째 데이터 imageNum
		{
			if (_tile[index / TILENUMX][index % TILENUMX].type == NONE
				|| _tile[index / TILENUMX][index % TILENUMX].type == ONOBJECT
				|| _tile[index / TILENUMX][index % TILENUMX].type == ONENEMY)
			{
				dataOrder++;
				continue;
			}

			int imageNum = atoi(temp);
			_tile[index / TILENUMX][index % TILENUMX].tileImage = IMAGEMANAGER->findImage(str2);
			_tile[index / TILENUMX][index % TILENUMX].imagePos.x = imageNum % 3;
			_tile[index / TILENUMX][index % TILENUMX].imagePos.y = imageNum / 3;
		}
		dataOrder++;
	}
}

void stageMap::moveMap()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		if (_sourX > 0)
			_sourX -= WINSIZEX;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		if (_sourX < (PAGENUMX - 1) * WINSIZEX)
			_sourX += WINSIZEX;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (_sourY > 0)
			_sourY -= WINSIZEY;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (_sourY < (PAGENUMY - 1) * WINSIZEY)
			_sourY += WINSIZEY;
	}
}
