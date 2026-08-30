#include <iostream>
//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include </GL/GLEWbin/glew.h>
#include </GL/GLEWbin/wglew.h>
#include <gl/gl.h>
#include </GL/wglext.h>
#include <gl/glu.h>
// TODO: maybe move this up
#include </GL/GLFW/include/GLFW/glfw3.h>


GLFWwindow* window;
void Init(int* widt, int* heig, float skyr, float skyg, float skyb, float skya, bool fullScre, bool hideCurs) {
	GLFWerrorfun callback = 0;
	glfwSetErrorCallback(callback);
	if (!glfwInit()) {
		throw("oops");
		// TODO: use callback
	}
	// windowed
	// TODO: define w/h as int
	window = glfwCreateWindow(*widt, *heig, "ANTECEDED AFTERMATH", NULL, NULL);
	// full screen
	//window = glfwCreateWindow(widt, heig, "ANTECEDED AFTERMATH", glfwGetPrimaryMonitor(), NULL);
	// TODO
	if (!window) {
		glfwTerminate();
		//return -1;
	}
	glfwMakeContextCurrent(window);
	//gladLoadGLLoader((GLADloadproc(glfwGetProcAddress));
	void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
	glfwSetKeyCallback(window, key_callback);
	void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// TODO: use options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// reset cursor
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glClearColor(skyr, skyg, skyb, skya);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GL_FILL);
}

// TODO
void Clos() {
	glfwTerminate();
}

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
//std::vector<float> VectCros3d__(std::vector<float> vec1, std::vector<float> vec2);
//cameSpee = 0.08;
// place second camera at same spot relative to door
//Came cam2;
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
*/