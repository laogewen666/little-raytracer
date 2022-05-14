#pragma once
#include"ray.h"
class material;

struct hit_record
{
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;

	double t;
	double u;
	double v;
	bool front_face;

	inline void set_front_face(const ray& r,const vec3& outward_normal )
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class aabb 
{
public:
	aabb(){}
	aabb(const vec3& a,const vec3& b):_min(a),_max(b){}

	vec3 min()const { return _min; }
	vec3 max()const { return _max; }

	bool hit(const ray& r, double tmin, double tmax)const;
public:
	vec3 _min, _max;
};

inline bool aabb::hit(const ray& r, double tmin, double tmax)const
{
	for (int a = 0; a < 3; ++a)
	{
		auto div = 1.0f / r.direction()[a];
		auto t0 = (min()[a] - r.oringin()[a]) * div;
		auto t1 = (max()[a] - r.oringin()[a]) * div;
		if (div<0.0f)
			std::swap(t0, t1);
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmin >= tmax)
			return false;
		
	}
	return true;
}

aabb surrounding_box(aabb box0, aabb box1)
{
	vec3 small( (ffmin(box0.min().x(), box1.min().x()) ), (ffmin(box0.min().y(), box1.min().y()) ),((ffmin(box0.min().z(), box1.min().z()) )));
	vec3 big((ffmax(box0.max().x(), box1.max().x())), (ffmax(box0.max().y(), box1.max().y())), (ffmax(box0.max().z(), box1.max().z())));
	return aabb(small, big);
}

class hitobject
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const = 0;
	virtual bool boudingbox(double t0,double t1,aabb& output_box)const = 0;
};

