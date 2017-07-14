#define _USE_MATH_DEFINES

#include "Common.h"

#include <fstream>
#include <cmath>

bool IsFileExist(const char *filePath)
{
	std::ifstream f(filePath);
	return f.good();
}

float ToRadian(float deg)
{
	return static_cast<float>(deg * M_PI / 180);
}

sf::Vector2f LenDir(float len, float dir)
{
	float rad = ToRadian(dir);

	sf::Vector2f vec;
	vec.x = static_cast<float>(sin(rad)) * len;
	vec.y = -static_cast<float>(cos(rad)) * len;
	return vec;
}
