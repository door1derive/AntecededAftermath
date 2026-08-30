
/*
 * math functions
 * 
 * constants
 * vector functions
 * plane functions
 * matrix functions
 * 
 */
 
#pragma once
 
#include <cmath>

float pi = acosf(-1.0);

float VectMagn(std::vector<float> vect) {
	float VectDot_(std::vector<float> vec1, std::vector<float> vec2);
	return powf(VectDot_(vect, vect), 0.5);
}

std::vector<float> VectNorm(std::vector<float> vect) {
	float VectMagn(std::vector<float> vect);
	float tole = 0.0001f;
	float magn = VectMagn(vect);
	std::vector<float> retu;
	for (unsigned int a = 0; a < vect.size(); a++) {
		if (fabs(magn) >= tole) {
			retu.push_back(vect[a] / magn);
		}
		else {
			retu.push_back(0.0);
		}
	}
	return retu;
}

// return the average vector in a list of vectors (to get the average location of a group of objects, as in CursTo__Sele(all_ = True), for example)
std::vector<float> VectAver(std::vector<std::vector<float>> vectList) {
	std::vector<float> VectScal(std::vector<float> vect, float scal);
	std::vector<float> retu;
	if (vectList.size() > 0) {
		for (unsigned int a = 0; a < vectList[0].size(); a++) {
			retu.push_back(0.0);
		}
	}
	unsigned int a = 0;
	for (; a < vectList.size(); a++) {
		for (unsigned int b = 0; b < vectList[a].size(); b++) {
			retu[b] += vectList[a][b];
		}
	}
	if (a != 0) {
		retu = VectScal(retu, 1.0f / a);
	}
	return retu;
}

std::vector<float> VectScal(std::vector<float> vect, float scal) {
	for (unsigned int a = 0; a < vect.size(); a++) {
		vect[a] *= scal;
	}
	return vect;
}

std::vector<float> VectAdd_(std::vector<float> vec1, std::vector<float> vec2) {
	for (unsigned int a = 0; a < vec1.size(); a++) {
		vec1[a] += vec2[a];
	}
	return vec1;
}

std::vector<float> Vect(std::vector<float> vec1, std::vector<float> vec2) {
	std::vector<float> retu;
	for (unsigned int a = 0; a < vec1.size(); a++) {
		retu.push_back(vec2[a] - vec1[a]);
	}
	return retu;
}

float VectDot_(std::vector<float> vec1, std::vector<float> vec2) {
	float retu = 0.0;
	for (unsigned int a = 0; a < vec1.size(); a++) {
		retu += vec1[a] * vec2[a];
	}
	return retu;
}

float VectAngl(std::vector<float> vec1, std::vector<float> vec2) {
	float tole = 0.001f;
	float retu = 0.0;
	float deno = VectMagn(vec1) * VectMagn(vec2);
	if (fabs(deno) >= tole) {
		float argu = VectDot_(vec1, vec2) / deno;
		if (fabs(argu) <= 1.0 + tole) {
			if (argu > 1.0) {
				argu = 1.0;
			}
			if (argu < -1.0) {
				argu = -1.0;
			}
			retu = acosf(argu) * 180.0f / pi;
		}
		else {
			std::cout << "math domain error" << std::endl;
		}
	}
	else {
		std::cout << "approaching divide by 0" << std::endl;
	}
	return retu;
}

// get the distance to the point where a vector intersects a plane
// assumes vectDire and norm are normalized
int DistTo__Inte(std::vector<float> vectOrig, std::vector<float> vectDire, std::vector<float> poin, std::vector<float> norm, float *dist) {
	std::vector<float> Vect(std::vector<float> vec1, std::vector<float> vec2);
	float VectDot_(std::vector<float> vec1, std::vector<float> vec2);
	int retu = -1;
	*dist = 0.0;
	float tole = 0.0001f;
	// normal is the direction of the plane
	float deno = VectDot_(vectDire, norm);
	if (fabs(deno) >= tole) {
		retu = 0;
		// point is any point on the plane being checked for distance to ray origin. it could be the face center for the polygon, for example.
		std::vector<float> vect = Vect(vectOrig, poin);
		*dist = VectDot_(vect, norm) / deno;
	}
	return retu;
}

std::vector<std::vector<float>> Matr(std::vector<std::vector<float>> mat1, std::vector<std::vector<float>>mat2, bool deco = true) {
	std::vector<std::vector<float>> MatrDeco(std::vector<std::vector<float>> matr);
	float VectDot_(std::vector<float> vec1, std::vector<float> vec2);
	std::vector<std::vector<float>> retu;
	int leng = -1;
	if (mat1.size() > 0) {
		leng = (int)mat1[0].size();
	}
	// check if the column count of the first matrix matches the row count of the second matrix
	if ((int)mat2.size() == leng) {
		if (deco) {
			mat2 = MatrDeco(mat2);
		}
		for (unsigned int a = 0; a < mat1.size(); a++) {
			std::vector<float> row_;
			for (unsigned int b = 0; b < mat2.size(); b++) {
				std::vector<float> vec1 = mat1[a];
				std::vector<float> vec2 = mat2[b];
				row_.push_back(VectDot_(vec1, vec2));
			}
			retu.push_back(row_);
		}
	}
	else {
		std::cout << "matrix sizes do not match" << std::endl;
	}
	return retu;
}

// copy rows to columns
std::vector<std::vector<float>> MatrDeco(std::vector<std::vector<float>> matr) {
	std::vector<std::vector<float>> retu;
	int leng = -1;
	if (matr.size() > 0) {
		leng = (int)matr[0].size();
	}
	for (int a = 0; a < leng; a++) {
		std::vector<float> row_;
		for (unsigned int b = 0; b < matr.size(); b++) {
			row_.push_back(matr[b][a]);
		}
		retu.push_back(row_);
	}
	return retu;
}
