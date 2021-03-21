#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "graphics.h"

namespace graphics
{
	void DrawFromBuffers(BufferCollection& bc)
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, bc.vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, bc.uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, bc.normalbuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc.elementbuffer);

		// Draw the triangles !
		glDrawElements(GL_TRIANGLES, bc.indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	}
}