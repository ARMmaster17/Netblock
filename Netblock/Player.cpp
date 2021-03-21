#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "controls.h"
#include "Player.h"

void Player::UpdateCamera(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ViewMatrixID)
{
	// Generate MVP maticies (will need to be replicated for each model rendered)
	computeMatricesFromInputs();
	ProjectionMatrix = getProjectionMatrix();
	// Camera matrix
	ViewMatrix = getViewMatrix();

	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This and ProgramID can be lifted if all objects use the same shader.
}
