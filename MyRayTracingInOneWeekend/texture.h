#pragma once
#include"rtweekend.h"

class texture
{
public:
	virtual vec3 value(double u, double v,const vec3& p)const = 0;
};

class constant_texture :public texture
{
public:
	constant_texture(){}
	constant_texture(vec3 c):color(c){}

	vec3 value(double u, double v,const vec3& p)const
	{
		return color;
	}
public:
	vec3 color;
};

class checker_texture :public texture
{
public:
	checker_texture(){}
	checker_texture(shared_ptr<texture> a,shared_ptr<texture> b):odd(a),even(b) {}

	vec3 value(double u, double v, const vec3& p)const
	{
		auto sine = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
		if (sine < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
	
public:
	shared_ptr<texture> odd;
	shared_ptr<texture> even;
};
