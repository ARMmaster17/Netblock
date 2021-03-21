#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "dynamicarray.h"
#include "graphics.h"
#include "chunk.h"

#include "world.h"

namespace world
{
	void GenerateWorld()
	{
		chunk::GenerateChunk();
	}

	void DrawWorld(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc)
	{
		chunk::DrawChunk(ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
	}
}