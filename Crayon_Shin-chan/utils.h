#pragma once


#include <cmath>

#define PI 3.141592654f
#define ONE_RAD (PI / 180)
#define PI2 (PI * 2)

//실수 대소비교하기 위한 변수 (가장 작은 단위라고 보자)
#define FLOAT_EPSILON 0.001f 

namespace MY_UTIL
{
	//거리 구하는 함수
	float getDistance(float startX, float startY, float endX, float endY);

	//각도 구하는 함수
	float getAngle(float startX, float startY, float endX, float endY);
}
