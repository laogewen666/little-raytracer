#pragma once
#include<cmath>
#include<cstdlib>
#include<memory>
#include<limits>

using std::shared_ptr;
using std::make_shared;

const double pi = 3.1415926535897932385;
const double infinity = std::numeric_limits<double>::infinity();

inline double degree_to_radians(double degree)
{
	return degree * pi / 180;
}

inline double ffmax(double a, double b)
{
	return a >= b ? a : b;
}

inline double ffmin(double a, double b)
{
	return a <= b ? a : b;
}

inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

inline double clamp(double x,double min,double max)
{
	if (x < min)return min;
	if (x > max)return max;
	return x;
}

inline double random_int(int min, int max)
{
	return rand() % (max - min + 1);
}


#include "ray.h"
#include "vec3.h"