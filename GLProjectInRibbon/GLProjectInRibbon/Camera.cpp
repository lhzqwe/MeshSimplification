#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	Position = glm::vec3(x, y, z);
}

void Camera::SetFront(GLfloat x, GLfloat y, GLfloat z)
{
	Front = glm::vec3(x, y, z);
}

void Camera::SetUp(GLfloat x, GLfloat y, GLfloat z)
{
	Up = glm::normalize(glm::vec3(x, y, z));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch /* = true */)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	//Make sure that when pitch is out of bounds, screen dosen't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch <= -89.0f)
			this->Pitch = -89.0f;
	}

	//Update Front, Right and Up Vectors
	this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	yoffset *= 0.1;
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	//Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	//Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

void Camera::ProcessMouseScrollInCameraPosition(GLshort zDelta)
{
	GLfloat velocity = this->MovementSpeed * 0.1;
	if (zDelta > 0)
		this->Position += this->Front * velocity;
	else if (zDelta < 0)
		this->Position -= this->Front * velocity;
}

