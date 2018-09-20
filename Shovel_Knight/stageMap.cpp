#include "stdafx.h"
#include "stageMap.h"
#include "player.h"
#include "objectManager.h"


stageMap::stageMap()
{
}


stageMap::~stageMap()
{
}

HRESULT stageMap::init(int stageNum)
{
	IMAGEMANAGER->addImage("sky", "image/background/sky.bmp", WINSIZEX, WINSIZEY, false, false);
	IMAGEMANAGER->addImage("backTree", "image/background/backTree.bmp", WINSIZEX, WINSIZEY, true, 0xff00ff);
	IMAGEMANAGER->addImage("frontTree", "image/background/frontTree.bmp", WINSIZEX, WINSIZEY, true, 0xff00ff);

	for (int i = 0; i < TILENUMY; ++i)
	{
		for (int j = 0; j < TILENUMX; ++j)
		{
			_tile[i][j].tileImage = NULL;
			_tile[i][j].pixelImage = NULL;
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
	int size = _objMgr->getVObject().size();
	for (int i = 0; i < size; ++i)
	{
		//화면이 넘어가면 부숴진 오브젝트 되살린다.
		if (_objMgr->getVObject()[i]->getIsBroken())
		{
			if (_objMgr->getVObject()[i]->getRect().right < _sourX
				|| _objMgr->getVObject()[i]->getX() > _sourX + WINSIZEX)
				_objMgr->getVObject()[i]->setIsBroken(false);
			else
				continue;
		}
	}
}

void stageMap::render()
{
	IMAGEMANAGER->render("sky", getMemDC(), _sourX, _sourY);
	IMAGEMANAGER->loopRender("backTree", getMemDC(), 
		&RectMake(_sourX, _sourY, _sourX + WINSIZEX, _sourY + WINSIZEY), _sourX - (_sourX/2), 0); 
	IMAGEMANAGER->loopRender("frontTree", getMemDC(),
			&RectMake(_sourX, _sourY, _sourX + WINSIZEX, _sourY + WINSIZEY), _sourX, 0);

	//화면에 보이는 타일
	for (int i = _sourY / TILESIZE; i < (_sourY + WINSIZEY) / TILESIZE; ++i)
	{
		for (int j = _sourX / TILESIZE; j <= (_sourX + WINSIZEX) / TILESIZE; ++j)
		{
			if (_tile[i][j].tileImage == NULL) continue;
			else
			{
				_tile[i][j].tileImage->render(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i][j].imagePos.x * TILESIZE, _tile[i][j].imagePos.y * TILESIZE, TILESIZE, TILESIZE);

				_tile[i][j].pixelImage->render(getPixelDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i][j].imagePos.x * TILESIZE, _tile[i][j].imagePos.y * TILESIZE, TILESIZE, TILESIZE);
			}
		}
	}
}

void stageMap::load(int stageNum)
{
	char str[128], str2[128], str3[128], str4[128], str5[128], temp[150000];

	//타일 데이터 불러오기
	sprintf_s(str, "Stage%d.txt", stageNum);
	sprintf_s(str2, "Stage%d", stageNum);		//타일 이미지 이름
	sprintf_s(str3, "Stage%d_pixel", stageNum);	//픽셀 이미지 이름
	sprintf_s(str4, "Stage%d_bg", stageNum);
	sprintf_s(str5, "Stage%d_bgpixel", stageNum);
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
		else if (dataOrder % 3 == 2)	//2번째 데이터 tile
		{
			int imageNum = atoi(temp);
			if (_tile[index / TILENUMX][index % TILENUMX].type == NONE)
			{
				_tile[index / TILENUMX][index % TILENUMX].tileImage = IMAGEMANAGER->findImage(str2);
				_tile[index / TILENUMX][index % TILENUMX].pixelImage = IMAGEMANAGER->findImage(str3);
			}
			else if (_tile[index / TILENUMX][index % TILENUMX].type == BACKGROUND)
			{
				_tile[index / TILENUMX][index % TILENUMX].tileImage = IMAGEMANAGER->findImage(str4);
				_tile[index / TILENUMX][index % TILENUMX].pixelImage = IMAGEMANAGER->findImage(str5);
			}

			_tile[index / TILENUMX][index % TILENUMX].imagePos.x = imageNum % 3;
			_tile[index / TILENUMX][index % TILENUMX].imagePos.y = imageNum / 3;
		}
		dataOrder++;
	}
}

void stageMap::moveMap(float x, float y)
{
	//좌우 맵 이동
	_sourX = x - (WINSIZEX / 2);
	if (_sourX < 0)
		_sourX = 0;
	else if (_sourX > WINSIZEX * (PAGENUMX - 1))
		_sourX = WINSIZEX * (PAGENUMX - 1);

	//상하 맵 이동
	if (y < _sourY + 80)
		_sourY = ((_sourY / WINSIZEY) - 1) * WINSIZEY;
	else if (y > _sourY + WINSIZEY)
		_sourY += WINSIZEY;

	if (_sourY < 0)
		_sourY = 0;
	else if (_sourY > WINSIZEY * (PAGENUMY - 1))
		_sourY = WINSIZEY * (PAGENUMY - 1);
}
