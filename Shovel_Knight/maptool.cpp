#include "stdafx.h"
#include "maptool.h"


maptool::maptool()
{
}


maptool::~maptool()
{
}

HRESULT maptool::init()
{
	_ui = new maptoolUI;
	_ui->init();

	_vTile.reserve(TILENUMX * TILENUMY);

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
			_vTile.push_back(&_tile[i][j]);
		}
	}	

	return S_OK;
}

void maptool::release()
{
	_ui->release();
	SAFE_DELETE(_ui);
}

void maptool::update()
{
	_nStage = _ui->getStageNum();

	keyControl();
	clickTile();
	
	_ui->update();
}

void maptool::render()
{
	HPEN oldPen, myPen;
	myPen = CreatePen(PS_DOT, 1, 0xffffff);
	oldPen = (HPEN)SelectObject(getMemDC(), myPen);
	SetBkMode(getMemDC(), TRANSPARENT);

	//타일 경계선
	for (int i = 0; i < WINSIZEX / TILESIZE; ++i)
	{
		LineMake(getMemDC(), _sourX + (i * TILESIZE), _sourY, _sourX + (i * TILESIZE), _sourY + WINSIZEY);
	}
	for (int i = 0; i < WINSIZEY / TILESIZE; ++i)
	{
		LineMake(getMemDC(), _sourX, _sourY + (i * TILESIZE), _sourX + WINSIZEX, _sourY + (i * TILESIZE));
	}

	//화면에 보이는 타일
	for (int i = _sourY / TILESIZE; i < (_sourY + WINSIZEY) / TILESIZE; ++i)
	{
		for (int j = _sourX / TILESIZE; j < (_sourX + WINSIZEX) / TILESIZE; ++j)
		{
			//if (_viewColl)
			//{
			//	//if (_tile[i][j].imagePos.x == 0 && _tile[i][j].imagePos.y == 0) continue;

			//	switch (_tile[i][j].type)
			//	{
			//	case NONE:
			//		TextOut(getMemDC(), _tile[i][j].rc.left + 2, _tile[i][j].rc.top, TEXT("N"), strlen("N"));
			//		break;
			//	}
			//}
			/*else*/ if (_tile[i][j].tileImage == NULL) continue;
			else
			{
				_tile[i][j].tileImage->render(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i][j].imagePos.x * TILESIZE, _tile[i][j].imagePos.y * TILESIZE, TILESIZE, TILESIZE);
				
				_tile[i][j].pixelImage->render(getPixelDC(), _tile[i][j].rc.left, _tile[i][j].rc.top,
					_tile[i][j].imagePos.x * TILESIZE, _tile[i][j].imagePos.y * TILESIZE, TILESIZE, TILESIZE);
			}
		}
	}

	//오브젝트 render
	for (int i = 0; i < OBJNUM; ++i)
	{
		for (_liObjList = _lObjList[i].begin(); _liObjList != _lObjList[i].end(); ++_liObjList)
		{
			if (_liObjList->x < _sourX || _liObjList->x > _sourX + WINSIZEX) continue;
			if (_liObjList->y < _sourY || _liObjList->y > _sourY + WINSIZEY) continue;

			//오브젝트 그리기
			_liObjList->objImage->render(getMemDC(), _liObjList->x, _liObjList->y);
		}
	}

	//에너미 render
	for (int i = 0; i < ENEMYNUM; ++i)
	{
		for (_liEnemy = _lEnemy[i].begin(); _liEnemy != _lEnemy[i].end(); ++_liEnemy)
		{
			if (_liEnemy->x < _sourX || _liEnemy->x > _sourX + WINSIZEX) continue;
			if (_liEnemy->y < _sourY || _liEnemy->y > _sourY + WINSIZEY) continue;

			//에너미 그리기
			_liEnemy->enemyImage->frameRender(getMemDC(), _liEnemy->x, _liEnemy->y, 0, 0);
		}
	}

	//UI
	_ui->render();

	SelectObject(getMemDC(), oldPen);
	DeleteObject(myPen);
}

void maptool::keyControl()
{
	moveWorkspace();

	//타일의 충돌 여부 확인
	if (KEYMANAGER->isOnceKeyDown('C'))
		_viewColl = !_viewColl;
	//타일 리셋
	if (KEYMANAGER->isOnceKeyDown('R'))
		reset();
	//현재 스테이지/페이즈 저장
	if (KEYMANAGER->isOnceKeyDown('S'))
		save();
	//선택한 스테이지_페이즈 불러오기
	if (KEYMANAGER->isOnceKeyDown('L'))
		load();
}

void maptool::moveWorkspace()
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

void maptool::clickTile()
{
	if (_ui->getOnSelTile())
	{
		//타일 그리기
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			if (_ui->getOnUI())
			{
				if (_click.x > _sourX + 671) return;
			}
			if (_click.y < _sourY + 64) return;

			setTile();
		}

		//빈 타일로
		if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
		{
			int tileX = _click.x / TILESIZE;
			int tileY = _click.y / TILESIZE;

			_tile[tileY][tileX].tileImage = NULL;
			_tile[tileY][tileX].pixelImage = NULL;
			_tile[tileY][tileX].imagePos.x = 0;
			_tile[tileY][tileX].imagePos.y = 0;
			_tile[tileY][tileX].type = NONE;
		}
	}
	else if (_ui->getOnSelObj())
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (_ui->getOnUI())
			{
				if (_click.x > _sourX + 671) return;
			}
			if (_click.y < _sourY + 64) return;

			setObject();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
		{
			deleteObject();
		}
	}
	else if (_ui->getOnSelChar())
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (_ui->getOnUI())
			{
				if (_click.x > _sourX + 671) return;
			}
			if (_click.y < _sourY + 64) return;

			setCharacter();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
		{
			deleteEnemy();
		}
	}
}

void maptool::setTile()
{
	int tileX = _click.x / TILESIZE;
	int tileY = _click.y / TILESIZE;

	//오브젝트가 있는 타일이라면 타일을 변경하지 못한다.
	//if (_tile[tileY][tileX].type == ONOBJECT) return;

	_tile[tileY][tileX].tileImage = _ui->getTileImage();
	_tile[tileY][tileX].pixelImage = _ui->getPixelImage();
	_tile[tileY][tileX].imagePos.x = _ui->getImageNumX();
	_tile[tileY][tileX].imagePos.y = _ui->getImageNumY();
	_tile[tileY][tileX].type = _ui->getTileType();
}

void maptool::setObject()
{
	int tileX = _click.x / TILESIZE;
	int tileY = _click.y / TILESIZE;

	const char* objectName = _ui->getObjName();

	//오브젝트가 있는 타일에 상태를 변경
	int width = IMAGEMANAGER->findImage(objectName)->getWidth() / TILESIZE;
	int height = IMAGEMANAGER->findImage(objectName)->getHeight() / TILESIZE;

	//에너미나 오브젝트가 있는 타일이라면 타일을 변경하지 못한다.
	//for (int i = tileX; i < tileX + width; ++i)
	//{
	//	for (int j = tileY; j > tileY - height; --j)
	//	{
	//		if (_tile[j][i].type == ONOBJECT /*|| _tile[j][i].type == ONENEMY*/) return;
	//	}
	//}

	tagObject obj;
	obj.type = _ui->getObjectType();
	obj.objImage = IMAGEMANAGER->findImage(objectName);
	obj.x = _tile[tileY][tileX].rc.left;
	obj.y = _tile[tileY][tileX].rc.bottom - obj.objImage->getHeight();
	obj.rc = RectMake(obj.x, obj.y, obj.objImage->getWidth(), obj.objImage->getHeight());

	_lObjList[obj.type].push_back(obj);

	//오브젝트가 있는 타일의 상태를 변경
	/*for (int i = tileX; i < tileX + width; ++i)
	{
		for (int j = tileY; j > tileY - height; --j)
		{
			_tile[j][i].type = ONOBJECT;
		}
	}*/
}

void maptool::setCharacter()
{
	int tileX = _click.x / TILESIZE;
	int tileY = _click.y / TILESIZE;

	const char* enemyName = _ui->getEnemyName();

	//에너미가 있는 타일에 상태를 변경
	int width = IMAGEMANAGER->findImage(enemyName)->getFrameWidth() / TILESIZE + 1;
	int height = IMAGEMANAGER->findImage(enemyName)->getFrameHeight() / TILESIZE + 1;

	//에너미나 오브젝트가 있는 타일이라면 타일을 변경하지 못한다.
	//for (int i = tileX; i < tileX + width; ++i)
	//{
	//	for (int j = tileY; j > tileY - height; --j)
	//	{
	//		if (_tile[j][i].type == ONOBJECT /*|| _tile[j][i].type == ONENEMY*/) return;
	//	}
	//}

	tagEnemy enemy;
	enemy.type = _ui->getEnemyType();
	enemy.enemyImage = IMAGEMANAGER->findImage(enemyName);
	enemy.x = _tile[tileY][tileX].rc.left;
	enemy.y = _tile[tileY][tileX].rc.bottom - enemy.enemyImage->getFrameHeight();
	enemy.rc = RectMake(enemy.x, enemy.y, enemy.enemyImage->getFrameWidth(), enemy.enemyImage->getFrameHeight());

	_lEnemy[enemy.type].push_back(enemy);

	//에너미가 있는 타일의 상태를 변경
	/*for (int i = tileX; i < tileX + width; ++i)
	{
		for (int j = tileY; j > tileY - height; --j)
		{
			_tile[j][i].type = ONENEMY;
		}
	}*/
}

void maptool::deleteObject()
{
	OBJECTTYPE type = _ui->getObjectType();

	if (_lObjList[type].empty()) return;

	for (_liObjList = _lObjList[type].begin(); _liObjList != _lObjList[type].end(); ++_liObjList)
	{
		if (_liObjList->x < _sourX || _liObjList->x > _sourX + WINSIZEX) continue;
		if (_liObjList->y < _sourY || _liObjList->y > _sourY + WINSIZEY) continue;

		if (PtInRect(&_liObjList->rc, _click))
		{
			int tileX = _liObjList->x / TILESIZE;
			int tileY = _liObjList->y / TILESIZE;

			//오브젝트가 있는 타일의 상태를 변경
			int width = _liObjList->objImage->getWidth() / TILESIZE;
			int height = _liObjList->objImage->getHeight() / TILESIZE;

			for (int i = tileX; i < tileX + width; ++i)
			{
				for (int j = tileY; j < tileY + height; ++j)
				{
					_tile[j][i].type = NONE;
				}
			}

			_liObjList = _lObjList[type].erase(_liObjList);
			break;
		}
	}
}

void maptool::deleteEnemy()
{
	ENEMYTYPE type = _ui->getEnemyType();

	if (_lEnemy[type].empty()) return;

	for (_liEnemy = _lEnemy[type].begin(); _liEnemy != _lEnemy[type].end(); ++_liEnemy)
	{
		if (_liEnemy->x < _sourX || _liEnemy->x > _sourX + WINSIZEX) continue;
		if (_liEnemy->y < _sourY || _liEnemy->y > _sourY + WINSIZEY) continue;

		if (PtInRect(&_liEnemy->rc, _click))
		{
			int tileX = _liEnemy->x / TILESIZE;
			int tileY = _liEnemy->y / TILESIZE;

			//에너미가 있는 타일의 상태를 변경
			int width = _liEnemy->enemyImage->getFrameWidth() / TILESIZE + 1;
			int height = _liEnemy->enemyImage->getFrameHeight() / TILESIZE + 1;

			for (int i = tileX; i < tileX + width; ++i)
			{
				for (int j = tileY - 1; j < tileY + height; ++j)
				{
					_tile[j][i].type = NONE;
				}
			}

			_liEnemy = _lEnemy[type].erase(_liEnemy);
			break;
		}
	}
}

void maptool::reset()
{
	_vTile.clear();

	//다시 타일 초기화
	for (int i = 0; i < TILENUMY; ++i)
	{
		for (int j = 0; j < TILENUMX; ++j)
		{
			_tile[i][j].tileImage = NULL;
			_tile[i][j].pixelImage = NULL;
			_tile[i][j].imagePos.x = 0;
			_tile[i][j].imagePos.y = 0;
			_tile[i][j].type = NONE;
			_tile[i][j].x = j * TILESIZE;
			_tile[i][j].y = i * TILESIZE;
			_tile[i][j].rc = RectMake(_tile[i][j].x, _tile[i][j].y, TILESIZE, TILESIZE);
			_vTile.push_back(&_tile[i][j]);
		}
	}

	//오브젝트 리셋
	for (int i = 0; i < OBJNUM; ++i)
	{
		_lObjList[i].clear();
	}

	//에너미 리셋
	for (int i = 0; i < ENEMYNUM; ++i)
	{
		_lEnemy[i].clear();
	}
}

void maptool::save()
{
	char temp[128], str[128];

	//타일 저장
	sprintf_s(str, "Stage%d.txt", _nStage);
	ofstream saveTile(str);
	for (_viTile = _vTile.begin(); _viTile != _vTile.end(); ++_viTile)
	{
		int imageNum = (*_viTile)->imagePos.y * 3 + (*_viTile)->imagePos.x;

		saveTile << itoa((*_viTile)->index, temp, 10) << endl;
		saveTile << itoa((*_viTile)->type, temp, 10) << endl;
		saveTile << itoa(imageNum, temp, 10) << endl;
	}
	saveTile.close();

	//오브젝트 저장
	sprintf_s(str, "Stage%d_obj.txt", _nStage);
	ofstream saveObj(str);
	int num = 0;
	for (int i = 0; i < OBJNUM; ++i)
	{
		for (_liObjList = _lObjList[i].begin(); _liObjList != _lObjList[i].end(); ++_liObjList)
		{
			saveObj << itoa(_liObjList->type, temp, 10) << endl;
			saveObj << itoa(_liObjList->x, temp, 10) << endl;
			saveObj << itoa(_liObjList->y, temp, 10) << endl;
		}
	}
	saveObj.close();

	//에너미 저장
	sprintf_s(str, "Stage%d_enemy.txt", _nStage);
	ofstream saveEnemy(str);
	num = 0;
	for (int i = 0; i < ENEMYNUM; ++i)
	{
		for (_liEnemy = _lEnemy[i].begin(); _liEnemy != _lEnemy[i].end(); ++_liEnemy)
		{
			saveEnemy << itoa(_liEnemy->type, temp, 10) << endl;
			saveEnemy << itoa(_liEnemy->x, temp, 10) << endl;
			saveEnemy << itoa(_liEnemy->y, temp, 10) << endl;
		}
	}
	saveEnemy.close();
}

void maptool::load()
{
	char str[128], str2[128], str3[128], str4[128], str5[128], temp[150000];

	//타일 데이터 불러오기
	sprintf_s(str, "Stage%d.txt", _nStage);
	sprintf_s(str2, "Stage%d", _nStage);		//타일 이미지 이름
	sprintf_s(str3, "Stage%d_pixel", _nStage);	//픽셀 이미지 이름
	sprintf_s(str4, "Stage%d_bg", _nStage);
	sprintf_s(str5, "Stage%d_bgpixel", _nStage);
	ifstream loadTile(str);

	//타일 리셋
	reset();

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

	//오브젝트 데이터 불러오기
	sprintf_s(str, "Stage%d_obj.txt", _nStage);
	ifstream loadObj(str);

	//오브젝트 리셋
	for (int i = 0; i < OBJNUM; ++i)
	{
		_lObjList[i].clear();
	}

	dataOrder = 0;		//데이터 순서(개수)
	tagObject obj;
	while (!loadObj.eof())
	{
		loadObj.getline(temp, 100);
		if (dataOrder % 3 == 0)			//0번째 데이터 type
		{
			obj.type = (OBJECTTYPE)atoi(temp);
			selectObjectName(obj.type);
			obj.objImage = IMAGEMANAGER->findImage(_objectName);
		}
		else if (dataOrder % 3 == 1)	//1번째 데이터 x
		{
			obj.x = atoi(temp);
		}
		else if (dataOrder % 3 == 2)	//2번째 데이터 y
		{
			obj.y = atoi(temp);
			pushObject(obj);
		}
		dataOrder++;
	}

	//에너미 데이터 불러오기
	sprintf_s(str, "Stage%d_enemy.txt", _nStage);
	ifstream loadEnemy(str);

	//에너미 리셋
	for (int i = 0; i < ENEMYNUM; ++i)
	{
		_lEnemy[i].clear();
	}

	dataOrder = 0;		//데이터 순서(개수)
	tagEnemy enemy;
	while (!loadEnemy.eof())
	{
		loadEnemy.getline(temp, 100);
		if (dataOrder % 3 == 0)			//0번째 데이터 type
		{
			enemy.type = (ENEMYTYPE)atoi(temp);
			selectEnemyName(enemy.type);
			enemy.enemyImage = IMAGEMANAGER->findImage(_enemyName);
		}
		else if (dataOrder % 3 == 1)	//1번째 데이터 x
		{
			enemy.x = atoi(temp);
		}
		else if (dataOrder % 3 == 2)	//2번째 데이터 y
		{
			enemy.y = atoi(temp);
			pushEnemy(enemy);
		}
		dataOrder++;
	}
}

void maptool::selectObjectName(OBJECTTYPE type)
{
	switch (type)
	{
	case BLOCK_SMALL:
		_objectName = "smallBlock";
		break;
	case BLOCK_BIG:
		_objectName = "bigBlock";
		break;
	case CHECKPOINT01:
		_objectName = "checkpoint01";
		break;
	case CHECKPOINT02:
		_objectName = "checkpoint02";
		break;
	case CHEST:
		_objectName = "chest";
		break;
	case AWL:
		_objectName = "awl";
		break;
	case PILE01:
		_objectName = "pile01";
		break;
	case PILE02:
		_objectName = "pile02";
		break;
	case MOVINGTILE01:
		_objectName = "movingTile01";
		break;
	case MOVINGTILE02:
		_objectName = "movingTile02";
		break;
	case PLATTER:
		_objectName = "platter";
		break;
	}
}

void maptool::selectEnemyName(ENEMYTYPE type)
{
	switch (type)
	{
	case BEETO:
		_enemyName = "beeto";
		break;
	case BLORB:
		_enemyName = "blorb";
		break;
	case SKELETON:
		_enemyName = "skeleton";
		break;
	case BUBBLEDRAGON:
		_enemyName = "bubbleDragon";
		break;
	case NONE_ENEMY01:
		break;
	case NONE_ENEMY02:
		break;
	case NONE_ENEMY03:
		break;
	case NONE_ENEMY04:
		break;
	case NONE_ENEMY05:
		break;
	case NONE_ENEMY06:
		break;
	case NONE_ENEMY07:
		break;
	case NONE_ENEMY08:
		break;
	}
}

void maptool::pushObject(tagObject & obj)
{
	obj.rc = RectMake(obj.x, obj.y, obj.objImage->getWidth(), obj.objImage->getHeight());

	_lObjList[obj.type].push_back(obj);

	//int tileX = obj.x / TILESIZE;
	//int tileY = obj.y / TILESIZE;
	//int width = obj.objImage->getWidth() / TILESIZE;
	//int height = obj.objImage->getHeight() / TILESIZE;

	////오브젝트가 있는 타일의 상태를 변경
	//for (int i = tileX; i < tileX + width; ++i)
	//{
	//	for (int j = tileY; j > tileY + height; ++j)
	//	{
	//		_tile[j][i].type = ONOBJECT;
	//	}
	//}
}

void maptool::pushEnemy(tagEnemy & enemy)
{
	enemy.rc = RectMake(enemy.x, enemy.y, enemy.enemyImage->getFrameWidth(), enemy.enemyImage->getFrameHeight());

	_lEnemy[enemy.type].push_back(enemy);

	//int tileX = enemy.x / TILESIZE;
	//int tileY = enemy.y / TILESIZE;
	//int width = enemy.enemyImage->getFrameWidth() / TILESIZE;
	//int height = enemy.enemyImage->getFrameHeight() / TILESIZE;

	////에너미가 있는 타일의 상태를 변경
	//for (int i = tileX; i < tileX + width; ++i)
	//{
	//	for (int j = tileY; j > tileY + height; ++j)
	//	{
	//		_tile[j][i].type = ONENEMY;
	//	}
	//}
}
