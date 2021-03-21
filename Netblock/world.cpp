#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "dynamicarray.h"
#include "world.h"
#include "chunk.h"



void GenerateWorld()
{
	GenerateChunk();
}

void DrawWorld(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& elementbuffer, std::vector<unsigned short>& indices)
{
	DrawChunk(ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, indices);
}