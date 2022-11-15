#ifndef _GL_DEBUG_HPP_
#define _GL_DEBUG_HPP_

#include <GL/glew.h>
#include <iostream>

#define APIENTRY

// callback to use with glDebugMessageCallback
void APIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

#endif