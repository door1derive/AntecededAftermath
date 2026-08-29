
#include "grap.h"

// dot (std::vector<float> * Vec3)
float operator * (std::vector<float> left, Vec3 righ) {
	float retu = left[0] * righ[0] + left[1] * righ[1] + left[2] * righ[2];
	return retu;
}

// scale (float * Vec3)
Vec3 operator * (float left, Vec3 righ) {
	Vec3 retu;
	retu.x = left * righ.x;
	retu.y = left * righ.y;
	retu.z = left * righ.z;
	return retu;
}

std::ostream & operator << (std::ostream & left, Vec3 & righ) {
	left << "(" << righ.x << ", " << righ.y << ", " << righ.z << ")";
	return left;
}

std::ostream & operator << (std::ostream & outp, Vert & righ) {
	outp << "(" << righ.x << ", " << righ.y << ", " << righ.z << ")";
	return outp;
}

// rotate a 3d vector
std::vector<float> VectRota3d__(std::vector<float> vect, std::vector<float> eule) {
	std::vector<std::vector<float>> VectRota2d__(std::vector<float> vect, float rota, unsigned int axis = 0);
	std::vector<float> vec_; vec_.push_back(vect[1]); vec_.push_back(vect[2]);
	std::vector<std::vector<float>> vecx = VectRota2d__(vec_, eule[0], 0); // y z
	vec_[0] = vect[0]; vec_[1] = vecx[1][0];
	std::vector<std::vector<float>> vecy = VectRota2d__(vec_, eule[1], 1); // x z
	vec_[0] = vecy[0][0]; vec_[1] = vecx[0][0];
	std::vector<std::vector<float>> vecz = VectRota2d__(vec_, eule[2], 2); // x y
	std::vector<float> retu; retu.push_back(vecz[0][0]); retu.push_back(vecz[1][0]); retu.push_back(vecy[1][0]);
	// TODO
	retu[0] *= 180.0 / pi; retu[1] *= 180.0 / pi; retu[2] *= 180.0 / pi;
	return retu;
}

// find an intersection point and direction for two (infinite) planes
int PlanInte(Plan *pla1, Plan *pla2) {
	std::vector<float> VectNorm(std::vector<float> vect);
	int DistTo__Inte(std::vector<float> vectOrig, std::vector<float> vectDire, std::vector<float> poin, std::vector<float> norm, float *dist);
	int retu = 0;
	float tole = 0.001;
	Vec3 inteDire = pla1->norm.Cros(pla2->norm);
	std::vector<float> inteDir_;
	inteDir_ = VectNorm(inteDire.List());
	inteDire = {inteDir_[0], inteDir_[1], inteDir_[2]};
	Vec3 edgeNor1 = inteDire.Cros(pla1->norm);
	Vec3 edgeNor2 = inteDire.Cros(pla2->norm);
	float dist = 0.0;
	int distTo__Inte = 0;
	std::vector<float> vect;
	float dot_ = 0.0;
	std::vector<float> edgeNo_1;
	std::vector<float> edgeNo_2;
	Vec3 intePoin;
	float magn = edgeNor1.Magn();
	if (magn > tole) {
		edgeNo_1 = VectNorm(edgeNor1.List());
		edgeNo_2 = VectNorm(edgeNor2.List());
		edgeNor1 = {edgeNo_1[0], edgeNo_1[1], edgeNo_1[2]};
		edgeNor2 = {edgeNo_2[0], edgeNo_2[1], edgeNo_2[2]};
		// get the distance from a point on the first plane to where the edge normal intersects with the second plane
		distTo__Inte = DistTo__Inte(pla1->cent.List(), edgeNor1.List(), pla2->cent.List(), pla2->norm.List(), &dist);
		// get the world point of intersection
		if (distTo__Inte == 0) {
			intePoin = edgeNor1 * dist;
			intePoin = intePoin + pla1->cent;
		}
		else {
			retu = -1;
		}
		// orient the edge normals to face original polygons
		if (distTo__Inte == 0) {
			vect = Vect(intePoin.List(), pla1->cent.List());
			dot_ = vect * edgeNor1;
			if (dot_ < 0.0) {
				// TODO: *=
				edgeNor1 = edgeNor1 * -1.0;
			}
			vect = Vect(intePoin.List(), pla2->cent.List());
			dot_ = vect * edgeNor2;
			if (dot_ < 0.0) {
				edgeNor2 = edgeNor2 * -1.0;
			}
		}
	}
	else {
		retu = -1;
	}
	if (retu == 0) {
		pla1->cent = intePoin;
		pla1->norm = inteDire;
		pla2->cent = edgeNor1;
		pla2->norm = edgeNor2;
	}
	return retu;
}

// TODO
Plan PolyTo__Plan(std::vector<Vec3> vertList) {
	Vec3 ListTo__Vec3(std::vector<float> vect);
	Plan retu;
	// !!! re-purposing cent and norm
	retu.cent = vertList[1] - vertList[0];
	retu.norm = vertList[2] - vertList[0];
	retu.norm = retu.cent.Cros(retu.norm);
	retu.cent = {0.0, 0.0, 0.0};
	for (unsigned int a = 0; a < vertList.size(); a++) {
		// TODO: += overload
		retu.cent = retu.cent + vertList[a];
	}
	if (vertList.size() != 0) {
		retu.cent = ListTo__Vec3(VectScal(retu.cent.List(), 1.0 / float(vertList.size())));
	}
	return retu;
}

Vec3 ListTo__Vec3(std::vector<float> vect) {
	Vec3 retu;
	if (vect.size() >= 1) retu.x = vect[0];
	if (vect.size() >= 2) retu.y = vect[1];
	if (vect.size() >= 3) retu.z = vect[2];
	return retu;
}

// check if two (flat) mesh polygons intersect
bool PolyInte(Mesh mes1, Mesh mes2) {
	Plan PolyTo__Plan(std::vector<Vec3> vertList);
	Vec3 ListTo__Vec3(std::vector<float> vect);
	std::vector<float> Vect(std::vector<float> vec1, std::vector<float> vec2);
	bool Ray_(Vec3 orig, Vec3 ray_, Mesh mesh, Plan plan);
	bool retu = false;
	std::vector<unsigned int> poly = mes1.polyList[0];
	unsigned int leng = poly.size();
	// TODO
	Plan plan = PolyTo__Plan(mes2.vertList);
	for (unsigned int a = 0; a < leng; a++) {
		unsigned int i1 = a;
		unsigned int i2 = a + 1;
		if (i2 == leng) {
			i2 = 0;
		}
		Vec3 vect = ListTo__Vec3(Vect(mes1.vertList[poly[i1]].List(), mes1.vertList[poly[i2]].List()));
		retu = Ray_(mes1.vertList[poly[i1]], vect, mes2, plan);
		if (retu) {
			break;
		}
	}
	return retu;
}

// TODO
bool Ray_(Vec3 orig, Vec3 ray_, Mesh mesh, Plan plan) {
	Vec3 ListTo__Vec3(std::vector<float> vect);
	int DistTo__Inte(std::vector<float> vectOrig, std::vector<float> vectDire, std::vector<float> poin, std::vector<float> norm, float *dist);
	float VectAngl(std::vector<float> vec1, std::vector<float> vec2);
	bool retu = false;
	float anglTole = 0.01;
	float magn = ray_.Magn();
	// TODO
	ray_ = ListTo__Vec3(VectNorm(ray_.List()));
	float dist;
	int distTo__Inte = DistTo__Inte(orig.List(), ray_.List(), plan.cent.List(), plan.norm.List(), &dist);
	if (distTo__Inte == 0 && dist > 0.0 && dist < magn) {
		Vec3 poin = ray_ * dist;
		poin = poin + orig;
		// TODO
		std::vector<Vec3> vectList;
		std::vector<unsigned int> poly = mesh.polyList[0];
		unsigned int leng = poly.size();
		for (unsigned int a = 0; a < leng; a++) {
			vectList.push_back(ListTo__Vec3(Vect(poin.List(), mesh.vertList[poly[a]].List())));
		}
		float angl = 0.0;
		for (unsigned int a = 0; a < leng - 1; a++) {
			angl += VectAngl(vectList[a].List(), vectList[a + 1].List());
		}
		angl += VectAngl(vectList[leng - 1].List(), vectList[0].List());
		if (fabs(360.0 - angl) < anglTole) {
			retu = true;
		}
	}
	return retu;
}

// split the first plane (polygon. pass [vertices, edges, polygons]) by the second plane (infinite plane. pass [center, normal])
std::vector<Mesh> PlanSpli(Mesh pla1, Plan pla2) {
	std::vector<float> Vect(std::vector<float> vec1, std::vector<float> vec2);
	float VectMagn(std::vector<float> vect);
	std::vector<float> VectNorm(std::vector<float> vect);
	int DistTo__Inte(std::vector<float> vectOrig, std::vector<float> vectDire, std::vector<float> poin, std::vector<float> norm, float *dist);
	std::vector<float> VectScal(std::vector<float> vect, float scal);
	std::vector<float> VectAdd_(std::vector<float> vec1, std::vector<float> vec2);
	std::vector<Mesh> retu;
	// TODO
	std::vector<Vec3> subdList;
	// does the plane bisect a vertex or on an edge
	std::vector<bool> vertInte;
	std::vector<Int2> edgeList;
	// polygon outline. list of vertex indices that outline the polygon in order
	std::vector<unsigned int> outl;
	unsigned int edgeKey_ = 0;
	unsigned int ver1 = pla1.edgeList[edgeKey_].a;
	unsigned int ver2 = 0;
	std::vector<bool> edgeBoolList;
	for (unsigned int a = 0; a < pla1.edgeList.size(); a++) {
		edgeBoolList.push_back(false);
	}
	int firs = -1;
	int seco = -1;
	bool key2 = true;
	bool brea = false;
	unsigned int temp = 0;
	Vec3 orig;
	Vec3 dest;
	std::vector<float> vect;
	float magn = 0.0;
	float dist = 0.0;
	unsigned int edv1 = 0;
	unsigned int edv2 = 0;
	bool matc = false;
	while (brea == false) {
		edgeBoolList[edgeKey_] = true;
		ver1 = pla1.edgeList[edgeKey_].a;
		ver2 = pla1.edgeList[edgeKey_].b;
		if (key2 == false) {
			temp = ver1;
			ver1 = ver2;
			ver2 = temp;
		}
		outl.push_back(ver2);
		orig = pla1.vertList[ver1];
		dest = pla1.vertList[ver2];
		// TODO
		std::vector<float> ori_ = orig.List();
		vect = Vect(orig.List(), dest.List());
		magn = VectMagn(vect);
		vect = VectNorm(vect);
		// get the distance to the intersection with the other plane
		// TODO
		int distTo__Inte = DistTo__Inte(ori_, vect, pla2.cent.List(), pla2.norm.List(), &dist);
		bool appe = false;
		std::vector<float> poinNew_;
		if (distTo__Inte == 0 && dist > 0.0 && dist < magn) {
			poinNew_ = VectAdd_(ori_, VectScal(vect, dist));
			bool edgeInte = true;
			// TODO: make a distance function
			if (VectMagn(Vect(poinNew_, dest.List())) < 0.001) {
				edgeInte = false;
				appe = true;
			}
			// if not, check if it sits on the edge
			if (edgeInte) {
				// if the distance from a point to the other plane is less than the length of the edge, subdivide
				if (dist > 0.001 && dist < magn) {
					appe = true;
				}
			}
		}
		if (appe) {
			subdList.push_back({poinNew_[0], poinNew_[1], poinNew_[2]});
			if (firs == -1) {
				firs = (int)outl.size() - 1;
			}
			else if (seco == -1) {
				seco = (int)outl.size() - 1;
			}
		}
		brea = true;
		for (unsigned int a = 0; a < pla1.edgeList.size(); a++) {
			if (edgeBoolList[a] == false) {
				edv1 = pla1.edgeList[a].a;
				edv2 = pla1.edgeList[a].b;
				matc = false;
				if (ver2 == edv1 || ver2 == edv2) {
					matc = true;
				}
				if (ver1 == edv1 || ver1 == edv2) {
					matc = false;
				}
				if (matc) {
					if (ver2 == edv2) {
						key2 = false;
					}
					else {
						key2 = true;
					}
					edgeKey_ = a;
					brea = false;
					break;
				}
			}
		}
		if (brea) {
			break;
		}
	}
	if (subdList.size() == 2) {
		////
		unsigned int vertInde = 0;
		std::vector<Vec3> vertList;
		std::vector<std::vector<unsigned int>> polyList;
		std::vector<unsigned int> poly;
		Mesh obje;
		////
		int a = 0;
		while (a < firs) {
			vertInde = outl[(unsigned int)a];
			vertList.push_back(pla1.vertList[vertInde]);
			a += 1;
		}
		vertInde = 0;
		vertList.push_back(subdList[vertInde]);
		vertInde = 1;
		vertList.push_back(subdList[vertInde]);
		a = seco;
		while (a < (int)outl.size()) {
			vertInde = outl[a];
			vertList.push_back(pla1.vertList[vertInde]);
			a += 1;
		}
		// check double verts
		bool appe = true;
		float tole = 0.001;
		a = (int)vertList.size() - 1;
		while (a >= 1) {
			int b = a - 1;
			while (b >= 0) {
				magn = VectMagn(Vect(vertList[a].List(), vertList[b].List()));
				if (magn <= tole) {
					vertList.erase(vertList.begin() + a);
					b = 0;
				}
				b -= 1;
			}
			a -= 1;
		}
		if (vertList.size() < 3) {
			appe = false;
		}		
		Int2 edge;
		if (appe) {
			edgeList.clear();
			poly.clear();
			polyList.clear();
			for (unsigned int a = 0; a < vertList.size(); a++) {
				if (a < vertList.size() - 1) {
					edge.a = a;
					edge.b = a + 1;
					edgeList.push_back(edge);
				}
				else {
					edge.a = a;
					edge.b = 0;
					edgeList.push_back(edge);
				}
				poly.push_back(a);
			}
			polyList.push_back(poly);
			obje.vertList = vertList;
			obje.edgeList = edgeList;
			obje.polyList = polyList;
			retu.push_back(obje);
		}
		vertList.clear();
		vertInde = 0;
		vertList.push_back(subdList[vertInde]);
		a = firs;
		while (a < seco) {
			vertInde = outl[a];
			vertList.push_back(pla1.vertList[vertInde]);
			a += 1;
		}
		vertInde = 1;
		vertList.push_back(subdList[vertInde]);
		a = vertList.size() - 1;
		while (a >= 1) {
			int b = a - 1;
			while (b >= 0) {
				magn = VectMagn(Vect(vertList[a].List(), vertList[b].List()));
				if (magn <= tole) {
					vertList.erase(vertList.begin() + a);
					b = 0;
				}
				b -= 1;
			}
			a -= 1;
		}
		if (vertList.size() < 3) {
			appe = false;
		}
		if (appe) {
			edgeList.clear();
			polyList.clear();
			poly.clear();
			for (unsigned int a = 0; a < vertList.size(); a++) {
				if (a < vertList.size() - 1) {
					edge.a = a;
					edge.b = a + 1;
					edgeList.push_back(edge);
				}
				else {
					edge.a = a;
					edge.b = 0;
					edgeList.push_back(edge);
				}
				poly.push_back(a);
			}
			polyList.push_back(poly);
			obje.vertList = vertList;
			obje.edgeList = edgeList;
			obje.polyList = polyList;
			retu.push_back(obje);
		}
	}
	return retu;
}

Mesh PlanGeom(Plan plan, float scal) {
	Vec3 VectTo__Eule3d__(Vec3 vect);
	std::vector<float> VectRota3d__(std::vector<float> vect, std::vector<float> eule);
	std::vector<float> VectScal(std::vector<float> vect, float scal);
	std::vector<float> VectAdd_(std::vector<float> vec1, std::vector<float> vec2);
	// create a plane with a normal equal to (1.0, 0.0, 0.0)
	std::vector<float> ver1; ver1.push_back(0.0); ver1.push_back(0.5); ver1.push_back(0.5);
	std::vector<float> ver2; ver2.push_back(0.0); ver2.push_back(-0.5); ver2.push_back(0.5);
	std::vector<float> ver3; ver3.push_back(0.0); ver3.push_back(-0.5); ver3.push_back(-0.5);
	std::vector<float> ver4; ver4.push_back(0.0); ver4.push_back(0.5); ver4.push_back(-0.5);
	Vec3 norm = VectTo__Eule3d__(plan.norm);
	std::vector<float> norv; norv.push_back(norm.x); norv.push_back(norm.y); norv.push_back(norm.z);
	// TODO
	ver1 = VectRota3d__(ver1, norv); ver2 = VectRota3d__(ver2, norv); ver3 = VectRota3d__(ver3, norv); ver4 = VectRota3d__(ver4, norv);
	ver1 = VectScal(ver1, scal); ver2 = VectScal(ver2, scal); ver3 = VectScal(ver3, scal); ver4 = VectScal(ver4, scal);
	std::vector<float> cenv; cenv.push_back(plan.cent.x); cenv.push_back(plan.cent.y); cenv.push_back(plan.cent.z);
	ver1 = VectAdd_(ver1, cenv); ver2 = VectAdd_(ver2, cenv); ver3 = VectAdd_(ver3, cenv); ver4 = VectAdd_(ver4, cenv);
	Vec3 ve_1 = {ver1[0], ver1[1], ver1[2]};
	Vec3 ve_2 = {ver2[0], ver2[1], ver2[2]};
	Vec3 ve_3 = {ver3[0], ver3[1], ver3[2]};
	Vec3 ve_4 = {ver4[0], ver4[1], ver4[2]};
	Mesh retu;
	retu.vertList.push_back(ve_1);
	retu.vertList.push_back(ve_2);
	retu.vertList.push_back(ve_3);
	retu.vertList.push_back(ve_4);
	Int2 edge;
	edge.a = 0; edge.b = 1;
	retu.edgeList.push_back(edge);
	edge.a = 1;	edge.b = 2;
	retu.edgeList.push_back(edge);
	edge.a = 2;	edge.b = 3;
	retu.edgeList.push_back(edge);
	edge.a = 3;	edge.b = 0;
	retu.edgeList.push_back(edge);
	std::vector<unsigned int> poly;
	poly.push_back(0); poly.push_back(1); poly.push_back(2); poly.push_back(3);
	retu.polyList.push_back(poly);
	return retu;
}

// convert a vector to an euler rotation
// assumes object was originally oriented towards +x
Vec3 VectTo__Eule3d__(Vec3 vect) {
	float tole = 0.0001;
	float x, y; x = y = 0.0;
	float z = atan2f(vect.y, vect.x);
	if (fabs(vect.x) >= tole) {
		float z2 = cosf(z);
		float x2 = 0.0;
		if (fabs(z2) >= tole) {
			x2 = vect.x / z2;
		}
		else {
			x2 = vect.x;
		}
		y = atan2f(-vect.z, x2);
	}
	else {
		if (vect.y > 0.0) {
			y = atan2f(-vect.z, vect.y);
		}
		else {
			y = atan2f(-vect.z, -vect.y);
		}
	}
	vect.x = x * 180.0 / pi; vect.y = y * 180.0 / pi; vect.z = z * 180.0 / pi;
	return vect;
}

std::vector<float> VectCros3d__(std::vector<float> vec1, std::vector<float> vec2) {
	std::vector<float> retu;
	retu.push_back(vec1[1] * vec2[2] - vec1[2] * vec2[1]);
	retu.push_back(vec1[0] * vec2[2] - vec1[2] * vec2[0]);
	retu.push_back(vec1[0] * vec2[1] - vec1[1] * vec2[0]);
	retu[1] *= -1.0;
	return retu;
}

// return the average vector in a list of vectors (to get the average location of a group of objects, as in CursTo__Sele(all_ = True), for example)
Vec3 Vec3Aver(std::vector<Vec3> vectList) {
	std::vector<float> VectScal(std::vector<float> vect, float scal);
	Vec3 retu;
	unsigned int a = 0;
	for (; a < vectList.size(); a++) {
		retu.x += vectList[a].x;
		retu.y += vectList[a].y;
		retu.z += vectList[a].z;
	}
	if (a != 0) {
		std::vector<float> ret_ = VectScal(retu.List(), 1.0 / a);
		retu.x = ret_[0];
		retu.y = ret_[1];
		retu.z = ret_[2];
	}
	return retu;
}

// rotate a 2 dimensional vector
// TODO
std::vector<std::vector<float>> VectRota2d__(std::vector<float> vect, float rota, unsigned int axis = 0) {
	std::vector<std::vector<float>> Matr(std::vector<std::vector<float>> mat1, std::vector<std::vector<float>>mat2, bool deco = true);
	std::vector<std::vector<float>> mat1;
	rota *= pi / 180.0;
	if (axis == 0 or axis == 2) {
		std::vector<float> row_;
		row_.push_back(cosf(rota));	row_.push_back(-sinf(rota));
		mat1.push_back(row_);
		row_[0] = sinf(rota); row_[1] = cosf(rota);
		mat1.push_back(row_);
	}
	else if (axis == 1) {
		std::vector<float> row_;
		row_.push_back(cosf(rota));	row_.push_back(sinf(rota));
		mat1.push_back(row_);
		row_[0] = -sinf(rota); row_[1] = cosf(rota);
		mat1.push_back(row_);
	}
	std::vector<std::vector<float>> mat2;
	std::vector<float> row_;
	row_.push_back(vect[0]);
	mat2.push_back(row_);
	row_[0] = vect[1];
	mat2.push_back(row_);
	std::vector<std::vector<float>> retu;
	retu = Matr(mat1, mat2);
	return retu;
}
