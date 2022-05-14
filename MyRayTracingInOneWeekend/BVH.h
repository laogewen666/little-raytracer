#pragma once
#include"hitobjectlist.h"
#include<algorithm>
#include"rtweekend.h"
class bvh_node :public hitobject
{
public:
	bvh_node();

	bvh_node(hitobjectlist& list,double t0,double t1):bvh_node(list.objects,0,list.objects.size(),t0,t1){}

	bvh_node(std::vector<shared_ptr<hitobject>>& objects, size_t start, size_t end, double t0, double t1);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool boudingbox(double t0, double t1, aabb& output_box)const;

public:
	shared_ptr<hitobject> left;
	shared_ptr<hitobject> right;
	aabb box;
};

bool bvh_node::boudingbox(double t0, double t1, aabb& output_box)const
{
	output_box = box;
	return true;
}

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	if (!box.hit(r, t_min, t_max))
		return false;

	bool hit_left = left->hit(r, t_min, t_max,rec);
	bool hit_right = right->hit(r, t_min, t_max, rec);

	return hit_left || hit_right;

}


inline bool axis_compare(const shared_ptr<hitobject> a, const shared_ptr<hitobject> b, int axis)
{
	aabb box_a, box_b;

	if (!a->boudingbox(0, 0, box_a) || !b->boudingbox(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.min().e[axis] < box_b.min().e[axis];
}

bool x_compare(const shared_ptr<hitobject> a, const shared_ptr<hitobject> b)
{
	return axis_compare(a, b, 0);
}

bool y_compare(const shared_ptr<hitobject> a, const shared_ptr<hitobject> b)
{
	return axis_compare(a, b, 1);
}

bool z_compare(const shared_ptr<hitobject> a, const shared_ptr<hitobject> b)
{
	return axis_compare(a, b, 2);
}

bvh_node::bvh_node(std::vector<shared_ptr<hitobject>>& objects, size_t start, size_t end, double t0, double t1)
{
	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? x_compare : (axis == 1) ? y_compare : z_compare;

	size_t object_span = end - start;
	if (object_span == 1)
	{
		left = right = objects[start];
	}
	else if(object_span == 2)
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start+1];
			right = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin(), objects.end(), comparator);
		auto mid = start + object_span / 2;
		left = make_shared<bvh_node>(objects, start, mid, t0, t1);
		right = make_shared<bvh_node>(objects, mid, end, t0, t1);
	}

	aabb box_left, box_right;
	if (!left->boudingbox(t0, t1, box_left) || !right->boudingbox(t0, t1, box_right))
		std::cerr << "No bounding box in bvh_node constructor.\n";
	box = surrounding_box(box_left, box_right);
}
