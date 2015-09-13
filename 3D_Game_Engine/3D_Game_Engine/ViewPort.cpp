#include "ViewPort.h"
#include <GL/glew.h>
#include <iostream>

ViewPort::ViewPort()
{
}

ViewPort::ViewPort(
	std::string windowName, 
	int screenWidth, 
	int screenHeight, 
	unsigned int currentFlags)
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Tell SDL that we want a double buffered window so we don't get any flickering
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Uint32 flags = getFlags(currentFlags);

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Open a SDL Window
	m_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, flags);
	if (m_window == nullptr)
	{
		std::cout << "Window was not created!" << std::endl;
	}

	// Set up our OpenGl context
	m_glContext = SDL_GL_CreateContext(m_window);
	if (m_glContext == nullptr)
	{
		std::cout << "Window Context was not created!" << std::endl;
	}

	// Set up glew (optional but recommended)
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "GLEW was not initialized!" << std::endl;
	}

	// Check the OpenGL version
	std::printf("***   OpenGL Version: %s   ***", glGetString(GL_VERSION));
	std::cout << std::endl;
	std::cout << std::endl;

	glEnable(GL_DEPTH_TEST);

}

ViewPort::~ViewPort()
{
	SDL_GL_DeleteContext(m_glContext);
}


void ViewPort::swapBuffer(float r, float g, float b, float a)
{
	// Set the base depth to 1.0
	glClearColor(r, g, b, a);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void ViewPort::Update()
{

	SDL_GL_SwapWindow(m_window);

}

bool ViewPort::IsClosed()
{
	return m_isClosed;
}

Uint32 ViewPort::getFlags(unsigned int currentFlags)
{
	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE)
	{
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}
	if (currentFlags & RESIZABLE)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	}

	return flags;
}