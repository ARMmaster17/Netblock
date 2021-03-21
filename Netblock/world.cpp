#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics.h"
//#include "dynamicarray.h"
#include "world.h"

void DrawWorld(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& elementbuffer, std::vector<unsigned short>& indices)
{
	//// Bind our texture in Texture Unit 0 (if changing textures)
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	//// Set our "myTextureSampler" sampler to use Texture Unit 0
	//glUniform1i(TextureID, 0);

	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{	
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x, 0.0f, z));
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			DrawFromBuffers(vertexbuffer, uvbuffer, normalbuffer, elementbuffer, indices);
		}
	}
}