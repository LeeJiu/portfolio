#pragma once
#include "gameNode.h"

class maptoolUI : public gameNode
{
private:
	image* _sampleImg;
	image* _objectImg;
	image* _enemyImg;

	RECT _selectStage;
	RECT _selectUI;
	RECT _selectTile;
	RECT _selectType;
	RECT _selectNum;
	RECT _selectObj;
	RECT _selectEnemy;

	char * _objectName;
	char * _enemyName;
	char _strStage[128];
	int _nStage;
	int _imageNumX, _imageNumY;
	int _objWidth, _objHeight;
	bool _onSelTile, _onSelObj, _onSelChar, _onUI, _viewColl;
	TILETYPE _tileType;
	OBJECTTYPE _objectType;
	ENEMYTYPE _enemyType;

public:
	maptoolUI();
	~maptoolUI();

	HRESULT init();
	void release();
	void update();
	void render();

	void selectTile();
	void selectObject();
	void selectCharacter();
	void selectStageState();

	void changeObjectImg();
	void changeEnemyImg();

	const char* getObjName() { return _objectName; }
	const char* getEnemyName() { return _enemyName; }
	int getStageNum() { return _nStage; }
	int getImageNumX() { return _imageNumX; }
	int getImageNumY() { return _imageNumY; }
	bool getOnUI() { return _onUI; }
	bool getOnSelTile() { return _onSelTile; }
	bool getOnSelObj() { return _onSelObj; }
	bool getOnSelChar() { return _onSelChar; }
	TILETYPE getTileType() { return _tileType; }
	OBJECTTYPE getObjectType() { return _objectType; }
	ENEMYTYPE getEnemyType() { return _enemyType; }
	image* getTileImage() { return _sampleImg; }
	//image* getObjectImage() { return _objectImg; }
};

