#ifndef _VEC3_
#define _VEC3_
#include<iostream>

class vec3
{
public:
	vec3() :e{0,0,0} {}
	vec3(double e1, double e2, double e3) :e{ e1, e2, e3 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator -() const{ return vec3(-e[0],-e[1],-e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i)  { return e[i]; }

	vec3& operator+=(const vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const vec3& v)
	{
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}
	vec3& operator/=(const vec3& v)
	{
		e[0] /= v.e[0];
		e[1] /= v.e[1];
		e[2] /= v.e[2];
		return *this;
	}

	double length() const
	{
		return sqrt(length_square());
	}

	double length_square() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	void write_color(std::ostream& out,int sample_per_pixel)
	{
		auto scale = 1.0 / sample_per_pixel;
		auto r = sqrt(e[0] * scale);
		auto g = sqrt(e[1] * scale);
		auto b = sqrt(e[2] * scale);


		out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(g, 0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
	}

	inline static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max)
	{
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
public:
	double e[3];
};

inline std::ostream& operator<< (std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator + (const vec3 &u, const vec3 &v)
{
	return vec3(u.e[0]+ v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator - (const vec3 &u, const vec3 &v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator * (const vec3 &u, const vec3 &v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator * (const vec3 &u, double t)
{
	return vec3(u.e[0] * t, u.e[1] * t, u.e[2] * t);
}

inline vec3 operator * ( double t, const vec3 &u)
{
	return u*t;
}

inline vec3 operator / (const vec3 &u, double t)
{
	return (1 / t) * u;
}

inline double dot(const vec3 &u, const vec3 &v)
{
	return	u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 crossdot(const vec3& u, const vec3& v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

vec3 random_in_unit_sphere()
{
	while (true)
	{
		auto p = vec3::random(-1,1) ;
		if (p.length_square() >= 1)
			continue;
		return p;
	}
}

vec3 random_unit_vector()
{
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);

	return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_hemisphere(const vec3& normal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(normal, in_unit_sphere) > 0)
		return in_unit_sphere;
	else
	{
		return -in_unit_sphere;
	}
}

vec3 reflected(const vec3& r_in, const vec3& normal)
{
	return r_in - 2 * dot(r_in, normal) * normal;
}

vec3 refract(const vec3& in, const vec3& N, double etai_over_etat)
{
	auto cos_theta = dot(-in, N);
	vec3 R_parrell = etai_over_etat * (in + cos_theta * N);
	vec3 R_vertical = -sqrt(1 - R_parrell.length_square()) * N;
	return R_parrell + R_vertical;
}

double schlick(double cosine, double ref_idx)
{
	auto t0 = (1 - ref_idx) / (1 + ref_idx);
	t0 = t0 * t0;
	return t0 + (1 - t0) * pow((1-cosine), 5);
}

vec3 random_in_unit_disk()
{
	while (true)
	{
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_square() > 1)
			continue;
		return p;
		
	}
}
#endif