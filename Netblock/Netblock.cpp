// Netblock.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define GLFW_DLL

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW
#include <gl/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "shaderhelpers.h"
#include "controls.h"
#include "texturehelpers.h"
#include "modelhelpers.h"
#include "vbo.h"
#include "graphics.h"
#include "world.h"

int main()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "NetBlock", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// Create Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Load and compile shader programs
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("grass.dds");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("cube.obj", vertices, uvs, normals);

	graphics::BufferCollection bc;///////////////////////////////////////////////////////////////////////////////////

	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, bc.indices, indexed_vertices, indexed_uvs, indexed_normals);

	// Load it into a VBO
	glGenBuffers(1, &bc.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bc.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &bc.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bc.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	glGenBuffers(1, &bc.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bc.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	glGenBuffers(1, &bc.elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc.elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bc.indices.size() * sizeof(unsigned short), &bc.indices[0], GL_STATIC_DRAW);

	world::GenerateWorld();

	// For speed computation
	double lastTime = glfwGetTime();
	double currentTime = 0;

	do {
		// Measure speed
		double previousTime = currentTime;
		currentTime = glfwGetTime();
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
			// printf and reset
			double timeDiff = currentTime - previousTime;
			printf("%f FPS\n", 1.0 / double(timeDiff));
			lastTime += 1.0;
		}

		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		// Generate MVP maticies (will need to be replicated for each model rendered)
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		// Camera matrix
		glm::mat4 ViewMatrix = getViewMatrix();

		glUseProgram(programID);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This and ProgramID can be lifted if all objects use the same shader.

		world::DrawWorld(ProjectionMatrix, ViewMatrix, ModelMatrixID, MatrixID, bc);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &bc.vertexbuffer);
	glDeleteBuffers(1, &bc.uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}