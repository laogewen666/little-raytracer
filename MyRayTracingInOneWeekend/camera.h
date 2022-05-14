#pragma once
#include"rtweekend.h"

class camera
{
public:
	camera(vec3 lookfrom,vec3 lookat,vec3 vup,double fov,double aspect,double aperture,double focus_dist,double t0 = 0,double t1 = 0)
	{
		oringin = lookfrom;
		len_radius = aperture / 2;
		auto theta = degree_to_radians(fov);
		auto half_height = tan(theta / 2);
		auto half_width = half_height * aspect;
		time0 = t0;
		time1 = t1;
		
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(crossdot(vup,w));
		v = unit_vector(crossdot(w,u));

		left_lower_corner = oringin - half_width * focus_dist * u - focus_dist * half_height * v - focus_dist * w;
		horizental = focus_dist * 2 * u * half_width;;
		vertical = focus_dist * 2 * v * half_height;
		
	}

	ray get_ray(double s, double t)
	{
		vec3 rd = len_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(oringin + offset, left_lower_corner + s * horizental + t * vertical - oringin - offset, random_double(time0,time1));
	}


public:
	vec3 left_lower_corner;
	vec3 horizental;
	vec3 vertical;
	vec3 oringin;
	vec3 u, v, w;
	double len_radius;
	double time0, time1;
};