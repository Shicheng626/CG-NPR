#pragma once
#include "myMesh.h"
#include "myImporter.h"




//GLint TextureFromFile(const char* path, string directory);

class Model
{
public:
	vector<Mesh> meshes;
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model(GLchar* mesh_path, GLchar* diffuse_path,GLchar* normal_path)
	{
		this->loadModel(mesh_path, diffuse_path, normal_path);
	}

	Model(string mesh_path,string diffuse_path,string normal_path)
	{
		this->loadModel(mesh_path, diffuse_path, normal_path);
	}

	// Draws the model, and thus all its meshes
	void Draw(Shader shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader);


	}

	void PencilDraw(Shader shader,PencilRenderer pencilrenderer)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].PencilDraw(shader, pencilrenderer);


	}

	void DrawOutline(Shader shader, PencilRenderer pencil)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].DrawOutline(shader, pencil);


	}

private:
	/*  Model Data  */

	string directory;
	vector<Texture> textures_loaded;	

	void loadModel(string mesh_path,string diffuse_path,string normal_path) {
		myImporter Importer;
		Importer.load_obj(mesh_path);
		vector<Vertex> new_vertices;
		vector<GLuint> new_indices;
		Texture new_normal_textures;
		Texture new_diffuse_textrue;
		for (Vertex v : Importer.vertices) {
			new_vertices.push_back(v);
		}
		new_diffuse_textrue.id = LoadTexture(diffuse_path);
		new_normal_textures.id = LoadTexture(normal_path);
		new_indices = Importer.indices;
		meshes.push_back(Mesh(new_vertices, new_indices, new_normal_textures,new_diffuse_textrue));


	}

	GLint LoadTexture(string path) {
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