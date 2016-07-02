#pragma once


#include <cmath>

#define PI		3.141592654f
#define PI2		(PI * 2)
#define PI8		float(PI / 8.0f)
#define PI16	float(PI / 16.f)
#define PI32	float(PI / 32.f)
#define PI64	float(PI / 64.f)
#define PI128	float (PI / 128.f)

#define ONE_RAD (PI / 180)


//실수 대소비교하기 위한 변수 (가장 작은 단위라고 보자)
#define FLOAT_EPSILON 0.001f 
#define FLOAT_TO_INT(f1)		static_cast<int>(f1 * FLOAT_EPSILON)
#define FLOAT_EQUAL(f1, f2)		(fabs(f1 - f2) <= FLOAT_EPSILON) //두 실수가 같은지 확인

#define OBJNUM 12
#define ENEMYNUM 4

#define PAGENUMX 5
#define PAGENUMY 4

//타일 변수
#define TILESIZE 32
#define TILENUMX (WINSIZEX * PAGENUMX / TILESIZE)
#define TILENUMY (WINSIZEY * PAGENUMY / TILESIZE)


//타일 타입
enum TILETYPE
{
	NONE, CRASHABLE, BREAKABLE, BACKGROUND, LADDER, ONOBJECT, ONENEMY
};

//타일 구조체
struct tagTile
{
	image* tileImage;
	RECT rc;
	POINT imagePos;	//이미지 좌표
	int index;
	int x, y;		//화면 절대 좌표
	TILETYPE type;
};

//오브젝트 리스트
enum OBJECTTYPE
{
	BLOCK_SMALL, BLOCK_BIG, CHECKPOINT01, CHECKPOINT02, 
	CHEST, AWL, PILE01, PILE02,
	MOVINGTILE01, MOVINGTILE02, PLATTER, NONE_OBJ
};

//오브젝트 구조체
struct tagObject
{
	image* objImage;
	RECT rc;
	int x, y;
	OBJECTTYPE type;
};

//에너미 리스트
enum ENEMYTYPE
{
	BEETO, BLORB, BUBBLEDRAGON, SKELETON,
	NONE_ENEMY01, NONE_ENEMY02, NONE_ENEMY03, NONE_ENEMY04,
	NONE_ENEMY05, NONE_ENEMY06, NONE_ENEMY07, NONE_ENEMY08,
};

//오브젝트 구조체
struct tagEnemy
{
	image* enemyImage;
	RECT rc;
	int x, y;
	ENEMYTYPE type;
};

namespace MY_UTIL
{
	//거리 구하는 함수
	float getDistance(float startX, float startY, float endX, float endY);

	//각도 구하는 함수
	float getAngle(float startX, float startY, float endX, float endY);
}
