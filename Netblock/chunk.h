#pragma once

void GenerateChunk();
bool BlockIsExposeed(int x, int y, int z);
void DrawChunk(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ModelMatrixID, GLuint& MatrixID, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& elementbuffer, std::vector<unsigned short>& indices);