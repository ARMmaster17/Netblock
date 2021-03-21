#pragma once

namespace graphics
{
	struct BufferCollection
	{
		GLuint vertexbuffer;
		GLuint uvbuffer;
		GLuint normalbuffer;
		GLuint elementbuffer;
		std::vector<unsigned short> indices;
	};

	void DrawFromBuffers(BufferCollection& bc);
}