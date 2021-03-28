#pragma once
#include "chunk.h"

class Player;

class World
{
public:
	const int DRAW_DISTANCE = 2;

	GLuint ViewMatrixID;
	Player* player;
	chunk::Chunk** chunks;

	void Draw(GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
	void Update();
	World();
	~World();
private:
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
};