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


//�Ǽ� ��Һ��ϱ� ���� ���� (���� ���� ������� ����)
#define FLOAT_EPSILON 0.001f 
#define FLOAT_TO_INT(f1)		static_cast<int>(f1 * FLOAT_EPSILON)
#define FLOAT_EQUAL(f1, f2)		(fabs(f1 - f2) <= FLOAT_EPSILON) //�� �Ǽ��� ������ Ȯ��

#define OBJNUM 12
#define ENEMYNUM 4

#define PAGENUMX 5
#define PAGENUMY 4

//Ÿ�� ����
#define TILESIZE 32
#define TILENUMX (WINSIZEX * PAGENUMX / TILESIZE)
#define TILENUMY (WINSIZEY * PAGENUMY / TILESIZE)


//Ÿ�� Ÿ��
enum TILETYPE
{
	NONE, CRASHABLE, BREAKABLE, BACKGROUND, LADDER, ONOBJECT, ONENEMY
};

//Ÿ�� ����ü
struct tagTile
{
	image* tileImage;
	RECT rc;
	POINT imagePos;	//�̹��� ��ǥ
	int index;
	int x, y;		//ȭ�� ���� ��ǥ
	TILETYPE type;
};

//������Ʈ ����Ʈ
enum OBJECTTYPE
{
	BLOCK_SMALL, BLOCK_BIG, CHECKPOINT01, CHECKPOINT02, 
	CHEST, AWL, PILE01, PILE02,
	MOVINGTILE01, MOVINGTILE02, PLATTER, NONE_OBJ
};

//������Ʈ ����ü
struct tagObject
{
	image* objImage;
	RECT rc;
	int x, y;
	OBJECTTYPE type;
};

//���ʹ� ����Ʈ
enum ENEMYTYPE
{
	BEETO, BLORB, BUBBLEDRAGON, SKELETON,
	NONE_ENEMY01, NONE_ENEMY02, NONE_ENEMY03, NONE_ENEMY04,
	NONE_ENEMY05, NONE_ENEMY06, NONE_ENEMY07, NONE_ENEMY08,
};

//������Ʈ ����ü
struct tagEnemy
{
	image* enemyImage;
	RECT rc;
	int x, y;
	ENEMYTYPE type;
};

namespace MY_UTIL
{
	//�Ÿ� ���ϴ� �Լ�
	float getDistance(float startX, float startY, float endX, float endY);

	//���� ���ϴ� �Լ�
	float getAngle(float startX, float startY, float endX, float endY);
}
