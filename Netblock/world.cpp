#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "dynamicarray.h"
#include "graphics.h"
#include "chunk.h"
#include "Player.h"

#include "world.h"

void World::InitCharacters()
{
	// Initialize player.
	Player p;
	//characterList.push_back(p);
	this->player = &p;
	// Initialize other characters as needed.
}

void World::GenerateWorld()
{
	chunk::GenerateChunk();
}

void World::Update()
{
	/*for (Character c : characterList)
	{
		c.Update();
	}*/
	player->UpdateCamera(this->ProjectionMatrix, this->ViewMatrix, this->ViewMatrixID);
}

void World::Draw(GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc)
{
	chunk::DrawChunk(this->ProjectionMatrix, this->ViewMatrix, ModelMatrixID, MatrixID, bc);
}