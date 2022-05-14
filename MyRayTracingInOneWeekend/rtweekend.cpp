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

inline double fmax(double a, double b)
{
	return a >= b ? a : b;
}

inline double fmin(double a, double b)
{
	return a <= b ? a : b;
}

#include"ray.h"
#include"vec3.h"