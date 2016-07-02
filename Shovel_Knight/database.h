#pragma once

#include "singletonBase.h"

#include <vector>
#include <map>

class tileData
{
public:
	int index;
	int x, y;
	int state;
	int imageNum;

	tileData() {};
	~tileData() {};
};

class objData
{
public:
	const char* imageName;
	int index;
	int x, y;
	int state;

	objData() {};
	~objData() {};
};

class database : public singletonBase<database>
{
private:
	typedef vector<string> arrElements;
	typedef vector<string>::iterator iterElements;
	typedef map<string, tileData*> arrElement;
	typedef map<string, tileData*>::iterator iterElement;

	typedef map<string, objData*> arrElement2;
	typedef map<string, objData*>::iterator iterElement2;

private:
	arrElement _mTotalElement;
	arrElement2 _mTotalElement2;

public:
	database();
	~database();

	HRESULT init();
	void release();

	void loadDatabase(string name);
	void loadDatabase2(string name);

	tileData* getTileDataElement(string str) { return _mTotalElement.find(str)->second; }
	objData* getObjDataElement(string str) { return _mTotalElement2.find(str)->second; }

	int getNumberOfObjData() { return _mTotalElement2.size() / 4; }

	void setTileDataIndex(string str, int index);
	void setTileDataX(string str, int x);
	void setTileDataY(string str, int y);
	void setTileDataState(string str, int state);
	void setTileDataImageNum(string str, int imageNum);
};

