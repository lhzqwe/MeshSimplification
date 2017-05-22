#pragma once

//Std. Includes
#include <vector>

//GL Includes
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#ifndef GLFW_3_H
#define GLFW_3_H
#include <glfw3.h>
#endif


//Defines several possible options for camera movement.Used as abstraction to stay away from window-system specific
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
	//Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	//Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

public:
	//Deflault Constructor
	Camera();
	//Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	};
	//Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	void SetFront(GLfloat x, GLfloat y, GLfloat z);
	void SetUp(GLfloat x, GLfloat y, GLfloat z);

	//Returns the view matrix caculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();
	//Process input received from any keyboard-like input system.Accept input parameter in the from of camera defined ENUM
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	//Process input received from a mouse, input system.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	//Process input received from a mouse scroll-wheel event.
	void ProcessMouseScroll(GLfloat yoffset);
	//Process input mouse scroll in camera position
	void ProcessMouseScrollInCameraPosition(GLshort zDelta);
	~Camera();
private:
	//Calculate the front vector from the Camera's Eular Angles
	void updateCameraVectors();
};

