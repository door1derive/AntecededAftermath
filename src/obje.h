
/*
 * structures for stationary game classes
 * object and camera classes
 * 
 */
 
#pragma once

#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <array>
#include "Math.cpp"
#include "grap.h"

struct Vec3Stat {
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
};

struct Int3Stat {
	int a = 0;
	int b = 0;
	int c = 0;
};

struct VertStat {
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float u = 0.0;
	float v = 0.0;
	float w = 0.0;
};

struct PolyStat {
	// 3 vertex indices that make up a polygon
	Int3 inde;
	// x y z local location of polygon centers
	Vec3Stat cent;
	// x y z that points in the direction of the face angle
	Vec3Stat norm;
	// r g b
	Vec3Stat colo;
	// set to false to stop the polygon from being drawn
	bool draw = true;
};

class Came {
	public:
		Vec3 loca;
		Vec3 u, v, w;
		float rang = 1000.0;
		float clip = 0.1;
		Came() {
			u.x = 1.0;
			w.y = 1.0;
			v.z = 1.0;
		}
};

class ObjeInit {
	public:
		// object id
		unsigned int iden = 0;
		std::string name = "";
		Vec3 loca;
		Vec3 scal;
		Vec3 rota;
		unsigned int indeStar = 0;
		unsigned int indeEnd_ = 0;
		unsigned int imagInde = 0;
		// is the object static or dynamic. static objects dont move. their global vertex locations are precomputed
		bool stat = false;
		int shad = 0;
		std::vector<unsigned int> chilList;
};

class ObjeDyna {
	public:
		// object id
		unsigned int iden = 0;
		std::string name = "";
		Vec3 loca;
		Vec3 scal;
		Vec3 rota;
		unsigned int indeStar = 0;
		unsigned int indeEnd_ = 0;
		unsigned int imagInde = 0;
		int shad = 0;
		std::vector<unsigned int> chilList;
	void operator = (ObjeInit righ) {
		iden = righ.iden;
		name = righ.name;
		loca = righ.loca;
		scal = righ.scal;
		rota = righ.rota;
		indeStar = righ.indeStar;
		indeEnd_ = righ.indeEnd_;
		imagInde = righ.imagInde;
		shad = righ.shad;
		for (unsigned int a = 0; a < righ.chilList.size(); a++) {
			chilList.push_back(righ.chilList[a]);
		}
	}
};
	
class ObjeStat {
	public:
		// object id
		unsigned int iden = 0;
		std::string name = "";
		unsigned int indeStar = 0;
		unsigned int indeEnd_ = 0;
		unsigned int imagInde = 0;
		int shad = 0;
	void operator = (ObjeInit righ) {
		iden = righ.iden;
		name = righ.name;
		indeStar = righ.indeStar;
		indeEnd_ = righ.indeEnd_;
		imagInde = righ.imagInde;
		shad = righ.shad;
	}
};
