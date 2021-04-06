#pragma once
#include "chunk.h"

class Player;

class World
{
public:
	GLuint ViewMatrixID;
	Player* player;
	chunk::Chunk* chunk;

	void InitCharacters();
	void GenerateWorld();
	void Draw(GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
	void Update();
private:
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
};