
/*
 * 2d and 3d graphics
 * 
 * structures
 * classes: vectors, vertices, polygons
 * 3d functions
 * 2d functions
 * 
 */
 
#pragma once

#include <iostream>
#include <cmath>
#include <vector>

#include "Math.cpp"

//#include "grap.cpp"

class Int2 {
	public:
		unsigned int a = 0;
		unsigned int b = 0;
	// index
	unsigned int & operator [] (unsigned int righ) {
		if (righ == 0) {
			return this->a;
		}
		else if (righ == 1) {
			return this->b;
		}
		return this->a;
	}
	void operator = (unsigned int *righ) {
		a = righ[0];
		b = righ[1];
	}
};

class Vec3 {
	public:
		float x = 0.0;
		float y = 0.0;
		float z = 0.0;
	// index
	float & operator [] (unsigned int righ) {
		if (righ == 0) {
			return this->x;
		}
		else if (righ == 1) {
			return this->y;
		}
		else if (righ == 2) {
			return this->z;
		}
		return this->x;
	}
	// add
	Vec3 operator + (Vec3 righ) {
		Vec3 retu;
		retu.x = x + righ.x;
		retu.y = y + righ.y;
		retu.z = z + righ.z;
		return retu;
	}
	// subtract
	Vec3 operator - (Vec3 righ) {
		Vec3 retu;
		retu.x = x - righ.x;
		retu.y = y - righ.y;
		retu.z = z - righ.z;
		return retu;
	}
	// dot
	float operator * (Vec3 righ) {
		float retu;
		retu = x * righ.x + y * righ.y + z * righ.z;
		return retu;
	}
	// dot
	float operator * (std::vector<float> righ) {
		float retu = x * righ[0] + y * righ[1] + z * righ[2];
		return retu;
	}
	// scale (Vec3 * float)
	Vec3 operator * (float righ) {
		Vec3 retu;
		retu.x = this->x * righ;
		retu.y = this->y * righ;
		retu.z = this->z * righ;
		return retu;
	}
	// magnitude
	float Magn() {
		float retu = x * x + y * y + z * z;
		retu = sqrtf(retu);
		return retu;
	}
	// normalize
	Vec3 Norm() {
		Vec3 retu;
		// TODO
		float magn = this->Magn();
		if (magn > 0.0) {
			retu.x = x / magn;
			retu.y = y / magn;
			retu.z = z / magn;
		}
		return retu;
	}
	// cross
	Vec3 Cros(Vec3 righ) {
		float a = y * righ.z - z * righ.y;
		float b = x * righ.z - z * righ.x;
		float c = x * righ.y - y * righ.x;
		Vec3 retu;
		retu.x = a;
		retu.y = -b;
		retu.z = c;
		return retu;
	}
	// 3d quaternion rotation
	void Quat(float angl, Vec3 axis) {
		Vec3 retu;
		float p = this->x;
		float q = this->y;
		float r = this->z;
		float t = angl * pi / 180.0f;
		t /= 2.0;
		float a = cosf(t);
		float b = sinf(t);
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;
		float i = r*(2*b*b*x*z+2*a*b*y)+b*b*p*x*x+2*b*b*q*x*y-b*b*p*y*y-b*b*p*z*z-2*a*b*q*z+a*a*p;
		float j = r*(2*b*b*y*z-2*a*b*x)-b*b*q*x*x+2*b*b*p*x*y+b*b*q*y*y-b*b*q*z*z+2*a*b*p*z+a*a*q;
		float k = r*(-b*b*x*x-b*b*y*y+b*b*z*z+a*a)+x*(2*b*b*p*z+2*a*b*q)+y*(2*b*b*q*z-2*a*b*p);
		this->x = i;
		this->y = j;
		this->z = k;
	}
	// set the Vec3 equal to a list
	std::vector<float> List() {
		std::vector<float> retu;
		retu.push_back(x);
		retu.push_back(y);
		retu.push_back(z);
		return retu;
	}
};

class Plan {
	public:
		Vec3 cent;
		Vec3 norm;
};

class Mesh {
	public:
		// vertList
		std::vector<Vec3> vertList;
		// edgeList
		std::vector<Int2> edgeList;
		// polyList
		std::vector<std::vector<unsigned int>> polyList;
};

class Int3 {
	public:
		unsigned int a = 0;
		unsigned int b = 0;
		unsigned int c = 0;
	// index
	unsigned int & operator [] (unsigned int righ) {
		if (righ == 0) {
			return this->a;
		}
		else if (righ == 1) {
			return this->b;
		}
		else if (righ == 2) {
			return this->c;
		}
		return this->a;
	}
	void operator = (unsigned int *righ) {
		a = righ[0];
		b = righ[1];
		c = righ[2];
	}
};

class Vert {
	public:
		float x = 0.0;
		float y = 0.0;
		float z = 0.0;
		float u = 0.0;
		float v = 0.0;
		float w = 0.0;
	// index
	float & operator [] (unsigned int righ) {
		if (righ == 0) {
			return this->x;
		}
		else if (righ == 1) {
			return this->y;
		}
		else if (righ == 2) {
			return this->z;
		}
		else if (righ == 3) {
			return this->u;
		}
		else if (righ == 4) {
			return this->v;
		}
		else if (righ == 5) {
			return this->w;
		}
		else {
			std::cout << "overflow" << std::endl;
		}
		return this->x;
	}
	Vert operator - (Vec3 righ) {
		Vert retu;
		retu.x = x - righ.x;
		retu.y = y - righ.y;
		retu.z = z - righ.z;
		return retu;
	}
	operator Vec3() {
		Vec3 retu;
		retu.x = x;
		retu.y = y;
		retu.z = z;
		return retu;
	}
	// magnitude
	float Magn() {
		float retu = x * x + y * y + z * z;
		retu = sqrtf(retu);
		return retu;
	}
	// 3d quaternion rotation
	void Quat(float angl, Vec3 axis) {
		float p = this->x;
		float q = this->y;
		float r = this->z;
		float t = angl * pi / 180.0f;
		t /= 2.0f;
		float a = cosf(t);
		float b = sinf(t);
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;
		float i = r*(2*b*b*x*z+2*a*b*y)+b*b*p*x*x+2*b*b*q*x*y-b*b*p*y*y-b*b*p*z*z-2*a*b*q*z+a*a*p;
		float j = r*(2*b*b*y*z-2*a*b*x)-b*b*q*x*x+2*b*b*p*x*y+b*b*q*y*y-b*b*q*z*z+2*a*b*p*z+a*a*q;
		float k = r*(-b*b*x*x-b*b*y*y+b*b*z*z+a*a)+x*(2*b*b*p*z+2*a*b*q)+y*(2*b*b*q*z-2*a*b*p);
		this->x = i;
		this->y = j;
		this->z = k;
	}
};

class Poly {
	public:
		// 3 vertex indices that make up a polygon
		Int3 inde;
		// x y z local location of polygon centers
		Vec3 cent;
		// x y z that points in the direction of the face angle
		Vec3 norm;
		// r g b
		Vec3 colo;
		// set to false to stop the polygon from being drawn
		bool draw = true;
	unsigned int & operator [] (unsigned int righ) {
		if (righ == 0) {
			return this->inde.a;
		}
		else if (righ == 1) {
			return this->inde.b;
		}
		else if (righ == 2) {
			return this->inde.c;
		}
		else {
			std::cout << "overflow" << std::endl;
		}
		return this->inde.a;
	}
};
