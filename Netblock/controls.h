#pragma once

void computeMatricesFromInputs(glm::vec3 *position, float horizontalAngle, float verticalAngle);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();