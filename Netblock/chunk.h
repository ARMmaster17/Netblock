#pragma once

namespace chunk
{
	void GenerateChunk();
	bool BlockIsExposeed(int x, int y, int z);
	void DrawChunk(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
}