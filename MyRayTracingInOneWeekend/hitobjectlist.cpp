#pragma once
#include"hitobject.h"
#include<memory>
#include<vector>


using std::shared_ptr;
using std::make_shared;

class hitobjectlist: public hitobject
{
public:
	hitobjectlist(){}
	hitobjectlist(shared_ptr<hitobject> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hitobject> object) { objects.push_back(object); }
	virtual bool hit(const ray& r, double t_max, double t_min,  hit_record& rec)const;
public:
	std::vector<shared_ptr<hitobject>> objects;
};

bool hitobjectlist::hit(const ray& r, double t_max, double t_min,  hit_record& rec)const
{
	hit_record temp_rec;
	bool hitanything = false;
	auto closest_so_far = t_max;

	for(const auto& object:objects)
		if (object->hit(r, closest_so_far, t_min, temp_rec))
		{
			closest_so_far = rec.t;
			hitanything = true;
			rec = temp_rec;
		}
	return hitanything;
}