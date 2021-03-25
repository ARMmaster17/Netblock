#pragma once
#include "Character.h"
class Player : public Character
{
public:
	GLuint ViewMatrixID;
	// Initial position : on +Z
	glm::vec3 position;
	// Initial horizontal angle : toward -Z
	float horizontalAngle;
	// Initial vertical angle : none
	float verticalAngle;
	float speed; // 3 units / secondS

	Player();
	~Player();
	void Update(int world[chunk::CHUNK_WIDTH][chunk::CHUNK_HEIGHT][chunk::CHUNK_WIDTH]);
	void UpdateCamera(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ViewMatrixID);

private:
	glm::vec3 velocity;

	void readControls();
	void resolvePosition(int world[chunk::CHUNK_WIDTH][chunk::CHUNK_HEIGHT][chunk::CHUNK_WIDTH]);
};

