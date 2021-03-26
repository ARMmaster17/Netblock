#include <gl/glew.h>
#include <GLFW/glfw3.h>
extern GLFWwindow* window;
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <stdio.h>
#include "controls.h"
#include "chunk.h"
#include "Player.h"

const float mouseSpeed = 0.005f;
const float heightOffset = 2.0f;
const float deAcceleration = -0.3f;
const float jumpPower = 34.0f;
const float gravity = -2.0f;

bool isJumping;
bool destroyAction;

void Player::UpdateCamera(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ViewMatrixID)
{
	// Generate MVP maticies (will need to be replicated for each model rendered)
	computeMatricesFromInputs(&this->position, this->horizontalAngle, this->verticalAngle);
	ProjectionMatrix = getProjectionMatrix();
	// Camera matrix
	ViewMatrix = getViewMatrix();

	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This and ProgramID can be lifted if all objects use the same shader.
}

Player::Player()
{
	this->position = glm::vec3(16, 15, 16);
	// Initial horizontal angle : toward -Z
	this->horizontalAngle = 3.14f;
	// Initial vertical angle : none
	this->verticalAngle = 0.0f;
	this->speed = 3.0f; // 3 units / second
	this->velocity = glm::vec3(0, 0, 0);
	isJumping = false;
	destroyAction = false;
}

Player::~Player()
{
}

void Player::Update(chunk::Chunk* chunk)
{
	this->readControls();
	this->resolvePosition(chunk);
	if (destroyAction) {

	}
}

void Player::readControls()
{
	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Compute new orientation
	this->horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
	this->verticalAngle += mouseSpeed * float(768 / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//this->position += direction * deltaTime * speed;
		this->velocity += direction * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//this->position -= direction * deltaTime * speed;
		this->velocity -= direction * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//this->position += right * deltaTime * speed;
		this->velocity += right * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//this->position -= right * deltaTime * speed;
		this->velocity -= right * speed;
	}

	// Jump
	isJumping = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

	// Destroy
	destroyAction = (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

	
}

void Player::resolveClickAction(chunk::Chunk* chunk)
{
	// TODO: Raw tracing in separate library, here, on in chunk.cpp
}

void Player::resolvePosition(chunk::Chunk* chunk)
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	velocity.y += gravity;

	// Jump logic
	if (isJumping && chunk->chunkData[(int)position.x][(int)(position.y - 0.1f - heightOffset)][(int)position.z] != chunk::BLOCK_AIR)
	{
		velocity.y += jumpPower;
	}

	
	float futureX = position.x + (velocity.x * deltaTime);
	if (chunk->chunkData[(int)futureX][(int)(position.y - heightOffset)][(int)position.z] == chunk::BLOCK_AIR)
	{
		position.x = futureX;
	}
	float futureY = position.y + (velocity.y * deltaTime);
	if (chunk->chunkData[(int)position.x][(int)(futureY - heightOffset)][(int)position.z] == chunk::BLOCK_AIR)
	{
		position.y = futureY;
	}
	float futureZ = position.z + (velocity.z * deltaTime);
	if (chunk->chunkData[(int)position.x][(int)(position.y - heightOffset)][(int)futureZ] == chunk::BLOCK_AIR)
	{
		position.z = futureZ;
	}

	lastTime = currentTime;

	// Decay acceleration
	velocity.x *= 0.5f;
	velocity.y *= 0.5f;
	velocity.z *= 0.5f;
	// If value becomes too small, zero it.
	if (velocity.x < 0.1f && velocity.x > -0.1f) velocity.x = 0.0f;
	if (velocity.y < 0.1f && velocity.y > -0.1f) velocity.y = 0.0f;
	if (velocity.z < 0.1f && velocity.z > -0.1f) velocity.z = 0.0f;
}
