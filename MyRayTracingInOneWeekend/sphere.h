#pragma once

#include"hitobject.h"
#include"vec3.h"

class sphere :public hitobject
{
public:
	sphere(){}
	sphere(vec3 cen,double r,shared_ptr<material> m):center(cen),radius(r),mat_ptr(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool boudingbox(double t0, double t1, aabb& output_box)const;

public:
	vec3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	vec3 ac = r.oringin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(r.direction(), ac);
	auto c = dot(ac, ac) - radius * radius;
	auto root = b * b - 4 * a * c;
	if (root > 0)
	{
		auto t = (-b - sqrt(root)) / (2 * a);
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - center)/radius;
			rec.set_front_face(r,rec.normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		 t = (-b + sqrt(root)) / (2 * a);
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center) / radius;
			rec.set_front_face(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool sphere::boudingbox(double t0, double t1, aabb& output_box)const
{
	output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}


class movingsphere:public hitobject
{
public:
	movingsphere(){}
	movingsphere(vec3 cen0,vec3 cen1,double r,double t0,double t1,shared_ptr<material> m):center0(cen0),center1(cen1), radius(r), time0(t0), time1(t1), mat_ptr(m) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool boudingbox(double t0, double t1, aabb& output_box)const;

	vec3 center(double time)const;
public:
	vec3 center0,center1;
	double radius;
	shared_ptr<material> mat_ptr;
	double time0, time1;
};

vec3 movingsphere::center(double time)const
{
	return center0 + ((time - time0)/(time1 - time0))*(center1 - center0);
}

bool movingsphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	vec3 ac = r.oringin() - center(r.time());
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(r.direction(), ac);
	auto c = dot(ac, ac) - radius * radius;
	auto root = b * b - 4 * a * c;
	if (root > 0)
	{
		auto t = (-b - sqrt(root)) / (2 * a);
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.p = r.at(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.set_front_face(r, rec.normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		t = (-b + sqrt(root)) / (2 * a);
		if (t > t_min && t < t_max)
		{
			rec.t = t;
			rec.p = r.at(rec.t);
			vec3 outward_normal = (rec.p - center(r.time())) / radius;
			rec.set_front_face(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	else
	{
		return false;
	}
	           
}

bool movingsphere::boudingbox(double t0, double t1, aabb& output_box)const
{
	auto sphere1 = aabb(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	auto sphere2 = aabb(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	output_box = surrounding_box(sphere1, sphere2);
	return true;
}


