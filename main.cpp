
#include <iostream>
#include <chrono>
#include "src/wind.cpp"
#include "obje/data/data.cpp"
#include "src/imag.cpp"
using namespace std;

// TODO: is this secure
vector<bool> key_Held;
double difx = 0;
double dify = 0;
double prex = 0;
double prey = 0;

int main(int argc, char **argv) {

	// TODO:
	// move speed is off

	void Load(std::vector<ObjeInit> &objeInitList, std::vector<ObjeDyna> &objeDynaList, std::vector<ObjeStat> &objeStatList, int *widt, int *heig, float *cameSpee, Came *came, float *skyr, float *skyg, float *skyb, float *skya, bool *sky_, bool *fullScre, bool *warpPoin, bool *hideCurs, bool *prin, int *timeFram, std::vector<unsigned int> &objeImagList);
	void Init(int* widt, int* heig, float skyr, float skyg, float skyb, float skya, bool fullScre, bool hideCurs);
	void Move(std::vector<ObjeDyna> &objeDynaList, Came *came, float cameSpee, int widt, int heig);
	void Draw(std::vector<ObjeDyna> objeDynaList, std::vector<ObjeStat> objeStatList, Came came, int widt, int heig, float persAngl, float skyr, float skyg, float skyb, float skya, bool sky_, bool prin);
	// TODO: update
	void Clos();
	std::vector<ObjeInit> objeInitList; std::vector<ObjeDyna> objeDynaList; std::vector<ObjeStat> objeStatList;	int widt; int heig; float cameSpee; Came came; float skyr; float skyg; float skyb; float skya; bool sky_; bool fullScre; bool warpPoin; bool hideCurs; bool prin; int timeFram; std::vector<unsigned int> objeImagList;
	Load(objeInitList, objeDynaList, objeStatList, &widt, &heig, &cameSpee, &came, &skyr, &skyg, &skyb, &skya, &sky_, &fullScre, &warpPoin, &hideCurs, &prin, &timeFram, objeImagList);
	//cout << widt << " " << heig << endl;
	objeInitList.clear();

	// make a window
	Init(&widt, &heig, skyr, skyg, skyb, skya, fullScre, hideCurs);
	TextInit(objeImagList);
	if (sky_) Sky_Init();
	// w, a, s, d, q, e
	key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false);
	// timing
	long long unsigned int framCoun = 0;
	// TODO: auto
	auto timeNow_ = std::chrono::steady_clock::now();
	auto timeComp = std::chrono::steady_clock::now();
	auto timeElap = timeComp - timeNow_;

	// TODO: make sure this the "natural" angle and not 90.0
	// TODO: will be truncated. will a double be better, or will it be truncated by GL
	float persAngl = 53.13010235415598f;
	//float persAngl = 2.0 * atan2f(0.5, 1.0) * 180.0 / 3.141592653589793;

	// TODO: callback function
	while (!glfwWindowShouldClose(window)) {
		timeComp = std::chrono::steady_clock::now();
		timeElap = timeComp - timeNow_;
		if (timeElap.count() > timeFram) {
			timeNow_ = std::chrono::steady_clock::now();
			glfwSwapBuffers(window);
			// TODO: will this skip key presses?
			glfwPollEvents();
			Move(objeDynaList, &came, cameSpee, widt, heig);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Draw(objeDynaList, objeStatList, came, widt, heig, persAngl, skyr, skyg, skyb, skya, sky_, prin);
			glFlush();
			if (prin == true) std::cout << "rendered" << std::endl;
			framCoun += 1;
		}
	}
	Clos();
	return 0;
}

void Move(std::vector<ObjeDyna>& objeDynaList, Came* came, float cameSpee, int widt, int heig) {
	//void LocaTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z);
	//void RotaTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z);
	//void ScalTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z);
	// mouse look sensitivity
	float sens = 1.0f / 10.0f;
	// handle horizotal mouse movement
	// axes around which mouse changes in x cause the camera to rotate
	Vec3 moux;
	moux.z = 1.0;
	if (difx != 0) {
		float mo_x = (float) - difx * sens;
		came->u.Quat(mo_x, moux);
		came->w.Quat(mo_x, moux);
		came->v.Quat(mo_x, moux);
	}
	// handle vertical mouse movement
	// is the camera aimed above or below straight ahead. positive: above. negative: below.
	float cameVert = came->w * moux;
	if (dify != 0) {
		float mo_y = (float) - dify * sens;
		Vec3 camwNext = came->w;
		Vec3 camvNext = came->v;
		camwNext.Quat(mo_y, came->u);
		camvNext.Quat(mo_y, came->u);
		// will the cameras v axis be pointed in the negative z direction if the above rotation is applied
		float camvVert = camvNext * moux;
		if (camvVert < 0.0) {
			// set the camera to point straight up
			if (cameVert > 0.0) {
				came->w = moux;
				came->v = moux.Cros(came->u) * -1.0;
			}
			// set the camera to point straight down
			else {
				came->w = moux * -1.0;
				came->v = moux.Cros(came->u);
			}
		}
		// if the camera did not exceed either vertical limit, apply the rotation
		else {
			came->w = camwNext;
			came->v = camvNext;
		}
	}
	// get the angle the camera is facing on the xy plane
	float angl = 0.0;
	if (fabs(cameVert) < 0.9) {
		angl = atan2f(came->w.y, came->w.x);
	}
	else {
		angl = atan2f(came->v.y, came->v.x);
		if (cameVert > 0.0) {
			angl += pi;
		}
	}
	// change location according to camera angle
	if (key_Held[0]) {
		came->loca.y += cameSpee * sinf(angl);
		came->loca.x += cameSpee * cosf(angl);
	}
	if (key_Held[1]) {
		came->loca.y -= cameSpee * sinf(angl - pi / 2.0f);
		came->loca.x -= cameSpee * cosf(angl - pi / 2.0f);
	}
	if (key_Held[2]) {
		came->loca.y -= cameSpee * sinf(angl);
		came->loca.x -= cameSpee * cosf(angl);
	}
	if (key_Held[3]) {
		came->loca.y += cameSpee * sinf(angl - pi / 2.0f);
		came->loca.x += cameSpee * cosf(angl - pi / 2.0f);
	}
	// set camera height
	if (key_Held[4]) {
		came->loca.z += cameSpee;
	}
	if (key_Held[5]) {
		came->loca.z -= cameSpee;
	}
	difx = 0;
	dify = 0;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	difx = xpos - prex;
	dify = ypos - prey;
	prex = xpos;
	prey = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//int action = glfwGetKey(window, key);
	//int scancode = glfwGetKeyScancode(GLFW_KEY_ESCAPE);
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) glfwDestroyWindow(window);
		if (key == GLFW_KEY_W) key_Held[0] = true;
		if (key == GLFW_KEY_A) key_Held[1] = true;
		if (key == GLFW_KEY_S) key_Held[2] = true;
		if (key == GLFW_KEY_D) key_Held[3] = true;
		if (key == GLFW_KEY_Q) key_Held[4] = true;
		if (key == GLFW_KEY_E) key_Held[5] = true;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) key_Held[0] = false;
		if (key == GLFW_KEY_A) key_Held[1] = false;
		if (key == GLFW_KEY_S) key_Held[2] = false;
		if (key == GLFW_KEY_D) key_Held[3] = false;
		if (key == GLFW_KEY_Q) key_Held[4] = false;
		if (key == GLFW_KEY_E) key_Held[5] = false;
	}
}

// TODO: separate sky
// TODO: use one function for static and dynamic
void Draw(std::vector<ObjeDyna> objeDynaList, std::vector<ObjeStat> objeStatList, Came came, int widt, int heig, float persAngl, float skyr, float skyg, float skyb, float skya, bool sky_, bool prin) {
	unsigned int end_ = (unsigned int)objeStatList.size();
	if (sky_) {
		// dont draw cube map twice
		end_ -= 1;
		glDepthMask(GL_FALSE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, sky_Vert);
		glNormalPointer(GL_FLOAT, 0, sky_Norm);
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_2D, sky_Imag[0]);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, sky_Uv__);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(persAngl, (float)widt / (float)heig, 0.1, 1000.0);
		gluLookAt(0.0, 0.0, 0.0, came.w.x, came.w.y, came.w.z, came.v.x, came.v.y, came.v.z);		
		glDrawArrays(GL_QUADS, 0, 24);
		glPopMatrix();
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_CUBE_MAP);
		glDepthMask(GL_TRUE);
	}
	for (unsigned int a = 0; a < end_; a++) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertList);
		glNormalPointer(GL_FLOAT, 0, normList);
		if (objeStatList[a].shad == 0 || objeStatList[a].shad == 1) {
			glDisable(GL_TEXTURE_2D);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, coloList);
		}
		else if (objeStatList[a].shad == 2) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, imagList[objeStatList[a].imagInde]);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, uv__List);
		}
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(persAngl, (float)widt / (float)heig, 0.001, 1000.0);
		gluLookAt(0.0, 0.0, 0.0, came.w.x, came.w.y, came.w.z, came.v.x, came.v.y, came.v.z);
		glTranslatef(-came.loca.x, -came.loca.y, -came.loca.z);
		glDrawArrays(GL_TRIANGLES, objeStatList[a].indeStar / 3, (objeStatList[a].indeEnd_ - objeStatList[a].indeStar) / 3);
		glPopMatrix();
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		if (objeStatList[a].shad == 0 || objeStatList[a].shad == 1) {
			glDisableClientState(GL_COLOR_ARRAY);
		}
		else if (objeStatList[a].shad == 2) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
	for (unsigned int a = 0; a < objeDynaList.size(); a++) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertList);
		glNormalPointer(GL_FLOAT, 0, normList);
		if (objeDynaList[a].shad == 0 || objeDynaList[a].shad == 1) {
			glDisable(GL_TEXTURE_2D);
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, coloList);
		}
		else if (objeDynaList[a].shad == 2) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, imagList[0]);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, uv__List);
		}
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(persAngl, (float)widt / (float)heig, 0.1, 1000.0);
		gluLookAt(0.0, 0.0, 0.0, came.w.x, came.w.y, came.w.z, came.v.x, came.v.y, came.v.z);
		glTranslatef(-came.loca.x, -came.loca.y, -came.loca.z);
		/////////////////
		// TODO: both blocks are mostly the same, make a function
		glTranslatef(objeDynaList[a].loca.x, objeDynaList[a].loca.y, objeDynaList[a].loca.z);
		glRotatef(objeDynaList[a].rota.x * pi / 180.0f, 1.0, 0.0, 0.0);
		glRotatef(objeDynaList[a].rota.y * pi / 180.0f, 0.0, 1.0, 0.0);
		glRotatef(objeDynaList[a].rota.z * pi / 180.0f, 0.0, 0.0, 1.0);
		glScalef(objeDynaList[a].scal.x, objeDynaList[a].scal.y, objeDynaList[a].scal.z);
		/////////////////
		glDrawArrays(GL_TRIANGLES, objeDynaList[a].indeStar / 3, (objeDynaList[a].indeEnd_ - objeDynaList[a].indeStar) / 3);
		glPopMatrix();
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		if (objeDynaList[a].shad == 0 || objeDynaList[a].shad == 1) {
			glDisableClientState(GL_COLOR_ARRAY);
		}
		else if (objeDynaList[a].shad == 2) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
	if (prin == true) std::cout << "passed polygon data to GL" << std::endl;
}

/*
void LocaTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z) {
	std::vector<unsigned int> ChilList(std::vector<ObjeDyna> objeDynaList, unsigned int inde);
	std::vector<unsigned int> chilList = ChilList(objeDynaList, inde);
	for (unsigned int a = 0; a < chilList.size(); a++) {
		objeDynaList[chilList[a]].loca.x += x;
		objeDynaList[chilList[a]].loca.y += y;
		objeDynaList[chilList[a]].loca.z += z;
	}
}

void RotaTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z) {
	std::vector<unsigned int> ChilList(std::vector<ObjeDyna> objeDynaList, unsigned int inde);
	std::vector<unsigned int> chilList = ChilList(objeDynaList, inde);
	Vec3 vect;
	Vec3 axix = {1.0, 0.0, 0.0};
	Vec3 axiy = {0.0, 1.0, 0.0};
	Vec3 axiz = {0.0, 0.0, 1.0};
	objeDynaList[inde].rota.x += x;
	objeDynaList[inde].rota.y += y;
	objeDynaList[inde].rota.z += z;
	for (unsigned int a = 1; a < chilList.size(); a++) {
		vect = objeDynaList[chilList[a]].loca - objeDynaList[inde].loca;
		vect.Quat(x, axix);
		vect.Quat(y, axiy);
		vect.Quat(z, axiz);
		vect = vect + objeDynaList[inde].loca;
		vect = vect - objeDynaList[chilList[a]].loca;
		objeDynaList[chilList[a]].loca.x += vect[0];
		objeDynaList[chilList[a]].loca.y += vect[1];
		objeDynaList[chilList[a]].loca.z += vect[2];
		objeDynaList[chilList[a]].rota.x += x;
		objeDynaList[chilList[a]].rota.y += y;
		objeDynaList[chilList[a]].rota.z += z;
	}
}

void ScalTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z) {
	std::vector<unsigned int> ChilList(std::vector<ObjeDyna> objeDynaList, unsigned int inde);
	std::vector<unsigned int> chilList = ChilList(objeDynaList, inde);
	std::vector<float> vect;
	float ratx = (objeDynaList[inde].scal.x + x) / objeDynaList[inde].scal.x;
	float raty = (objeDynaList[inde].scal.y + y) / objeDynaList[inde].scal.y;
	float ratz = (objeDynaList[inde].scal.z + z) / objeDynaList[inde].scal.z;
	objeDynaList[inde].scal.x += x;
	objeDynaList[inde].scal.y += y;
	objeDynaList[inde].scal.z += z;
	for (unsigned int a = 1; a < chilList.size(); a++) {
		vect = Vect(objeDynaList[inde].loca.List(), objeDynaList[chilList[a]].loca.List());
		objeDynaList[chilList[a]].loca.x = vect[0] * ratx + objeDynaList[inde].loca.x;
		objeDynaList[chilList[a]].loca.y = vect[1] * raty + objeDynaList[inde].loca.y;
		objeDynaList[chilList[a]].loca.z = vect[2] * ratz + objeDynaList[inde].loca.z;
		objeDynaList[chilList[a]].scal.x += x;
		objeDynaList[chilList[a]].scal.y += y;
		objeDynaList[chilList[a]].scal.z += z;
	}
}

// TODO: do this before the game is run
std::vector<unsigned int> ChilList(std::vector<ObjeDyna> objeDynaList, unsigned int inde) {
	std::vector<unsigned int> retu;
	unsigned int a = 0;
	unsigned int size = 1;
	retu.push_back(inde);
	while (a < size) {
		for (unsigned int b = 0; b < objeDynaList[retu[a]].chilList.size(); b++) {
			retu.push_back(objeDynaList[retu[a]].chilList[b]);
			size += 1;
		}
		a += 1;
	}
	return retu;
}
*/
