#pragma once

namespace world
{
	void GenerateWorld();
	void DrawWorld(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
}