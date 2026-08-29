
#include <iostream>
#include <chrono>

#include "src/wind.cpp"

// TODO: how can data files access obje.h
//#include "obje.h"

#include "obje/data/data.cpp"
#include "src/imag.cpp"

// TODO: this was in the math file or something
#include <vector>

/*
std::vector<float> VectCros3d__(std::vector<float> vec1, std::vector<float> vec2) {
	std::vector<float> retu;
	retu.push_back(vec1[1] * vec2[2] - vec1[2] * vec2[1]);
	retu.push_back(vec1[0] * vec2[2] - vec1[2] * vec2[0]);
	retu.push_back(vec1[0] * vec2[1] - vec1[1] * vec2[0]);
	retu[1] *= -1.0;
	return retu;
}
*/

//#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>

//#include <iostream>
using namespace std;

int main(int argc, char **argv) {

	// came
	//void Load(std::vector<ObjeInit> &objeInitList, std::vector<ObjeDyna> &objeDynaList, std::vector<ObjeStat> &objeStatList, unsigned int *widt, unsigned int *heig, float *cameSpee, Came *came, float *skyr, float *skyg, float *skyb, float *skya, bool *sky_, bool *fullScre, bool *warpPoin, bool *hideCurs, bool *prin, int *timeFram, std::vector<unsigned int> &objeImagList);
	//void Init(unsigned int *widt, unsigned int *heig, float skyr, float skyg, float skyb, float skya, bool fullScre, bool warpPoin, bool hideCurs);
	//void Inpu(std::vector<bool> &key_Held, int *difx, int *dify, bool *runn, unsigned int widt, unsigned int heig, Display *disp, XEvent *even);
	// came
	//void Move(std::vector<ObjeDyna> &objeDynaList, Came *came, std::vector<bool> key_Held, int difx, int dify, float cameSpee, unsigned int widt, unsigned int heig, Display *disp, Window *wind, int soux, int souy, bool warpPoin);
	// came
	//void Draw(std::vector<ObjeDyna> objeDynaList, std::vector<ObjeStat> objeStatList, Came came, unsigned int widt, unsigned int heig, float persAngl, Display *disp, Window *wind, float skyr, float skyg, float skyb, float skya, bool sky_, bool prin);
	void Draw(std::vector<ObjeDyna> objeDynaList, std::vector<ObjeStat> objeStatList, Came came, unsigned int widt, unsigned int heig, float persAngl, float skyr, float skyg, float skyb, float skya, bool sky_, bool prin);
	//void Clos(Display *disp, GLXContext grap, Window wind);	
	//std::vector<ObjeInit> objeInitList; std::vector<ObjeDyna> objeDynaList; std::vector<ObjeStat> objeStatList;	unsigned int widt; unsigned int heig; float cameSpee; Came came; float skyr; float skyg; float skyb; float skya; bool sky_; bool fullScre; bool warpPoin; bool hideCurs; bool prin; int timeFram; std::vector<unsigned int> objeImagList;
	// came
	//Load(objeInitList, objeDynaList, objeStatList, &widt, &heig, &cameSpee, &came, &skyr, &skyg, &skyb, &skya, &sky_, &fullScre, &warpPoin, &hideCurs, &prin, &timeFram, objeImagList);
	//objeInitList.clear();

	//cout << "hello hell" << endl;

	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitWindowSize(800, 600);
	//glutCreateWindow("My OpenGL Window");

	/*
	SDL_Window* window;                    // Declare a pointer
	bool done = false;
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3
	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL3 window",                  // window title
		640,                               // width, in pixels
		480,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);
	*/

	//std::vector<bool> key_Held;
	vector<bool> key_Held;
	int difx = 0;
	int dify = 0;


	GLFWerrorfun callback = 0;
	glfwSetErrorCallback(callback);
	if (!glfwInit()) {
		throw("oops");
		// TODO: use callback
	}

	// windowed
	// TODO: check for error, if !window
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//return 0;
	// full screen
	//GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", glfwGetPrimaryMonitor(), NULL);
	glfwMakeContextCurrent(window);

	//gladLoadGLLoader((GLADloadproc(glfwGetProcAddress));

	
	//return 0;
	//GLFWkeyfun key_call = ;
	//GLFWkeyfun *key_call;
	//GLFWkeyfun* key_callback(GLFWwindow * window, int key, std::vector<bool>&key_Held, int* difx, int* dify, bool* runn);
	//GLFWK

	// TODO: wtf is a callback
	//void key_callback(GLFWwindow * window, int key, std::vector<bool>&key_Held, int* difx, int* dify, bool* runn);
	void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	
	glfwSetKeyCallback(window, key_callback);
	//return 0;
	


	//return 0;


	// make a window
	//Init(&widt, &heig, skyr, skyg, skyb, skya, fullScre, warpPoin, hideCurs);
	// break game loop if the window is closed
	//Atom clos = XInternAtom(disp, "WM_DELETE_WINDOW", false);
	//XSetWMProtocols(disp, wind, &clos, 1);

	/*

	TextInit(objeImagList);
	if (sky_) {
		Sky_Init();
	}
	// are these keys being held
	std::vector<bool> key_Held;
	// w, a, s, d, q, e
	key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false);
	key_Held.push_back(false); key_Held.push_back(false); key_Held.push_back(false);
	// mouse movement
	int difx = 0;
	int dify = 0;
	// timing
	long long unsigned int framCoun = 0;
	// TODO: auto
	auto timeNow_ = std::chrono::steady_clock::now();
	auto timeComp = std::chrono::steady_clock::now();
	auto timeElap = timeComp - timeNow_;
	*/

	////////////////////////////

	/*
	// create a framebuffer to render to and a texture to apply the render to
	// TODO: function
	glewInit();
	// TODO: what int should this be
	//GLuint FBO = 0;
	GLuint FBO = 0;
	//GLuint FBO = 2;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widt / 1, heig / 1, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// TODO: which settings are better
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, widt, heig);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);
	*/
	
	//////////////////////////////////////////////////////

	/*
	// basic fragment shader
	const char *vertexSource = "#version 130\n"
	"out vec4 FragColor;\n"
	"void main(){\n"
	"	FragColor = vec4(0.1f, 0.2f, 0.3f, 0.5f);\n"
	"}\n";
	GLuint ShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ShaderObj, 1, &vertexSource, NULL);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		std::cerr << "Error compiling shader type fragement shader" << std::endl;
	}
	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, ShaderObj);
	glLinkProgram(ShaderProgram);
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		std::cerr << "Error linking shader program" << std::endl;
	}
	*/

	//////////////////////////////////////////////////////
	
	//bool use_Sten = true;
	//use_Sten = false;
	
	/////////////////////////////////////
	
	// TODO:
	// include grap.h and dont define / declare vectcros3d__
	// check persAngl
	// set global clip start and clip end
	// delete object data
	// write a description for all functions
	
	//came.u = {0.0, 1.0, 0.0};
	//came.v = {0.0, 0.0, 1.0};
	//came.w = {-1.0, 0.0, 0.0};
	//came.loca.x = 6.0;
	//came.loca.y = 0.0;
	//came.loca.z = 2.0;

	// TODO: make sure this the "natural" angle and not 90.0
	//float persAngl = 53.13010235415598;
	//float persAngl = 2.0 * atan2f(0.5, 1.0) * 180.0 / 3.141592653589793;
	
	//std::vector<float> VectCros3d__(std::vector<float> vec1, std::vector<float> vec2);
	
	//cameSpee = 0.08;
	// place second camera at same spot relative to door
	//Came cam2;
	
	/////////////////////////////////////

	/*

	bool runn = true;
	while (runn) {
		if (XPending(disp)) {
			Inpu(key_Held, &difx, &dify, &runn, widt, heig, disp, &even);
		}
		else {
			timeComp = std::chrono::steady_clock::now();
			timeElap = timeComp - timeNow_;
			if (timeElap.count() > timeFram) {
				timeNow_ = std::chrono::steady_clock::now();
				Move(objeDynaList, &came, key_Held, difx, dify, cameSpee, widt, heig, disp, &wind, even.xmotion.x, even.xmotion.y, warpPoin);
				Draw(objeDynaList, objeStatList, cam2, widt, heig, persAngl, disp, &wind, skyr, skyg, skyb, skya, sky_, prin);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				bool use_Sten = true;
				use_Sten = false;
				if (use_Sten == true) {
					glEnable(GL_STENCIL_TEST);
					// TODO: are these both needed
					glClearStencil(0);
					glStencilMask(~0);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				}
				else {
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				}
				if (use_Sten == true) {
					glStencilFunc(GL_ALWAYS, 1, 0xFF);
					glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
					glStencilMask(0xFF);
					glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					glDepthMask(GL_FALSE);
				}
				if (use_Sten == true) {
					glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					glDepthMask(GL_TRUE);
					glStencilFunc(GL_EQUAL, 0, 0xFF);
					glStencilMask(0x00);
				}
				//glUseProgram(ShaderProgram);
				if (use_Sten == true) {
					glDisable(GL_STENCIL_TEST);
				}
				//glUseProgram(0);
				glFlush();
				glXSwapBuffers(disp, wind);
				if (prin == true) std::cout << "rendered" << std::endl;
				framCoun += 1;
			}
		}
		if (even.type == ClientMessage && (unsigned int)even.xclient.data.l[0] == clos) {
			runn = false;
		}
	}
	Clos(disp, glxc, wind);
	*/
	///*
	bool runn = true;
	//while (runn) {
	// TODO: callback function
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
		//glfwGetKeyScancode()





		//Draw(objeDynaList, objeStatList, cam2, widt, heig, persAngl, skyr, skyg, skyb, skya, sky_, prin);

		
		// TODO
		//int mods = glfwGet
		//key_callback(GLFWwindow * window, int key, int scancode, state, int mods, std::vector<bool>&key_Held, int* difx, int* dify, bool* runn)
		//key_callback(window, key, key_Held, &difx, &dify, &runn);
		//key_callback(window, (int)key_call, key_Held, &difx, &dify, &runn);
		//key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	}
	// TODO: update Clos function
	//glfwWindowShouldClose(window);
	//glfwDestroyWindow(window);
	glfwTerminate();
	//*/
	return 0;
}

//void key_callback(GLFWwindow* window, int key, std::vector<bool>& key_Held, int* difx, int* dify, bool* runn) {
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//int action = glfwGetKey(window, key);
	// TODO: what is this
	//int scancode = glfwGetKeyScancode(GLFW_KEY_ESCAPE);
	// TODO: modifier keys

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		//activate_airship();
		//dookie_on_face();
		//*runn = false;
		glfwDestroyWindow(window);
	}
}

/*
void Inpu(std::vector<bool>& key_Held, int* difx, int* dify, bool* runn, unsigned int widt, unsigned int heig, Display* disp, XEvent* even) {
	char keybInpu[1] = "";
	KeySym key;
	XNextEvent(disp, even);
	if (even->type == MotionNotify) {
		*difx = even->xmotion.x - widt / 2;
		*dify = even->xmotion.y - heig / 2;
	}
	if (even->type == KeyPress && XLookupString(&even->xkey, keybInpu, 255, &key, 0) == 1) {
		// ESC
		if (keybInpu[0] == 27) *runn = false;
		if (keybInpu[0] == 'w') key_Held[0] = true;
		if (keybInpu[0] == 'a') key_Held[1] = true;
		if (keybInpu[0] == 's') key_Held[2] = true;
		if (keybInpu[0] == 'd') key_Held[3] = true;
		if (keybInpu[0] == 'q') key_Held[4] = true;
		if (keybInpu[0] == 'e') key_Held[5] = true;
	}
	if (even->type == KeyRelease && XLookupString(&even->xkey, keybInpu, 255, &key, 0) == 1) {
		if (keybInpu[0] == 'w') key_Held[0] = false;
		if (keybInpu[0] == 'a') key_Held[1] = false;
		if (keybInpu[0] == 's') key_Held[2] = false;
		if (keybInpu[0] == 'd') key_Held[3] = false;
		if (keybInpu[0] == 'q') key_Held[4] = false;
		if (keybInpu[0] == 'e') key_Held[5] = false;
	}
}
*/


void Draw(std::vector<ObjeDyna> objeDynaList, std::vector<ObjeStat> objeStatList, Came came, unsigned int widt, unsigned int heig, float persAngl, float skyr, float skyg, float skyb, float skya, bool sky_, bool prin) {
//void Draw(std::vector<ObjeDyna> objeDynaList, std::vector<ObjeStat> objeStatList, Came came, unsigned int widt, unsigned int heig, Display *disp, Window *wind, float skyr, float skyg, float skyb, float skya, bool sky_, bool prin, GLuint renderedTexture) {
//void Draw(std::vector<ObjeDyna> objeDynaList, std::vector<ObjeStat> objeStatList, Came came, unsigned int widt, unsigned int heig, Display *disp, Window *wind, float skyr, float skyg, float skyb, float skya, bool sky_, bool prin, unsigned int renderedTexture) {
	unsigned int end_ = objeStatList.size();
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
		gluPerspective(persAngl, (float)widt / (float)heig, 0.1, 100.0);
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
		gluPerspective(persAngl, (float)widt / (float)heig, 0.001, 100.0);
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
		gluPerspective(persAngl, (float)widt / (float)heig, 0.1, 100.0);
		gluLookAt(0.0, 0.0, 0.0, came.w.x, came.w.y, came.w.z, came.v.x, came.v.y, came.v.z);
		glTranslatef(-came.loca.x, -came.loca.y, -came.loca.z);
		/////////////////
		// TODO: both blocks are mostly the same, make a function
		glTranslatef(objeDynaList[a].loca.x, objeDynaList[a].loca.y, objeDynaList[a].loca.z);
		glRotatef(objeDynaList[a].rota.x * pi / 180.0, 1.0, 0.0, 0.0);
		glRotatef(objeDynaList[a].rota.y * pi / 180.0, 0.0, 1.0, 0.0);
		glRotatef(objeDynaList[a].rota.z * pi / 180.0, 0.0, 0.0, 1.0);
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
void Move(std::vector<ObjeDyna> &objeDynaList, Came *came, std::vector<bool> key_Held, int difx, int dify, float cameSpee, unsigned int widt, unsigned int heig, Display *disp, Window *wind, int soux, int souy, bool warpPoin) {
	void LocaTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z);
	void RotaTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z);
	void ScalTran(std::vector<ObjeDyna> &objeDynaList, unsigned int inde, float x, float y, float z);
	// mouse look sensitivity
	float sens = 1.0 / 10.0;
	if (warpPoin && (difx != 0 || dify != 0)) {
		XWarpPointer(disp, None, *wind, soux, souy, widt, heig, (int)(widt / 2), (int)(heig / 2));
	}
	// handle horizotal mouse movement
	// axes around which mouse changes in x cause the camera to rotate
	Vec3 moux;
	moux.z = 1.0;
	if (difx != 0) {
		float mo_x = -difx * sens;
		came->u.Quat(mo_x, moux);
		came->w.Quat(mo_x, moux);
		came->v.Quat(mo_x, moux);
	}
	// handle vertical mouse movement
	// is the camera aimed above or below straight ahead. positive: above. negative: below.
	float cameVert = came->w * moux;
	if (dify != 0) {
		float mo_y = -dify * sens;
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
		came->loca.y -= cameSpee * sinf(angl - pi / 2.0);
		came->loca.x -= cameSpee * cosf(angl - pi / 2.0);
	}
	if (key_Held[2]) {
		came->loca.y -= cameSpee * sinf(angl);
		came->loca.x -= cameSpee * cosf(angl);
	}
	if (key_Held[3]) {
		came->loca.y += cameSpee * sinf(angl - pi / 2.0);
		came->loca.x += cameSpee * cosf(angl - pi / 2.0);
	}
	// set camera height
	if (key_Held[4]) {
		came->loca.z += cameSpee;
	}
	if (key_Held[5]) {
		came->loca.z -= cameSpee;
	}
}

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
