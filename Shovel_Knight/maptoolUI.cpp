#include "stdafx.h"
#include "maptoolUI.h"


maptoolUI::maptoolUI()
{
}


maptoolUI::~maptoolUI()
{
}

HRESULT maptoolUI::init()
{
	//UI
	_selectStage = RectMake(_sourX, _sourY, WINSIZEX - 128, 128);
	_selectUI = RectMake(_sourX + 768, _sourY, 192, WINSIZEY);
	_selectType = RectMake(_sourX + 816, _sourY + 32, 96, 64);
	_selectTile = RectMake(_sourX + 816, _sourY + 128, 96, 224);
	_selectNum = RectMake(_sourX + 96, _sourY + 32, 32, 64);
	_selectObj = RectMake(_sourX + 784, _sourY + 64, 160, 481);
	_selectEnemy= RectMake(_sourX + 784, _sourY + 64, 160, 481);

	sprintf_s(_strStage, "Stage%d", _nStage);

	IMAGEMANAGER->addImage("ui_dlg_big", "image/ui/ui_dlg_big.bmp", 192, 640, false, false);
	IMAGEMANAGER->addImage("ui_dlg_big2", "image/ui/ui_dlg_big2.bmp", 960, 128, false, false);
	IMAGEMANAGER->addImage("ui_selectPos", "image/ui/ui_selectPos.bmp", 96, 96, false, false);
	IMAGEMANAGER->addImage("ui_selectType", "image/ui/ui_selectType.bmp", 96, 64, false, false);
	IMAGEMANAGER->addImage("ui_selectNum", "image/ui/ui_selectNum.bmp", 32, 64, false, false);

	_sampleImg = IMAGEMANAGER->findImage(_strStage);
	_objectImg = NULL;
	_enemyImg = IMAGEMANAGER->findImage("enemies");;
	_tileType = BACKGROUND;
	_enemyType = BEETO;
	_objectName = "smallBlock";	//디폴트 오브젝트
	_enemyName = "beeto";		//디폴트 에너미

	return S_OK;
}

void maptoolUI::release()
{
}

void maptoolUI::update()
{
	//렉트 갱신
	_selectStage = RectMake(_sourX, _sourY, WINSIZEX - 128, 128);
	_selectUI = RectMake(_sourX + 768, _sourY, 192, WINSIZEY);
	_selectType = RectMake(_sourX + 816, _sourY + 32, 96, 64);
	_selectTile = RectMake(_sourX + 816, _sourY + 128, 96, 224);
	_selectNum = RectMake(_sourX + 96, _sourY + 32, 32, 64);
	_selectObj = RectMake(_sourX + 784, _sourY + 64, 160, 481);
	_selectEnemy = RectMake(_sourX + 784, _sourY + 64, 160, 481);

	sprintf_s(_strStage, "Stage%d", _nStage);

	selectStageState();
	selectTile();
	selectObject();
	selectCharacter();

	//샘플이미지 변경
	_sampleImg = IMAGEMANAGER->findImage(_strStage);
}

void maptoolUI::render()
{
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SetBkMode(getMemDC(), TRANSPARENT);
	
	//스테이지와 페이즈 넘버 세팅 / 상단 ui
	IMAGEMANAGER->findImage("ui_dlg_big2")->render(getMemDC(), _selectStage.left, _selectStage.top);
	IMAGEMANAGER->findImage("ui_selectNum")->render(getMemDC(), _selectNum.left, _selectNum.top);
	TextOut(getMemDC(), _selectStage.left + 32, _selectStage.top + 54, _strStage, strlen(_strStage));

	//우측 팝업 ui
	if (_onUI)
	{
		IMAGEMANAGER->findImage("ui_dlg_big")->render(getMemDC(), _selectUI.left, _selectUI.top);
		
		if (_onSelTile)
		{
			IMAGEMANAGER->findImage("ui_selectType")->render(getMemDC(), _selectType.left, _selectType.top);
			_sampleImg->render(getMemDC(), _selectTile.left, _selectTile.top);
		}
		else if (_onSelObj)
		{
			IMAGEMANAGER->findImage("objects")->render(getMemDC(), _selectObj.left, _selectObj.top);
		}
		else if (_onSelChar)
		{
			IMAGEMANAGER->findImage("enemies")->render(getMemDC(), _selectEnemy.left, _selectEnemy.top);
		}
	}

	char str[128];
	sprintf_s(str, "%d", (int)_enemyType);
	TextOut(getMemDC(), _sourX + 10, _sourY + 320, str, strlen(str));
}

void maptoolUI::selectStageState()
{
	//스테이지 넘버
	if (PtInRect(&_selectNum, _click))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (_click.y < _selectNum.top + 32)
				_nStage++;
			else
			{
				_nStage--;
				if (_nStage < 0)
					_nStage = 0;
			}
		}
	}
}

void maptoolUI::changeObjectImg()
{
	switch (_objectType)
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
	default:
		break;
	}
}

void maptoolUI::changeEnemyImg()
{
	switch (_enemyType)
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
	default:
		break;
	}
}

void maptoolUI::selectTile()
{
	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		if (_onUI && _onSelTile)
		{
			_onUI = false;
			_onSelTile = true;
			_onSelObj = false;
			_onSelChar = false;
		}
		else
		{
			_onUI = true;
			_onSelTile = true;
			_onSelObj = false;
			_onSelChar = false;
		}
	}

	if (_onSelTile && _onUI)
	{
		if (PtInRect(&_selectTile, _click))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				int width = _sampleImg->getWidth();
				int height = _sampleImg->getHeight();
				int frameX = width / TILESIZE;
				int frameY = height / TILESIZE;

				for (int i = 0; i < frameX; ++i)
				{
					if (_click.x > _selectTile.left + (width / frameX * i)
						&& _click.x < _selectTile.left + (width / frameX * (i + 1)))
					{
						_imageNumX = i;
						break;
					}
				}

				for (int i = 0; i < frameY; ++i)
				{
					if (_click.y > _selectTile.top + (height / frameY * i)
						&& _click.y < _selectTile.top + (height / frameY * (i + 1)))
					{
						_imageNumY = i;
						break;
					}
				}

				if (_imageNumX == frameX - 1 && _imageNumY == frameY - 1)
					_tileType = LADDER;
			}
		}

		//타일의 상태 설정 (충돌 여부)
		if (PtInRect(&_selectType, _click))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
			{
				int width = IMAGEMANAGER->findImage("ui_selectType")->getWidth();
				for (int i = 0; i < 3; ++i)
				{
					if (_click.x > _selectType.left + (width / 3 * i)
						&& _click.x < _selectType.left + (width / 3 * (i + 1)))
					{
						_tileType = (TILETYPE)(i + 1);
						break;
					}
				}
			}
		}
	}
}

void maptoolUI::selectObject()
{
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		if (_onUI && _onSelObj)
		{
			_onUI = false;
			_onSelTile = false;
			_onSelObj = true;
			_onSelChar = false;
		}
		else
		{
			_onUI = true;
			_onSelTile = false;
			_onSelObj = true;
			_onSelChar = false;
		}
	}

	if (_onSelObj && _onUI)
	{
		if (PtInRect(&_selectObj, _click))
		{
			if(_leftButtonDown)
			{
				_leftButtonDown = false;
				int width = IMAGEMANAGER->findImage("objects")->getWidth();
				int height = IMAGEMANAGER->findImage("objects")->getHeight();
				int x = 0, y = 0;

				for (int i = 0; i < 2; ++i)
				{
					if (_click.x > _selectObj.left + (width / 2 * i)
						&& _click.x < _selectObj.left + (width / 2 * (i + 1)))
					{
						x = i;
						break;
					}
				}

				for (int i = 0; i < 6; ++i)
				{
					if (_click.y > _selectObj.top + (height / 6 * i)
						&& _click.y < _selectObj.top + (height / 6 * (i + 1)))
					{
						y = i;
						break;
					}
				}
				_objectType = (OBJECTTYPE)(y * 2 + x);
				changeObjectImg();
			}
		}
	}
}

void maptoolUI::selectCharacter()
{
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		if (_onUI && _onSelChar)
		{
			_onUI = false;
			_onSelTile = false;
			_onSelObj = false;
			_onSelChar = true;
		}
		else
		{
			_onUI = true;
			_onSelTile = false;
			_onSelObj = false;
			_onSelChar = true;
		}
	}

	if (_onSelChar && _onUI)
	{
		if (PtInRect(&_selectEnemy, _click))
		{
			if (_leftButtonDown)
			{
				_leftButtonDown = false;
				int width = IMAGEMANAGER->findImage("enemies")->getWidth();
				int height = IMAGEMANAGER->findImage("enemies")->getHeight();
				int x = 0, y = 0;

				for (int i = 0; i < 2; ++i)
				{
					if (_click.x > _selectEnemy.left + (width / 2 * i)
						&& _click.x < _selectEnemy.left + (width / 2 * (i + 1)))
					{
						x = i;
						break;
					}
				}

				for (int i = 0; i < 6; ++i)
				{
					if (_click.y > _selectEnemy.top + (height / 6 * i)
						&& _click.y < _selectEnemy.top + (height / 6 * (i + 1)))
					{
						y = i;
						break;
					}
				}

				if(y < 2)
					_enemyType = (ENEMYTYPE)(y * 2 + x);
				changeEnemyImg();
			}
		}
	}
}
