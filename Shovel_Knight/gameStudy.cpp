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
	IMAGEMANAGER->addImage("Stage0", "image/stage0.bmp", 96, 256, true, 0xff00ff);
	IMAGEMANAGER->addImage("Stage0_pixel", "image/stage0_pixel.bmp", 96, 256, true, 0x000000);
	IMAGEMANAGER->addImage("Stage0_bg", "image/stage0_bg.bmp", 96, 256, true, 0xff00ff);
	IMAGEMANAGER->addImage("Stage0_bgpixel", "image/stage0_bgpixel.bmp", 96, 256, true, 0x000000);

	//오브젝트
	//맵툴에서 사용하는 단일 이미지
	IMAGEMANAGER->addImage("objects", "image/objects.bmp", 128, 384, false, false);
	IMAGEMANAGER->addImage("smallBlock", "image/object/maptool/smallBlock.bmp", 32, 32, false, false);
	IMAGEMANAGER->addImage("bigBlock", "image/object/maptool/bigBlock.bmp", 64, 64, false, false);
	IMAGEMANAGER->addImage("checkpoint01", "image/object/maptool/checkpoint01.bmp", 70, 112, true, 0xff00ff);
	IMAGEMANAGER->addImage("checkpoint02", "image/object/maptool/checkpoint02.bmp", 42, 96, true, 0xff00ff);
	IMAGEMANAGER->addImage("chest_map", "image/object/maptool/chest.bmp", 54, 30, true, 0xff00ff);
	IMAGEMANAGER->addImage("awl", "image/object/maptool/awl.bmp", 32, 32, true, 0xff00ff);
	IMAGEMANAGER->addImage("movingTile01", "image/object/maptool/movingTile01.bmp", 96, 32, true, 0xff00ff);
	IMAGEMANAGER->addImage("movingTile02", "image/object/maptool/movingTile02.bmp", 96, 32, true, 0xff00ff);
	IMAGEMANAGER->addImage("pile01", "image/object/maptool/pile01.bmp", 66, 30, true, 0xff00ff);
	IMAGEMANAGER->addImage("pile02", "image/object/maptool/pile02.bmp", 56, 26, true, 0xff00ff);
	IMAGEMANAGER->addImage("platter", "image/object/maptool/platter.bmp", 58, 36, true, 0xff00ff);
	IMAGEMANAGER->addImage("ladder", "image/object/maptool/ladder.bmp", 32, 32, true, 0xff00ff);

	//프레임 이미지
	IMAGEMANAGER->addFrameImage("chest", "image/object/chest.bmp", 136, 46, 2, 1, true, 0xff00ff);
	
	//보석
	IMAGEMANAGER->addFrameImage("gems", "image/object/gems.bmp", 252, 32, 6, 1, true, 0xff00ff);
	//bubble
	IMAGEMANAGER->addFrameImage("bubble", "image/object/bubble.bmp", 256, 64, 4, 1, true, 0xff00ff);

	//에너미
	//맵툴에서 사용하는 단일 이미지
	IMAGEMANAGER->addImage("enemies", "image/enemies.bmp", 128, 384, false, false);
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
	IMAGEMANAGER->addFrameImage("bubbleDragon_attack", "image/enemy/bubbleDragon_attack.bmp", 1086, 178, 3, 1, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("bubbleDragon_attack2", "image/enemy/bubbleDragon_attack2.bmp", 2172, 178, 6, 1, true, 0xff00ff);
	//skeleton
	IMAGEMANAGER->addFrameImage("skeleton_idle", "image/enemy/skeleton_idle.bmp", 78, 116, 1, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("skeleton_walk", "image/enemy/skeleton_walk.bmp", 312, 124, 4, 2, true, 0xff00ff);
	IMAGEMANAGER->addFrameImage("skeleton_attack", "image/enemy/skeleton_attack.bmp", 272, 144, 2, 2, true, 0xff00ff);

	//effect
	EFFECTMANAGER->addEffect("explosion", "image/fx/explosion.bmp", 320, 64, 64, 64, 1, 0.2, 10);
	EFFECTMANAGER->addEffect("bubble_pong", "image/fx/bubble_pong.bmp", 192, 96, 96, 96, 1, 0.2, 10);
	EFFECTMANAGER->addEffect("sparkle1", "image/fx/sparkle1.bmp", 42, 14, 14, 14, 1, 0.2, 10);
	EFFECTMANAGER->addEffect("sparkle2", "image/fx/sparkle2.bmp", 70, 14, 14, 14, 1, 0.2, 10);
	EFFECTMANAGER->addEffect("sparkle3", "image/fx/sparkle3.bmp", 42, 14, 14, 14, 1, 0.2, 10);
	EFFECTMANAGER->addEffect("wind", "image/fx/wind.bmp", 1435, 87, 205, 87, 1, 0.2, 10);

	//sound
	SOUNDMANAGER->addSound("bgm", "sound/StrikeTheEarth.mp3", true, true);
	SOUNDMANAGER->addSound("campfire", "sound/TheStarlitWilds.mp3", true, true);
	SOUNDMANAGER->addSound("clear", "sound/NoWeaponsHere.mp3", true, true);
	SOUNDMANAGER->addSound("main", "sound/MainTheme.mp3", true, true);
	SOUNDMANAGER->addSound("appear", "sound/sfx_appear.mp3", false, false);
	SOUNDMANAGER->addSound("dig", "sound/sfx_dig.mp3", false, false);
	SOUNDMANAGER->addSound("jump", "sound/sfx_jump.mp3", false, false);
	SOUNDMANAGER->addSound("get", "sound/sfx_get.mp3", false, false);
	SOUNDMANAGER->addSound("dead", "sound/sfx_dead.mp3", false, false);
	SOUNDMANAGER->addSound("select", "sound/sfx_select.mp3", false, false);
	SOUNDMANAGER->addSound("ok", "sound/sfx_ok.mp3", false, false);


	SCENEMANAGER->addScene("title", new title);
	SCENEMANAGER->addScene("maptool", new maptool);
	SCENEMANAGER->addScene("stage", new stage);
	SCENEMANAGER->addScene("gameover", new gameOver);
	SCENEMANAGER->addScene("clear", new clear);

	SCENEMANAGER->changeScene("title");

	return S_OK;
}

void gameStudy::release()
{
	SCENEMANAGER->release();

	gameNode::release();
}

void gameStudy::update()
{
	if (KEYMANAGER->isOnceKeyDown('P'))
		_isPixel = !_isPixel;

	SOUNDMANAGER->update();
	SCENEMANAGER->update();

	gameNode::update();
}

void gameStudy::render()
{
	//백버퍼에서 뿌려준다
	
	PatBlt(getMemDC(), _sourX, _sourY, WINSIZEX, WINSIZEY, BLACKNESS);
	PatBlt(getPixelDC(), _sourX, _sourY, WINSIZEX, WINSIZEY, BLACKNESS);
	//IMAGEMANAGER->findImage("background")->render(getMemDC());
	//======================
	//이 사이에서 그려주면 됨.

	SCENEMANAGER->render();

	//======================
	//TIMEMANAGER->render(getMemDC());
	if(!_isPixel)
		this->getBackBuffer()->render(getHDC(), 0, 0, _sourX, _sourY, WINSIZEX, WINSIZEY);
	else	
		this->getPixelBuffer()->render(getHDC(), 0, 0, _sourX, _sourY, WINSIZEX, WINSIZEY);
}


