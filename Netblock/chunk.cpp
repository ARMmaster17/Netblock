#include <vector>
#include <ctime>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics.h"
//#include "dynamicarray.h"
#include "block.h"
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
		
		// TODO: Should probably move this.
		block::InitRotations();
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

	void DrawBlock(int type, int x, int y, int z, glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc)
	{
		//// Bind our texture in Texture Unit 0 (if changing textures)
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, 2);
		//// Set our "myTextureSampler" sampler to use Texture Unit 0
		//glUniform1i(1, 0);
		
		// Draw bottom face.
		if (y != 0 && chunkData[x][y - 1][z] == BLOCK_AIR)
		{
			glBindTexture(GL_TEXTURE_2D, 1);
			block::DrawFace(chunkData[x][y][z], x, y, z, 0, ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
		}
		// Draw top face.
		if (y == CHUNK_HEIGHT - 1 || chunkData[x][y + 1][z] == BLOCK_AIR)
		{
			glBindTexture(GL_TEXTURE_2D, 2);
			block::DrawFace(chunkData[x][y][z], x, y + 1, z, 0, ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
		}
		// Draw near face.
		if (x != 0 && chunkData[x - 1][y][z] == BLOCK_AIR)
		{
			glBindTexture(GL_TEXTURE_2D, 1);
			block::DrawFace(chunkData[x][y][z], x, y, z, 1, ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
		}
		// Draw far face.
		if (x != CHUNK_WIDTH - 1 && chunkData[x + 1][y][z] == BLOCK_AIR)
		{
			glBindTexture(GL_TEXTURE_2D, 1);
			block::DrawFace(chunkData[x][y][z], x + 1, y, z, 1, ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
		}
		// Draw right face.
		if (z != CHUNK_WIDTH - 1 && chunkData[x][y][z + 1] == BLOCK_AIR)
		{
			glBindTexture(GL_TEXTURE_2D, 1);
			block::DrawFace(chunkData[x][y][z], x, y, z, 2, ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
		}
		// Draw left face.
		if (z != 0 && chunkData[x][y][z - 1] == BLOCK_AIR)
		{
			glBindTexture(GL_TEXTURE_2D, 1);
			block::DrawFace(chunkData[x][y][z], x, y, z - 1, 2, ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
		}
	}

	void DrawChunk(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, graphics::BufferCollection& bc)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (int z = 0; z < CHUNK_WIDTH; z++)
				{
					if (chunkData[x][y][z] != BLOCK_AIR)
					{
						//if (BlockIsExposeed(x, y, z))
						//{
							DrawBlock(chunkData[x][y][z], x, y, z, ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
						//}
					}
				}
			}
		}
	}
}