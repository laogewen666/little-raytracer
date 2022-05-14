#pragma once
#include"ray.h"
#include"hitobject.h"
#include"rtweekend.h"
#include"texture.h"
class material
{
public:
	virtual bool scatter(const ray& ray_in,const hit_record& rec,vec3& attenuation,ray& scattered)const = 0;
};

class lambertian :public material
{
public:
	lambertian(shared_ptr<texture> a):albedo(a){}

	virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered)const
	{
		attenuation = albedo->value(rec.u,rec.v,rec.p);
		vec3 scattered_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scattered_direction, ray_in.time());
		return true;
	}
public:
	shared_ptr<texture> albedo;
};


class metal :public material
{
public:
	metal(const vec3& a,double f):albedo(a),fuzz(f<1?f:1) {}

	virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered)const
	{
		attenuation = albedo;
		vec3 reflect = reflected(ray_in.direction(),rec.normal );
		scattered = ray(rec.p, reflect + fuzz * random_in_unit_sphere(), ray_in.time());
		return (dot(scattered.direction(), rec.normal) > 0);
	}
public:
	vec3 albedo;
	double fuzz;
};

class dielectric :public material
{
public:
	dielectric(double i):ref_idx(i){}
	virtual bool scatter(const ray& ray_in, const hit_record& rec, vec3& attenuation, ray& scattered)const
	{
		attenuation = vec3(1.0, 1.0, 1.0);
		double etai_over_etat = (rec.front_face) ? (1 / ref_idx) : (ref_idx);
		vec3 unit_direction = unit_vector(ray_in.direction());
		
		auto cos_theta = ffmin(dot(-unit_direction, rec.normal),1.0);
		auto sin_theta = sqrt(1 - cos_theta * cos_theta);

		if (etai_over_etat * sin_theta > 1.0)
		{
			auto reflect = reflected(unit_direction, rec.normal);
			scattered = ray(rec.p, reflect, ray_in.time());
			return true;
		}

		auto reflect_prob = schlick(cos_theta, etai_over_etat);
		if (random_double() < reflect_prob)
		{
			auto reflect = reflected(unit_direction, rec.normal);
			scattered = ray(rec.p, reflect, ray_in.time());
			return true;
		}

		auto refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered = ray(rec.p, refracted, ray_in.time());
		return true;
	}

public:
	double ref_idx;
};