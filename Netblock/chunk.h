#pragma once
#include "graphics.h"
namespace chunk
{
	const int CHUNK_WIDTH = 32;
	const int CHUNK_HEIGHT = 16;

	int chunkData[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

	const int BLOCK_AIR = 0;
	const int BLOCK_DIRT = 1;

	void GenerateChunk();
	bool BlockIsExposeed(int x, int y, int z);
	void DrawChunk(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
}