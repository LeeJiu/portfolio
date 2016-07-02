#pragma once
#pragma warning(disable : 4996)
#include "gameNode.h"
#include "maptoolUI.h"

class maptool : public gameNode
{
private:
	maptoolUI* _ui;
	
	vector<tagTile*> _vTile;
	vector<tagTile*>::iterator _viTile;

	tagTile _tile[TILENUMY][TILENUMX];

	list<tagObject> _lObjList[OBJNUM];
	list<tagObject>::iterator _liObjList;

	list<tagEnemy> _lEnemy[ENEMYNUM];
	list<tagEnemy>::iterator _liEnemy;

	char * _objectName;
	char * _enemyName;
	int _nStage;

	bool _onUI, _viewColl;
	TILETYPE _tileType;

public:
	maptool();
	~maptool();

	HRESULT init();
	void release();
	void update();
	void render();

	void keyControl();
	void moveWorkspace();
	void clickTile();
	void setTile();
	void setObject();
	void setCharacter();
	void deleteObject();
	void deleteEnemy();
	void reset();
	void save();
	void load();
	void selectObjectName(OBJECTTYPE type);
	void selectEnemyName(ENEMYTYPE type);
	void pushObject(tagObject& obj);
	void pushEnemy(tagEnemy& enemy);
};

