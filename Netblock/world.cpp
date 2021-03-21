#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics.h"
//#include "dynamicarray.h"
#include "world.h"

int chunkData[10][10][10];

const int CHUNK_WIDTH = 10;
const int CHUNK_HEIGHT = 10;

const int BLOCK_AIR = 0;
const int BLOCK_DIRT = 1;

void GenerateWorld()
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				chunkData[x][y][z] = BLOCK_DIRT;
			}
		}
	}
	chunkData[5][7][5] = BLOCK_DIRT;
}

bool BlockIsExposeed(int x, int y, int z)
{
	if (x - 1 < 0 || x + 1 > CHUNK_WIDTH - 1)
	{
		// Block faces outside of chunk, ignore this check for now.
		return true;
	}
	else if (chunkData[x + 1][y][z] == BLOCK_AIR || chunkData[x - 1][y][z] == BLOCK_AIR)
	{
		return true;
	}
	if (y - 1 < 0 || y + 1 > CHUNK_HEIGHT - 1)
	{
		// Block faces outside of chunk, ignore this check for now.
		return true;
	}
	else if (chunkData[x][y + 1][z] == BLOCK_AIR || chunkData[x][y - 1][z] == BLOCK_AIR)
	{
		return true;
	}
	if (z - 1 < 0 || z + 1 > CHUNK_WIDTH - 1)
	{
		// Block faces outside of chunk, ignore this check for now.
		return true;
	}
	else if (chunkData[x][y][z + 1] == BLOCK_AIR || chunkData[x][y][z - 1] == BLOCK_AIR)
	{
		return true;
	}
	return false;
}

void DrawWorld(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& elementbuffer, std::vector<unsigned short>& indices)
{
	//// Bind our texture in Texture Unit 0 (if changing textures)
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);
	//// Set our "myTextureSampler" sampler to use Texture Unit 0
	//glUniform1i(TextureID, 0);

	// TODO: Only draw cubes with at least one open face. (will require implementation of block class and chunk structures).

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				if (chunkData[x][y][z] != BLOCK_AIR)
				{
					if (BlockIsExposeed(x, y, z))
					{
						glm::mat4 ModelMatrix = glm::mat4(1.0);
						ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x, y, z));
						glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
						glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
						glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
						DrawFromBuffers(vertexbuffer, uvbuffer, normalbuffer, elementbuffer, indices);
					}
				}
			}
		}
	}
}