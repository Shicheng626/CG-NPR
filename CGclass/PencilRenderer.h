
#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "SOIL.h"
// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class PencilRenderer {
public:
	GLuint level0;
	GLuint level1;
	GLuint level2;
	GLuint level3;
	GLuint level4;
	GLuint level5;

	float tile_factor = 5;
	bool inface_blending = true, align_to_normal = true;
	float max = 3.14;
	float k = 1/3.14;

	void setup() {
		level0 = LoadPencilTexture("hatching/hatch_0.jpg");
		level1 = LoadPencilTexture("hatching/hatch_1.jpg");
		level2 = LoadPencilTexture("hatching/hatch_2.jpg");
		level3 = LoadPencilTexture("hatching/hatch_3.jpg");
		level4 = LoadPencilTexture("hatching/hatch_4.jpg");
		level5 = LoadPencilTexture("hatching/hatch_5.jpg");
	}

protected:
	GLint LoadPencilTexture(string path) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;
	}
};