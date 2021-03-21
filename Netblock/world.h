#pragma once

class Player;

class World
{
public:
	GLuint ViewMatrixID;
	Player* player;

	void InitCharacters();
	void GenerateWorld();
	void Draw(GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
	void Update();
private:
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
};