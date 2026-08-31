
#include <sstream>

// skybox geometry and texture coordinates
GLfloat sky_Vert[] = {	1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 
						1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 
						1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 
						-1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 
						1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 
						1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0};
GLfloat sky_Norm[] = {	0.0, 1.0, 0.0, 
						0.0, -1.0, 0.0, 
						1.0, 0.0, 0.0, 
						-1.0, 0.0, 0.0, 
						0.0, 0.0, 1.0, 
						0.0, 0.0, -1.0};

GLfloat sky_Uv__[] = {	1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 
						-1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 
						1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 
						-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 
						-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 
						-1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0};

struct Image {
	short int sizeX;
	short int sizeY;
	char *data;
};

int ImageLoad(std::string fileName, Image *image) {
	FILE *file;
	// size of the image in bytes.
	unsigned long size;	
	// number of planes in image (must be 1)
	unsigned short int planes;
	// number of bits per pixel (must be 24)
	unsigned short int bpp;
	unsigned long i;
	// make sure the file is there.
	// TODO: use fstream
	//if ((fopen_s(&file, fileName.c_str(), "rb")) != 0) return -1;
	if ((file = fopen(fileName.c_str(), "rb")) == NULL) return -1;
	// read width
	fseek(file, 18, SEEK_SET); fread(&image->sizeX, 2, 1, file);
	// read height
	fseek(file, 22, SEEK_SET); fread(&image->sizeY, 2, 1, file);
	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;
	fseek(file, 26, SEEK_SET);
	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) return -1;
	if (planes != 1) return -1;
	// read the bits per pixel
	if ((i = (unsigned long)fread(&bpp, 2, 1, file)) != 1) return -1;
	if (bpp != 24) return -1;
	// read the data.
	fseek(file, 54, SEEK_SET);
	image->data = (char *)malloc(size);
	if (image->data == NULL) return -1;
	if ((i = (unsigned long)fread(image->data, size, 1, file)) != 1) return -1;
	char temp;
	for (i = 0; i < size; i += 3) {
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	return 0;
}

Image * loadTexture(std::string fileName){
	Image *image1;
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) exit(-1);
	if (ImageLoad((char*)fileName.c_str(), image1) != 0) {
		exit(1);
	}
	return image1;
}

void TextInit(std::vector<unsigned int> objeImagList) {
	unsigned int leve = 0;
	std::string objeDire = "obje/";
	std::stringstream fileName;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures((GLsizei)objeImagList.size(), imagList);
	for (unsigned int a = 0; a < objeImagList.size(); a++) {
		fileName.str("");
		fileName << objeDire << a << "/text.bmp";
		Image *imag = loadTexture(fileName.str());
		glBindTexture(GL_TEXTURE_2D, imagList[a]);
		glTexImage2D(GL_TEXTURE_2D, leve, 3, imag->sizeX, imag->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, imag->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void Sky_Init() {
	Image *imaa = loadTexture("obje/sky_/east.bmp");
	Image *imab = loadTexture("obje/sky_/west.bmp");
	Image *imac = loadTexture("obje/sky_/nort.bmp");
	Image *imad = loadTexture("obje/sky_/sout.bmp");
	Image *imae = loadTexture("obje/sky_/up__.bmp");
	Image *imaf = loadTexture("obje/sky_/down.bmp");
	glGenTextures(6, sky_Imag);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_Imag[0]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_Imag[1]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_Imag[2]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_Imag[3]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_Imag[4]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_Imag[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 3, imaa->sizeX, imaa->sizeY, GL_RGB, GL_UNSIGNED_BYTE, imaa->data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 3, imab->sizeX, imab->sizeY, GL_RGB, GL_UNSIGNED_BYTE, imab->data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 3, imac->sizeX, imac->sizeY, GL_RGB, GL_UNSIGNED_BYTE, imac->data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 3, imad->sizeX, imad->sizeY, GL_RGB, GL_UNSIGNED_BYTE, imad->data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 3, imae->sizeX, imae->sizeY, GL_RGB, GL_UNSIGNED_BYTE, imae->data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 3, imaf->sizeX, imaf->sizeY, GL_RGB, GL_UNSIGNED_BYTE, imaf->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
