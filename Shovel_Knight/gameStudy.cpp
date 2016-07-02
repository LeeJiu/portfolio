#include "stdafx.h"
#include "gameStudy.h"


gameStudy::gameStudy()
{
}


gameStudy::~gameStudy()
{
}

HRESULT gameStudy::init()
{
	gameNode::init(true);

	//이미지 추가
	//스테이지별 타일
	IMAGEMANAGER->addImage("Stage0", "image/testTile.bmp", 96, 224, true, 0xff00ff);

	//오브젝트
	//맵툴에서 사용하는 단일 이미지
	IMAGEMANAGER->addImage("objects", "image/objects.bmp", 160, 481, false, false);
	IMAGEMANAGER->addImage("smallBlock", "image/object/maptool/smallBlock.bmp", 32, 32, false, false);
	IMAGEMANAGER->addImage("bigBlock", "image/object/maptool/bigBlock.bmp", 64, 64, false, false);
	IMAGEMANAGER->addImage("checkpoint01", "image/object/maptool/checkpoint01.bmp", 70, 112, true, 0xff00ff);
	IMAGEMANAGER->addImage("checkpoint02", "image/object/maptool/checkpoint02.bmp", 42, 96, true, 0xff00ff);
	IMAGEMANAGER->addImage("chest", "image/object/maptool/chest.bmp", 54, 30, true, 0xff00ff);
	IMAGEMANAGER->addImage("awl", "image/object/maptool/awl.bmp", 32, 32, true, 0xff00ff);
	IMAGEMANAGER->addImage("movingTile01", "image/object/maptool/movingTile01.bmp", 96, 32, true, 0xff00ff);
	IMAGEMANAGER->addImage("movingTile02", "image/object/maptool/movingTile02.bmp", 96, 32, true, 0xff00ff);
	IMAGEMANAGER->addImage("pile01", "image/object/maptool/pile01.bmp", 66, 30, true, 0xff00ff);
	IMAGEMANAGER->addImage("pile02", "image/object/maptool/pile02.bmp", 56, 26, true, 0xff00ff);
	IMAGEMANAGER->addImage("platter", "image/object/maptool/platter.bmp", 58, 36, true, 0xff00ff);
	IMAGEMANAGER->addImage("ladder", "image/object/maptool/ladder.bmp", 32, 32, true, 0xff00ff);

	//에너미
	//맵툴에서 사용하는 단일 이미지
	IMAGEMANAGER->addImage("enemies", "image/enemies.bmp", 160, 481, false, false);
	IMAGEMANAGER->addImage("beeto", "image/enemy/beeto.bmp", 52, 32, true, 0xff00ff);
	IMAGEMANAGER->addImage("blorb", "image/enemy/blorb.bmp", 40, 22, true, 0xff00ff);
	IMAGEMANAGER->addImage("skeleton", "image/enemy/skeleton.bmp", 64, 58, true, 0xff00ff);
	IMAGEMANAGER->addImage("bubbleDragon", "image/enemy/bubbleDragon.bmp", 294, 178, true, 0xff00ff);
	//프레임 이미지
	//beeto
	IMAGEMANAGER->addFrameImage("beeto_walk", "image/enemy/beeto_walk.bmp", 208, 64, 4, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("beeto_dead", "image/enemy/beeto_dead.bmp", 104, 64, 2, 2, true, 0xff00ff);
	//blorb
	IMAGEMANAGER->addFrameImage("blorb_walk", "image/enemy/blorb_walk.bmp", 160, 22, 4, 1, true, 0xff00ff);
	//bubbleDragon
	IMAGEMANAGER->addFrameImage("bubbleDragon_idle", "image/enemy/bubbleDragon_idle.bmp", 4344, 178, 12, 1, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("bubbleDragon_walk", "image/enemy/bubbleDragon_walk.bmp", 2172, 178, 6, 1, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("bubbleDragon_attack", "image/enemy/bubbleDragon_attack.bmp", 1086, 178, 3, 1, true, 0xff00ff);
	//skeleton
	IMAGEMANAGER->addFrameImage("skeleton_idle", "image/enemy/skeleton_idle.bmp", 78, 116, 1, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("skeleton_walk", "image/enemy/skeleton_walk.bmp", 312, 124, 4, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("skeleton_attack", "image/enemy/skeleton_attack.bmp", 272, 144, 2, 2, true, 0xff00ff);
	

	SCENEMANAGER->addScene("maptool", new maptool);
	SCENEMANAGER->addScene("stage", new stage);

	SCENEMANAGER->changeScene("maptool");
	SCENEMANAGER->changeScene("stage", 0);

	return S_OK;
}

void gameStudy::release()
{
	SCENEMANAGER->release();

	gameNode::release();
}

void gameStudy::update()
{
	SCENEMANAGER->update();

	gameNode::update();
}

void gameStudy::render()
{
	//백버퍼에서 뿌려준다
	
	PatBlt(getMemDC(), _sourX, _sourY, WINSIZEX, WINSIZEY, BLACKNESS);
	//IMAGEMANAGER->findImage("background")->render(getMemDC());
	//======================
	//이 사이에서 그려주면 됨.

	SCENEMANAGER->render();

	//======================
	TIMEMANAGER->render(getMemDC());
	this->getBackBuffer()->render(getHDC(), 0, 0, _sourX, _sourY, WINSIZEX, WINSIZEY);
}


