#pragma once
#include"vec3.h"

class ray
{
public:
	ray(){}
	ray(const vec3& oringnal, const vec3& direction, double t ) :orin(oringnal), dir(direction), tm(t) {}

	vec3 oringin() const { return orin; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }
	vec3 at(double t) const{ return orin + t * dir; }

public:
	vec3 orin;
	vec3 dir;
	double tm;

};