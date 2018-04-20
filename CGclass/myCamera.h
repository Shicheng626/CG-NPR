#pragma once


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint WIDTH = 16, HEIGHT = 9;

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front_vector;
	glm::vec3 up_vector;
	glm::vec3 right_vector;
	glm::vec3 global_up;
	glm::vec3 rotation;

	float near_plane = 0.1f;
	float far_plane = 100.0f;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3 rotation = glm::vec3(0.0f,-90.0f,0.0f))
	{
		this->position = position;
		this->global_up = up;
		this->rotation = rotation;

		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		//glm::mat4 test;
		//test = glm::translate(test, position);
		//return test;
		updateCameraVectors();
		return glm::lookAt(this->position, this->position + this->front_vector, this->up_vector);
	}

	glm::mat4 GetProjectionMatrix() {
		glm::mat4 projection_matrix;
		projection_matrix = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, near_plane, far_plane);
		return projection_matrix;
	}

	void MoveFoward(float stepsize)
	{
		updateCameraVectors();
		this->position += stepsize * this->front_vector;
	}

	void MoveRight(float stepsize)
	{
		updateCameraVectors();
		this->position += stepsize * this->right_vector;
	}

	void MoveUp(float stepsize)
	{
		updateCameraVectors();
		this->position += stepsize * this->up_vector;
	}

	void pitch(float stepsize) {
		this->rotation[0] += stepsize;
		updateCameraVectors();
	}

	void yaw(float stepsize) {
		this->rotation[1] += stepsize;
		updateCameraVectors();
	}

	void roll(float stepsize) {
		this->rotation[2] += stepsize;
		updateCameraVectors();
	}




	void reset() {
		this->position = { 0.0f,0.0f,5.0f };
		this->rotation = { 0.0f,-90.0f,0.0f };
		this->near_plane = 0.1f;
		this->far_plane = 100.0f;
		this->global_up = { 0.0f, 1.0f, 0.0f };
		updateCameraVectors();
	}
private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->rotation[1])) * cos(glm::radians(this->rotation[0]));
		front.y = sin(glm::radians(this->rotation[0]));
		front.z = sin(glm::radians(this->rotation[1])) * cos(glm::radians(this->rotation[0]));
		this->front_vector = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right_vector = glm::normalize(glm::cross(this->front_vector, this->global_up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up_vector = glm::normalize(glm::cross(this->right_vector, this->front_vector));

		glm::mat4 rotation_raw;
		glm::vec4 tmp_vector;
		rotation_raw = glm::rotate(rotation_raw, glm::radians(this->rotation[2]), this->front_vector);
		
		tmp_vector = { right_vector,0 };
		tmp_vector = rotation_raw * tmp_vector;

		this->right_vector = { tmp_vector[0] ,tmp_vector[1] ,tmp_vector[2] };

		tmp_vector = { up_vector,0 };
		tmp_vector = rotation_raw * tmp_vector;

		this->up_vector = { tmp_vector[0] ,tmp_vector[1] ,tmp_vector[2] };

	}

};