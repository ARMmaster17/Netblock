#include <vector>
#include <ctime>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics.h"
//#include "dynamicarray.h"
#include "chunk.h"

namespace chunk
{
	const int CHUNK_WIDTH = 32;
	const int CHUNK_HEIGHT = 16;

	const int CHUNK_NOISE_VARIANCE_MAX = 2;

	int chunkData[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

	const int BLOCK_AIR = 0;
	const int BLOCK_DIRT = 1;

	void GenerateChunk()
	{
		/* Generate a new random seed from system time - do this once in your constructor */
		srand(time(0));

		/* Setup constants */
		const static int q = 15;
		const static float c1 = (1 << q) - 1;
		const static float c2 = ((int)(c1 / 3)) + 1;
		const static float c3 = 1.f / c1;

		/* random number in range 0 - 1 not including 1 */
		float random = 0.f;

		/* the white noise */
		float noise = 0.f;
		
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
				for (int z = 0; z < CHUNK_WIDTH; z++)
				{
					random = ((float)rand() / (float)(RAND_MAX + 1));
					noise = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
					int normalizedNoise = noise - 0.5f;
					int runningLevelSum = 0;
					int runningLevelCount = 0;
					for (int checkX = std::max(x - 1, 0); checkX <= std::min(x + 1, CHUNK_WIDTH - 1); checkX++)
					{
						for (int checkZ = std::max(z - 1, 0); checkZ <= std::min(z + 1, CHUNK_WIDTH - 1); checkZ++)
						{
							int yTop = CHUNK_HEIGHT - 1;
							while (chunkData[checkX][yTop][checkZ] == BLOCK_AIR && yTop > 0)
							{
								yTop--;
							}
							if (yTop == 0)
							{
								// Column does not exist yet, do not report in average.
								continue;
							}
							runningLevelSum += yTop;
							runningLevelCount++;
						}
					}
					int avgLevel = 0;
					if (runningLevelCount == 0)
					{
						avgLevel = CHUNK_HEIGHT / 2;
					}
					else
					{
						avgLevel = runningLevelSum / runningLevelCount;
					}
					int level = std::max(std::min((int)(noise * CHUNK_NOISE_VARIANCE_MAX) + avgLevel, CHUNK_HEIGHT), 0);
					for (int y = 0; y <= level; y++)
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

	void DrawChunk(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc)
	{
		//// Bind our texture in Texture Unit 0 (if changing textures)
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, Texture);
		//// Set our "myTextureSampler" sampler to use Texture Unit 0
		//glUniform1i(TextureID, 0);

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
							glm::mat4 ModelMatrix = glm::mat4(1.0f);
							ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x, y, z));
							ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f));
							glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
							glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
							glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
							graphics::DrawFromBuffers(bc);
						}
					}
				}
			}
		}
	}
}