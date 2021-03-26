#pragma once
#include "graphics.h"
namespace chunk
{
	const int CHUNK_WIDTH = 32;
	const int CHUNK_HEIGHT = 16;

	const int BLOCK_AIR = 0;
	const int BLOCK_DIRT = 1;

	class Chunk
	{
	public:
		int*** chunkData;
		Chunk* xPosChunk;
		Chunk* xNegChunk;
		Chunk* zPosChunk;
		Chunk* zNegChunk;

		Chunk();
		~Chunk();
		void DrawBlock(int type, int x, int y, int z, glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
		void GenerateChunk(int distFromPlayer);
		bool BlockIsExposeed(int x, int y, int z);
		void DrawChunk(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc, int distFromPlayer);
	};
}