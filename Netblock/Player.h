#pragma once
#include "Character.h"
class Player : public Character
{
public:
	GLuint ViewMatrixID;

	void UpdateCamera(glm::mat4& ProjectionMatrix, glm::mat4& ViewMatrix, GLuint& ViewMatrixID);
};

