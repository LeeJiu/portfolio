#include "stdafx.h"
#include "database.h"


database::database()
{
}


database::~database()
{
}

HRESULT database::init()
{
	//loadDatabase("database.txt");

	return S_OK;
}

void database::release()
{
	iterElement mIter = _mTotalElement.begin();

	for (; mIter != _mTotalElement.end(); ++mIter)
	{
		SAFE_DELETE(mIter->second);
	}

	_mTotalElement.clear();
}

void database::loadDatabase(string name)
{
	//데이터 읽어온다
	arrElements vTemp;
	vTemp = TXTDATA->txtLoad(name.c_str());

	string str;
	int count = 0;

	//|,index,x,y,state,imageNum,
	for (unsigned int i = 0; i < vTemp.size(); i++)
	{
		if (vTemp[i] == "|")
		{
			tileData* em = new tileData;
			str = vTemp[i + 1];
			_mTotalElement.insert(pair<string, tileData*>(vTemp[i + 1], em));
			if (i != 0) count += 6;
			continue;
		}

		iterElement mIter = _mTotalElement.find(str);
		if (i == count + 1) mIter->second->index = atoi(vTemp[i].c_str());
		else if (i == count + 2) mIter->second->x = atoi(vTemp[i].c_str());
		else if (i == count + 3) mIter->second->y = atoi(vTemp[i].c_str());
		else if (i == count + 4) mIter->second->state = (TILETYPE)atoi(vTemp[i].c_str());
		else if (i == count + 5) mIter->second->imageNum = atoi(vTemp[i].c_str());
	}

	vTemp.clear();
}

void database::loadDatabase2(string name)
{
	//데이터 읽어온다
	arrElements vTemp;
	vTemp = TXTDATA->txtLoad(name.c_str());

	string str;
	int count = 0;

	//|,index,x,y,state,
	for (unsigned int i = 0; i < vTemp.size(); i++)
	{
		if (vTemp[i] == "|")
		{
			objData* em = new objData;
			str = vTemp[i + 1];
			_mTotalElement2.insert(pair<string, objData*>(vTemp[i + 1], em));
			if (i != 0) count += 6;
			continue;
		}

		iterElement2 mIter = _mTotalElement2.find(str);
		if (i == count + 1) mIter->second->index = atoi(vTemp[i].c_str());
		else if (i == count + 2) mIter->second->x = atoi(vTemp[i].c_str());
		else if (i == count + 3) mIter->second->y = atoi(vTemp[i].c_str());
		else if (i == count + 4) mIter->second->state = (TILETYPE)atoi(vTemp[i].c_str());
		else if (i == count + 5) mIter->second->imageName = vTemp[i].c_str();
	}

	vTemp.clear();
}

void database::setTileDataIndex(string str, int index)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->index = index;
}

void database::setTileDataX(string str, int x)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->x = x;
}

void database::setTileDataY(string str, int y)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->y = y;
}

void database::setTileDataState(string str, int state)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->state = state;
}

void database::setTileDataImageNum(string str, int imageNum)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->state = imageNum;
}
