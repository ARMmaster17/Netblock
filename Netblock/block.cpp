#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "graphics.h"
#include "block.h"

glm::mat4 preCompiledRotations[3];

void block::InitRotations()
{
	preCompiledRotations[0] = block::GetQuatMatrix(0.0f, 0.0f);
	preCompiledRotations[1] = block::GetQuatMatrix(0.0f, 90.0f);
	preCompiledRotations[2] = block::GetQuatMatrix(90.0f, 0.0f);
}

glm::mat4 block::GetQuatMatrix(float degX, float degZ)
{
	glm::vec3 eulerRotations(glm::radians(degX), 0, glm::radians(degZ));
	glm::quat quatRotation = glm::quat(eulerRotations);
	return glm::mat4_cast(quatRotation);
}

void block::DrawFace(int type, int x, int y, int z, int rotationNumber, glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

	glm::mat4 ModelMatrix;
	if (rotationNumber == 0)
	{
		ModelMatrix = translationMatrix;
	}
	else
	{
		ModelMatrix = translationMatrix * preCompiledRotations[rotationNumber];
	}
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	graphics::DrawFromBuffers(bc);
}
