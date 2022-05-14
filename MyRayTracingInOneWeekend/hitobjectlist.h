#pragma once
#include"hitobject.h"
#include<memory>
#include<vector>


using std::shared_ptr;
using std::make_shared;

class hitobjectlist : public hitobject
{
public:
	hitobjectlist() {}
	hitobjectlist(shared_ptr<hitobject> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hitobject> object) { objects.push_back(object); }
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool boudingbox(double t0, double t1, aabb& output_box)const;

public:
	std::vector<shared_ptr<hitobject>> objects;
};

bool hitobjectlist::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	hit_record temp_rec;
	bool hitanything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects)
		if (object->hit(r, t_min, closest_so_far, temp_rec))
		{
			closest_so_far = temp_rec.t;
			hitanything = true;
			rec = temp_rec;
		}
	return hitanything;
}

bool hitobjectlist::boudingbox(double t0, double t1, aabb& output_box)const
{
	if (objects.empty())
		return false;

	aabb temp_box;
	bool first_box = true;

	for (auto& object : objects)
	{
		if (!object->boudingbox(t0, t1, temp_box))
			return false;

		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}

	return true;
}