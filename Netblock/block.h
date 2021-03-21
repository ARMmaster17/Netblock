#pragma once


namespace block
{
	void DrawFace(int type, int x, int y, int z, int rotationNumber, glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc);
	glm::mat4 GetQuatMatrix(float degX, float degZ);
	void InitRotations();
}