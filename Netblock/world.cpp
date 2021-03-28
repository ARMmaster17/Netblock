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

void World::Update()
{
	/*for (Character c : characterList)
	{
		c.Update();
	}*/
	player->Update(chunk);
	player->UpdateCamera(this->ProjectionMatrix, this->ViewMatrix, this->ViewMatrixID);
}

World::World()
{
	this->chunks = new chunk::Chunk* [DRAW_DISTANCE];
	for (int x = 0; x < DRAW_DISTANCE; x++)
	{
		this->chunks[x] = new chunk::Chunk[DRAW_DISTANCE];
		for (int z = 0; z < DRAW_DISTANCE; z++)
		{
			this->chunks[x][z] = new chunk::Chunk(x * chunk::CHUNK_WIDTH, z * chunk::CHUNK_WIDTH);
			this->chunks[x][z].GenerateChunk(0);
		}
	}
	
	chunk = new chunk::Chunk();
	chunk->GenerateChunk(0);

	// Initialize player.
	//characterList.push_back(p);
	player = new Player();
}

World::~World()
{
}

void World::Draw(GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc)
{
	chunk->DrawChunk(this->ProjectionMatrix, this->ViewMatrix, ModelMatrixID, MatrixID, bc, 0);
}