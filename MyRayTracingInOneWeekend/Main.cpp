#include<iostream>
#include<fstream>
#include "rtweekend.h"
#include"hitobjectlist.h"
#include"sphere.h"
#include"camera.h"
#include"material.h"
#include"BVH.h"
using namespace std;

hitobjectlist two_sphere()
{
	hitobjectlist objects;
	auto checker = make_shared<checker_texture>(
		make_shared<constant_texture>(vec3(1.0, 0.0, 0.0)),
		make_shared<constant_texture>(vec3(0.9, 0.9, 0.9)));

	objects.add(make_shared<sphere>(vec3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	objects.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	return objects;
}
hitobjectlist random_scene() {
	hitobjectlist world;

	auto checker = make_shared<checker_texture>(
		make_shared<constant_texture>(vec3(1.0, 0.0, 0.0)),
		make_shared<constant_texture>(vec3(0.9, 0.9, 0.9)));

	world.add(make_shared<sphere>(
		vec3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = vec3::random() * vec3::random();
					world.add(
						make_shared<movingsphere>(center,center+vec3(0,random_double(0,0.5),0), 0.2, 0.0, 1.0, make_shared<lambertian>(make_shared<constant_texture>(albedo))));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = vec3::random(.5, 1);
					auto fuzz = random_double(0, .5);
					world.add(
						make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
				}
				else {
					// glass
					world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
				}
			}
		}
	}

	world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));

	world.add(
		make_shared<sphere>(vec3(-4, 1, 0), 1.0, make_shared<lambertian>(make_shared<constant_texture>(vec3(0.4, 0.2, 0.1)))));

	world.add(
		make_shared<sphere>(vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return static_cast<hitobjectlist>(make_shared<bvh_node>(world, 0, 1));
}

vec3 ray_color(const ray& r,const hitobject& world,int depth)
{
	hit_record rec;

	if (depth <= 0)
		return vec3(1, 0, 0);
	if (world.hit(r, 0.0001, infinity, rec))
	{
		vec3 attenuation;
		ray scattered;

		if(rec.mat_ptr->scatter(r,rec,attenuation,scattered))
			return attenuation*ray_color(scattered,world,depth - 1);

		return vec3(1, 0, 0);
	}

	vec3 unitdirection = unit_vector(r.direction());
	auto time = 0.5 * (unitdirection.y() + 1.0);
	return (1 - time) * vec3(1.0, 1.0, 1.0) + time * vec3(0.5, 0.7, 1.0);
}
int main()
{
	ofstream fout;
	fout.open("image.ppm");
	const int image_width = 200;
	const int image_height = 100;
	const int sameple_per_pixel = 100;
	const int maxdepth = 100;

	const auto aspect_ratio = double(image_width) / image_height;

	

	fout << "P3\n" << image_width << ' ' << image_height << "\n255\n";	

	auto world = two_sphere();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.0;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	for (int j = image_height - 1; j >= 0; j--)
	{
		cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			vec3 color(0,0,0);
				for (int s = 0; s < sameple_per_pixel; ++s)
				{
					auto u = double(i+random_double()) / image_width;
					auto v = double(j+random_double()) / image_height;
					ray r = cam.get_ray(u, v);
					color += ray_color(r, world,maxdepth);
				}
			
			color.write_color(fout,sameple_per_pixel);
		}
	}
	cerr << "\nDone\n";
	fout.close();
}